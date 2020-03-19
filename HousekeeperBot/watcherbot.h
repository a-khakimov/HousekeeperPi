#ifndef WATCHERBOT_H
#define WATCHERBOT_H

#include <stdint.h>
#include <memory>
#include <tgbot/tgbot.h>
#include <tgbot/net/CurlHttpClient.h>
#include "imgdiff.h"

class WatcherBot
{
public:
    WatcherBot(const std::string& token, const std::list<uint64_t>& chat_list);
    WatcherBot(const std::string& token, const std::list<uint64_t>& chat_list, const std::string& proxy);
    ~WatcherBot();

private:
    std::shared_ptr<TgBot::Bot> bot;
    TgBot::CurlHttpClient curl;
    std::list<uint64_t> chat_list;
    ImgDiffFinder idf;

    void init();
    void doStart(const int64_t id);
    void doStop(const int64_t id);
    void doHelp(const int64_t id);
    void doMyId(const int64_t id);
    void doPhoto(const int64_t id);
};

#endif // WATCHERBOT_H
