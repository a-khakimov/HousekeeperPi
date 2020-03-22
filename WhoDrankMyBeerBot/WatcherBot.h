/**
 *  @file   WatcherBot.h
 *  @brief
 *  @author a-khakimov
 ***********************************************/

#ifndef WATCHERBOT_H
#define WATCHERBOT_H

#include <stdint.h>
#include <memory>
#include <tgbot/tgbot.h>
#include <tgbot/net/CurlHttpClient.h>
#include "ImgDiffFinder.h"

/**
 * @brief The WatcherBot class
 */
class WatcherBot
{
public:
    /**
     * @brief WatcherBot
     * @param token - токен для авторизации в телеграм
     * @param chat_list - список id-чатов (пользователей), которые могут взаимодействовать с ботом
     * @param cameras - Список камер (см. HttpCamera), которые будут использоваться ботом
     */
    WatcherBot( const std::string&           token,
                const std::list<uint64_t>&   chat_list,
                const std::list<HttpCamera>& cameras
              );
    /**
     * @brief WatcherBot
     * @param token - токен для авторизации в телеграм
     * @param chat_list - список id-чатов (пользователей), которые могут взаимодействовать с ботом
     * @param cameras - Список камер (см. HttpCamera), которые будут использоваться ботом
     * @param proxy - адрес прокси сервера
     */
    WatcherBot( const std::string&           token,
                const std::list<uint64_t>&   chat_list,
                const std::list<HttpCamera>& cameras,
                const std::string&           proxy
              ); 
    /**
     * @brief Деструктор
     */
    virtual ~WatcherBot();

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

    /**
     * @brief init - Внутренняя функция для выполнения инициализации бота
     * @internal
     */
    void init                (                                );
    /**
     * @brief Обработчик
     * @param imgDiffPath - путь
     */
    void sendImageDiffToChat ( const std::string& imgDiffPath );
    /**
     * @brief Команда start для возобновления процесса мониторинга камер
     * @param id - Id чата (пользователя)
     */
    void doCmdStart          ( const int64_t id               );
    /**
     * @brief doCmdStop
     * @param id - Id чата (пользователя)
     */
    void doCmdStop           ( const int64_t id               );
    /**
     * @brief doCmdHelp
     * @param id - Id чата (пользователя)
     */
    void doCmdHelp           ( const int64_t id               );
    /**
     * @brief doCmdId
     * @param id - Id чата (пользователя)
     */
    void doCmdId             ( const int64_t id               );
    /**
     * @brief doCmdCamList
     * @param id - Id чата (пользователя)
     */
    void doCmdCamList        ( const int64_t id               );
    /**
     * @brief doCmdPhoto
     * @param id - Id чата (пользователя)
     */
    void doCmdPhoto          ( const int64_t id               );
    /**
     * @brief Метод для определения наличия привилегий у написавшего пользователя
     * @param id - Id чата (пользователя)
     * @return true - привилегия есть, false - привилегий нет
     */
    bool isUserHasRights     ( const int64_t id               );
};

#endif // WATCHERBOT_H
