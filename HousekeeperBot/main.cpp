/** 
 *  @file   main.cpp 
 *  @author a-khakimov 
 ***********************************************/

#include <list>
#include <stdio.h>
#include <tgbot/tgbot.h>
#include <tgbot/net/CurlHttpClient.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "plog/Log.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#include "cxxopts.hpp"
#include "imgdiff.h"

/*!
  \fn auto options(int argc, char** argv)
    Blah blah
*/
auto options(int argc, char** argv)
{
    try {
        std::string logFile;
        std::string confFile;
        cxxopts::Options options(argv[0]);
        options.positional_help("[optional args]").show_positional_help();
        options.add_options()
                ("h,help", "")
                ("l,logfile", "", cxxopts::value<std::string>(logFile)->default_value("HousekeeperBot.log"))
                ("c,configurations", "", cxxopts::value<std::string>(confFile));

        auto result = options.parse(argc, argv);
        if (result.count("help") or confFile.empty()) {
            std::cout << options.help({"", "Group"}) << std::endl;
            exit(0);
        }

        return std::tuple { logFile, confFile };
    }
    catch (const cxxopts::OptionException& e)
    {
        std::cerr << "error parsing options: " << e.what() << std::endl;
        exit(0);
    }
}

auto configurations(const std::string& confFile)
{
    std::ifstream i(confFile);
    nlohmann::json conf;
    i >> conf;

    std::string proxy_host = conf["proxy_host"];
    std::string token = conf["token"];

    std::list<uint64_t> chats;
    for (auto chat : conf["chats"]) {
        chats.push_back(chat);
    }

    std::list<HttpCamera> cameras;
    for (auto cam_cfg : conf["http_cameras"]) {
        HttpCamera cam(cam_cfg["host"], cam_cfg["port"], cam_cfg["path"]);
        cameras.push_back(cam);
    }

    return std::tuple { proxy_host, token, chats, cameras };
}

void doStart()
{

}


void doStop()
{

}


void doHelp()
{

}

void doMyId()
{

}

int main(int argc, char** argv) {
    try {
        const auto [logFile, confFile] = options(argc, argv);

        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::verbose, logFile.c_str()).addAppender(&consoleAppender);

        const auto [proxy_host, token, chat, cameras] = configurations(confFile);
        const std::list chats = chat;

        TgBot::CurlHttpClient curl;

        curl_easy_setopt(curl.curlSettings, CURLOPT_PROXY, proxy_host.c_str());
        curl_easy_setopt(curl.curlSettings, CURLOPT_SOCKS5_AUTH, CURLAUTH_BASIC);

        TgBot::Bot bot(token, curl);
        ImgDiffFinder imgdiff;
        imgdiff.onImgDiffFinded(1000, [&bot, &chats](double mse, std::string imgDiffPath, bool isOk) {
            if (not chats.empty()) {
                for (const auto& chat : chats) {
                    if (isOk) {
                        bot.getApi().sendMessage(chat, "Found diffs with mse:" + std::to_string(mse));
                        bot.getApi().sendPhoto(chat, TgBot::InputFile::fromFile(imgDiffPath, "image/png"));
                    } else {
                        bot.getApi().sendMessage(chat, "PiCameraServer is not avaliable");
                    }
                }
            } else {
                PLOG_WARNING << "chat_id is empty";
            }
        });

        imgdiff.onImgDiffFinded(1000, [&bot, &chats](double mse, std::string imgDiffPath, bool isOk) {
            if (not chats.empty()) {
                for (const auto& chat : chats) {
                    if (isOk) {
                        bot.getApi().sendMessage(chat, "Found diffs with mse:" + std::to_string(mse));
                        bot.getApi().sendPhoto(chat, TgBot::InputFile::fromFile(imgDiffPath, "image/png"));
                    } else {
                        bot.getApi().sendMessage(chat, "PiCameraServer is not avaliable");
                    }
                }
            } else {
                PLOG_WARNING << "chat_id is empty";
            }
        });

        bot.getEvents().onCommand("help", [&bot, &chats](TgBot::Message::Ptr message) {
            PLOG_INFO << "Command 'help' from " << message->chat->id;
            const bool found = std::find(chats.begin(), chats.end(), message->chat->id) != chats.end();
            if (found) {
                const std::string help = "/help - help message\n"
                                         "/photo - take photo and send";
                bot.getApi().sendMessage(message->chat->id, help);
            } else {
                PLOG_WARNING << "User " << message->chat->id << " does not have rights.";
            }
        });

        bot.getEvents().onCommand("myid", [&bot](TgBot::Message::Ptr message) {
            PLOG_INFO << "Command 'myid' from " << message->chat->id;
            const std::string msg("Your id is: " + std::to_string(message->chat->id));
            bot.getApi().sendMessage(message->chat->id, msg);
        });


        // TODO: start
        bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {

        });

        // TODO: stop
        bot.getEvents().onCommand("stop", [&bot](TgBot::Message::Ptr message) {

        });

        bot.getEvents().onCommand("photo", [&bot, &chats](TgBot::Message::Ptr message) {
            PLOG_INFO << "Command 'photo' from " << message->chat->id;
            const bool found = std::find(chats.begin(), chats.end(), message->chat->id) != chats.end();
            if (found) {
                HttpCamera camera("localhost", 1234, "/webcamera.png");
                auto [ img, isOk ] = camera.get();
                if (not isOk) {
                    PLOG_INFO << "is not ok";
                    return;
                }
                const std::string imgPath = "/tmp/fast.img.png";
                cv::imwrite(imgPath, img);
                bot.getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile(imgPath, "image/png"));
            } else {
                PLOG_WARNING << "User " << message->chat->id << " does not have rights.";
            }
        });

        TgBot::TgLongPoll longPoll(bot);
        PLOG_INFO << "Bot username: " << bot.getApi().getMe()->username;

        while (true) {
            PLOG_INFO << "Long poll...";
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        PLOG_ERROR << e.what();
    }
    catch (nlohmann::json::exception& e) {
        PLOG_ERROR << e.what();
    }

    return 0;
}
