/** 
 *  @file   main.cpp 
 *  @brief  Все начинается тут
 *  @author a-khakimov 
 ***********************************************/

#include <list>
#include "plog/Log.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#include <nlohmann/json.hpp>
#include "cxxopts.hpp"
#include "HttpCamera.h"
#include "WatcherBot.h"

/**
 * @brief Функция предназначенная для парсинга аргументов коммандной строки
 * @details Для парсинга используется библиотека cxxopts::Options
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
                ("l,logfile", "", cxxopts::value<std::string>(logFile)->default_value("WhoDrankMyBeer.log"))
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


/**
 * @brief Функция предназначенная для получения конфигураций из json-файла
 * @details Для парсинга json-файла используется библиотека nlohmann::json
 */
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


int main(int argc, char** argv) {
    try {
        const auto [logFile, confFile] = options(argc, argv);
        const auto [proxy_host, token, chats_list, cameras] = configurations(confFile);

        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::verbose, logFile.c_str()).addAppender(&consoleAppender);

        WatcherBot bot(token, chats_list, cameras, proxy_host);
    }
    catch (TgBot::TgException& e) {
        PLOG_ERROR << e.what();
    }
    catch (nlohmann::json::exception& e) {
        PLOG_ERROR << e.what();
    }

    return 0;
}
