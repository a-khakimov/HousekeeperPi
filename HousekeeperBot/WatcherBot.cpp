#include "WatcherBot.h"
#include "plog/Log.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

WatcherBot::WatcherBot(
        const std::string& token,
        const std::list<uint64_t>& chat_list,
        const std::list<HttpCamera>& cameras
) : chat_list(chat_list), cameras(cameras)
{
    bot = std::make_shared<TgBot::Bot>(token);
    init();
}

WatcherBot::WatcherBot(
        const std::string &token,
        const std::list<uint64_t>& chat_list,
        const std::list<HttpCamera>& cameras,
        const std::string &proxy
) : chat_list(chat_list), cameras(cameras)
{
    curl_easy_setopt(curl.curlSettings, CURLOPT_PROXY, proxy.c_str());
    curl_easy_setopt(curl.curlSettings, CURLOPT_SOCKS5_AUTH, CURLAUTH_BASIC);

    bot = std::make_shared<TgBot::Bot>(token, curl);
    init();
}

void WatcherBot::init()
{
    if (chat_list.empty()) {
        // TODO: throw exception
        PLOG_ERROR << "Chat id list is empty";
        return;
    }

    if (cameras.empty()) {
        // TODO: throw exception
        PLOG_ERROR << "Camera list is empty";
        return;
    }

    for (auto camera : cameras) {
        PLOG_INFO << "Use camera " << camera.info();
        diff_finders.push_back(ImgDiffFinder(camera));
    }

    for (auto img_diff_finder : diff_finders) {
        img_diff_finder.onImgDiffFinded(1000, [this](double mse, std::string imgDiffPath, bool isOk) {
            if (not chat_list.empty()) {
                for (const auto& chat : chat_list) {
                    if (isOk) {
                        bot->getApi().sendMessage(chat, "Found diffs with mse:" + std::to_string(mse));
                        bot->getApi().sendPhoto(chat, TgBot::InputFile::fromFile(imgDiffPath, "image/png"));
                    } else {
                        bot->getApi().sendMessage(chat, "PiCameraServer is not avaliable");
                    }
                }
            } else {
                PLOG_WARNING << "chat_id is empty";
            }
        });
    }


    bot->getEvents().onCommand("help", [this](TgBot::Message::Ptr message) {
        doHelp(message->chat->id);
    });

    bot->getEvents().onCommand("myid", [this](TgBot::Message::Ptr message) {
        doMyId(message->chat->id);
    });

    bot->getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
        doStart(message->chat->id);
    });

    bot->getEvents().onCommand("stop", [this](TgBot::Message::Ptr message) {
        doStop(message->chat->id);
    });

    bot->getEvents().onCommand("photo", [this](TgBot::Message::Ptr message) {
        doPhoto(message->chat->id);
    });

    TgBot::TgLongPoll longPoll(*bot);
    PLOG_INFO << "Bot username: " << bot->getApi().getMe()->username;

    while (true) {
        PLOG_INFO << "Long poll...";
        longPoll.start();
    }
}

void WatcherBot::doStart(const int64_t id)
{

}

void WatcherBot::doStop(const int64_t id)
{

}

void WatcherBot::doHelp(const int64_t id)
{
    PLOG_INFO << "Command 'help' from " << id;
    const bool found = std::find(chat_list.begin(), chat_list.end(), id) != chat_list.end();
    if (found) {
        const std::string help = "/help - help message\n"
                                 "/photo - take photo and send";
        bot->getApi().sendMessage(id, help);
    } else {
        PLOG_WARNING << "User " << id << " does not have rights.";
    }
}

void WatcherBot::doMyId(const int64_t id)
{
    PLOG_INFO << "Command 'myid' from " << id;
    const std::string msg("Your id is: " + std::to_string(id));
    bot->getApi().sendMessage(id, msg);
}

void WatcherBot::doPhoto(const int64_t id)
{
    PLOG_INFO << "Command 'photo' from " << id;
    const bool found = std::find(chat_list.begin(), chat_list.end(), id) != chat_list.end();
    if (found) {
        for (auto camera : cameras) {
            auto [ img, isOk ] = camera.get();
            if (not isOk) {
                PLOG_INFO << "Camera " << camera.info() << " is not avaliable" ;
                bot->getApi().sendMessage(id, "Camera " + camera.info() + " is not avaliable");
                return;
            }
            const std::string imgPath = "/tmp/fast.img.png";
            cv::imwrite(imgPath, img);
            bot->getApi().sendPhoto(id, TgBot::InputFile::fromFile(imgPath, "image/png"));
        }
    } else {
        bot->getApi().sendMessage(id, "You doesn't have rights");
        PLOG_WARNING << "User " << id << " does not have rights.";
    }
}

WatcherBot::~WatcherBot()
{

}
