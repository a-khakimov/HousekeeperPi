/**
 *  @file   WatcherBot.cpp
 *  @copydoc WatcherBot
 *  @author a-khakimov
 ***********************************************/

#include "WatcherBot.h"
#include "plog/Log.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

WatcherBot::WatcherBot(
        const std::string& token,
        const std::list<uint64_t>& chat_list,
        const std::list<HttpCamera>& cameras
) :  _botState(BotState::run), _chat_list(chat_list), _cameras(cameras)
{
    _bot = std::make_shared<TgBot::Bot>(token);
    init();
}

WatcherBot::WatcherBot(
        const std::string &token,
        const std::list<uint64_t>& chat_list,
        const std::list<HttpCamera>& cameras,
        const std::string &proxy
) :  _botState(BotState::run), _chat_list(chat_list), _cameras(cameras)
{
    curl_easy_setopt(_curl.curlSettings, CURLOPT_PROXY, proxy.c_str());
    curl_easy_setopt(_curl.curlSettings, CURLOPT_SOCKS5_AUTH, CURLAUTH_BASIC);
    _bot = std::make_shared<TgBot::Bot>(token, _curl);
    init();
}

void WatcherBot::init()
{
    if (_chat_list.empty()) {
        // TODO: throw exception
        PLOG_ERROR << "Chat id list is empty";
        return;
    }

    if (_cameras.empty()) {
        // TODO: throw exception
        PLOG_ERROR << "Camera list is empty";
        return;
    }

    for (auto& camera : _cameras) {
        PLOG_INFO << "Use camera " << camera.info();
        ImgDiffFinder* idf = new ImgDiffFinder(camera);
        _diff_finders.push_back(idf);
    }

    for (auto& finder : _diff_finders) {
        finder->onImgDiffFinded(1000, [this](const std::string& imgDiffPath, const bool isOk) {
            if (isOk) {
                if (_botState == BotState::run) {
                    sendImageDiffToChat(imgDiffPath);
                }
            }
        });
    }

    _bot->getEvents().onCommand("help", [this](TgBot::Message::Ptr message) {
        doCmdHelp(message->chat->id);
    });

    _bot->getEvents().onCommand("id", [this](TgBot::Message::Ptr message) {
        doCmdId(message->chat->id);
    });

    _bot->getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
        doCmdStart(message->chat->id);
    });

    _bot->getEvents().onCommand("stop", [this](TgBot::Message::Ptr message) {
        doCmdStop(message->chat->id);
    });

    _bot->getEvents().onCommand("photo", [this](TgBot::Message::Ptr message) {
        doCmdPhoto(message->chat->id);
    });

    _bot->getEvents().onCommand("list", [this](TgBot::Message::Ptr message) {
        doCmdCamList(message->chat->id);
    });

    TgBot::TgLongPoll longPoll(*_bot);
    PLOG_INFO << "Bot username: " << _bot->getApi().getMe()->username;

    while (true) {
        PLOG_INFO << "Long poll...";
        longPoll.start();
    }
}

void WatcherBot::sendImageDiffToChat(const std::string& imgDiffPath)
{
    for (const auto& chat : _chat_list) {
        _bot->getApi().sendPhoto(chat, TgBot::InputFile::fromFile(imgDiffPath, "image/png"));
    }
}

void WatcherBot::doCmdCamList(const int64_t id)
{
    PLOG_INFO << "Command 'list' from " << id;
    if (isUserHasRights(id)) {
        std::string msg;
        for (auto camera : _cameras) {
            msg += camera.info() + "\n";
        }
        _bot->getApi().sendMessage(id, msg);
    } else {
        _bot->getApi().sendMessage(id, "You doesn't have rights");
        PLOG_WARNING << "User " << id << " does not have rights.";
    }
}

void WatcherBot::doCmdStart(const int64_t id)
{
    PLOG_INFO << "Command 'start' from " << id;
    if (_botState == BotState::run) {
        _bot->getApi().sendMessage(id, "Bot already was started");
    } else {
        _botState = BotState::run;
        _bot->getApi().sendMessage(id, "Running...");
    }
}

void WatcherBot::doCmdStop(const int64_t id)
{
    PLOG_INFO << "Command 'stop' from " << id;
    if (_botState == BotState::sleep) {
        _bot->getApi().sendMessage(id, "Bot already was stoped");
    } else {
        _botState = BotState::sleep;
        _bot->getApi().sendMessage(id, "Stop!");
    }
}

void WatcherBot::doCmdHelp(const int64_t id)
{
    PLOG_INFO << "Command 'help' from " << id;
    if (isUserHasRights(id)) {
        const static std::string help = "/help - help message\n"
                                 "/start - start monitoring\n"
                                 "/stop - stop monitoring\n"
                                 "/id - send chat id\n"
                                 "/list - list of cameras\n"
                                 "/photo - take photo and send";
        _bot->getApi().sendMessage(id, help);
    } else {
        PLOG_WARNING << "User " << id << " does not have rights.";
    }
}

void WatcherBot::doCmdId(const int64_t id)
{
    PLOG_INFO << "Command 'id' from " << id;
    const std::string msg("Your id is: " + std::to_string(id));
    _bot->getApi().sendMessage(id, msg);
}

void WatcherBot::doCmdPhoto(const int64_t id)
{
    PLOG_INFO << "Command 'photo' from " << id;
    if (isUserHasRights(id)) {
        for (auto camera : _cameras) {
            auto [ img, isOk ] = camera.get();
            if (not isOk) {
                PLOG_INFO << "Camera " << camera.info() << " is not avaliable" ;
                _bot->getApi().sendMessage(id, "Camera " + camera.info() + " is not avaliable");
                continue;
            }
            const std::string imgPath = "/tmp/fast.img.png";
            cv::imwrite(imgPath, img);
            _bot->getApi().sendPhoto(id, TgBot::InputFile::fromFile(imgPath, "image/png"));
        }
    } else {
        _bot->getApi().sendMessage(id, "You doesn't have rights");
        PLOG_WARNING << "User " << id << " does not have rights.";
    }
}

bool WatcherBot::isUserHasRights(const int64_t id)
{
    return std::find(_chat_list.begin(), _chat_list.end(), id) != _chat_list.end();
}

WatcherBot::~WatcherBot()
{
    for (auto& finder : _diff_finders) {
        delete finder;
    }
}
