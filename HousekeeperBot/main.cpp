#include <stdio.h>
#include <tgbot/tgbot.h>
#include <tgbot/net/CurlHttpClient.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/quality.hpp>
#include "imgsource.h"
#include "timercpp.h"

int main() {
    try {
        std::ifstream i("/home/ainr/garage/HousekeeperPi/HousekeeperBot/conf.json");
        nlohmann::json conf;
        i >> conf;

        std::string proxy_host = conf["proxy_host"];
        std::string token = conf["token"];

        TgBot::CurlHttpClient curl;

        Timer t = Timer();
        t.setInterval([&]() {
            std::cout << "Hey.. After each 1s..." << std::endl;
        }, 1000);

        ImgSource isrc;
        cv::Mat img1 = isrc.get();
        cv::Mat img2 = isrc.get();
        cv::cvtColor(img1, img1, cv::COLOR_BGR2GRAY);
        cv::cvtColor(img2, img2, cv::COLOR_BGR2GRAY);
        cv::Mat out;
        cv::absdiff(img1, img2, out);
        cv::imwrite("/tmp/bot.img.out.0.png", img1);
        cv::imwrite("/tmp/bot.img.out.1.png", img2);
        cv::imwrite("/tmp/bot.img.out.2.png", out);

        while(1) {
            sleep(1);
        }


        return 0;

        curl_easy_setopt(curl.curlSettings, CURLOPT_PROXY, proxy_host.c_str());
        curl_easy_setopt(curl.curlSettings, CURLOPT_SOCKS5_AUTH, CURLAUTH_BASIC);

        TgBot::Bot bot(token, curl);

        bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
            bot.getApi().sendMessage(message->chat->id, "Hi!");
        });


        const std::string photoFilePath = "/tmp/img_23:21:08.png";
        const std::string photoMimeType = "image/png";

        bot.getEvents().onCommand("photo", [&bot, &photoFilePath, &photoMimeType](TgBot::Message::Ptr message) {
            bot.getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile(photoFilePath, photoMimeType));
        });

        bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
            if (StringTools::startsWith(message->text, "/start")) {
                return;
            }
            std::cout << ">\t id:" << message->chat->id << std::endl
                      << "\t username:" << message->chat->username << std::endl
                      << "\t lastName:" << message->chat->lastName << std::endl
                      << "\t firstName:" << message->chat->firstName << std::endl
                      << "\t text:" << message->text << std::endl
                      << "\t from->id:" << message->from->id << std::endl
                      << "\t from->isBot:" << message->from->isBot << std::endl;
            bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
        });

        std::cout << "Bot username: " << bot.getApi().getMe()->username << std::endl;
        TgBot::TgLongPoll longPoll(bot);

        while (true) {
            std::cout << "Long poll started" << std::endl;
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (nlohmann::json::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    //catch (...) {
    //    std::cerr << "Unknown exception" << std::endl;
    //}

    return 0;
}
