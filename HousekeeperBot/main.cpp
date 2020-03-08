#include <stdio.h>
#include <tgbot/tgbot.h>
#include <tgbot/net/CurlHttpClient.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

int main() {
    try {
        std::ifstream i("/home/ainr/garage/HousekeeperPi/HousekeeperBot/conf.json");
        nlohmann::json conf;
        i >> conf;

        std::string proxy_host = conf["proxy_host"];
        std::string token = conf["token"];

        TgBot::CurlHttpClient curl;

        const std::string photoFilePath = "/tmp/img_23:21:08.png";
        const std::string photoMimeType = "image/png";

        curl_easy_setopt(curl.curlSettings, CURLOPT_PROXY, proxy_host.c_str());
        curl_easy_setopt(curl.curlSettings, CURLOPT_SOCKS5_AUTH, CURLAUTH_BASIC);

        TgBot::Bot bot(token, curl);

        bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
            bot.getApi().sendMessage(message->chat->id, "Hi!");
        });


        bot.getEvents().onCommand("photo", [&bot, &photoFilePath, &photoMimeType](TgBot::Message::Ptr message) {
            bot.getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile(photoFilePath, photoMimeType));
        });

        bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
            printf("User wrote %s\n", message->text.c_str());
            if (StringTools::startsWith(message->text, "/start")) {
                return;
            }
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
    catch (...) {
        std::cerr << "Unknown exception" << std::endl;
    }

    return 0;
}
