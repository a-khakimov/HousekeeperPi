#include <stdio.h>
#include <tgbot/tgbot.h>
#include <tgbot/net/CurlHttpClient.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/quality.hpp>
#include "imgdiff.h"
#include "plog/Log.h"
#include "plog/Appenders/ColorConsoleAppender.h"


int main() {
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, "housekeeperPi.log").addAppender(&consoleAppender);

    try {
        std::ifstream i("/home/ainr/garage/HousekeeperPi/HousekeeperBot/conf.json");
        nlohmann::json conf;
        i >> conf;

        std::string proxy_host = conf["proxy_host"];
        std::string token = conf["token"];
        int64_t chat_id = conf["chat_id"];

        TgBot::CurlHttpClient curl;

        curl_easy_setopt(curl.curlSettings, CURLOPT_PROXY, proxy_host.c_str());
        curl_easy_setopt(curl.curlSettings, CURLOPT_SOCKS5_AUTH, CURLAUTH_BASIC);

        TgBot::Bot bot(token, curl);

        bot.getEvents().onCommand("help", [&bot, &chat_id](TgBot::Message::Ptr message) {
            PLOG_INFO << "Command 'help' from " << message->chat->id;
            if (message->chat->id == chat_id) {
                const std::string help = "/help - help message\n"
                                         "/photo - take photo and send";
                bot.getApi().sendMessage(message->chat->id, help);
            }
        });

        bot.getEvents().onCommand("photo", [&bot, &chat_id](TgBot::Message::Ptr message) {
            PLOG_INFO << "Command 'photo' from " << message->chat->id;
            if (message->chat->id == chat_id) {
                ImgSource isrc;
                auto [ img, isOk ] = isrc.get();
                if (not isOk) {
                    PLOG_INFO << "is not ok";
                    return;
                }
                const std::string imgPath = "/tmp/fast.img.png";
                cv::imwrite(imgPath, img);
                bot.getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile(imgPath, "image/png"));
            }
        });

        ImgDiffFinder imgdiff;
        auto imgDiffHandler = [&bot, &chat_id](double mse, std::string imgDiffPath) {
            PLOG_INFO << "Found diffs with mse=" << mse << " [" << imgDiffPath << "]";
            bot.getApi().sendMessage(chat_id, "Found diffs with mse:" + std::to_string(mse));
            bot.getApi().sendPhoto(chat_id, TgBot::InputFile::fromFile(imgDiffPath, "image/png"));
        };
        imgdiff.run(1000, imgDiffHandler);

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
