#ifndef WATCHERBOT_H
#define WATCHERBOT_H

#include <stdint.h>
#include <memory>
#include <tgbot/tgbot.h>
#include <tgbot/net/CurlHttpClient.h>
#include "ImgDiffFinder.h"

class WatcherBot
{
public:
    WatcherBot( const std::string&           token,
                const std::list<uint64_t>&   chat_list,
                const std::list<HttpCamera>& cameras
              );
    WatcherBot( const std::string&           token,
                const std::list<uint64_t>&   chat_list,
                const std::list<HttpCamera>& cameras,
                const std::string&           proxy
              );
    ~WatcherBot();

private:

    typedef enum {
        sleep,
        run
    } BotState;

    BotState                    _botState;
    std::shared_ptr<TgBot::Bot> _bot;
    TgBot::CurlHttpClient       _curl;
    std::list<uint64_t>         _chat_list;
    std::list<ImgDiffFinder*>   _diff_finders;
    std::list<HttpCamera>       _cameras;

    void init                (                                );
    void sendImageDiffToChat ( const std::string& imgDiffPath );
    void doCmdStart          ( const int64_t id               );
    void doCmdStop           ( const int64_t id               );
    void doCmdHelp           ( const int64_t id               );
    void doCmdId             ( const int64_t id               );
    void doCmdCamList        ( const int64_t id               );
    void doCmdPhoto          ( const int64_t id               );
    bool isUserHasRights     ( const int64_t id               );
};

#endif // WATCHERBOT_H
