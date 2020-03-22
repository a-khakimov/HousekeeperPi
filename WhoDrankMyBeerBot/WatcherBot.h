/**
 *  @author a-khakimov
 *  @file WatcherBot.h
 *  @brief Класс, содержащий логику для взаимодействия с пользователями.
 ***********************************************/

#ifndef WATCHERBOT_H
#define WATCHERBOT_H

#include <memory>
#include <stdint.h>
#include <tgbot/tgbot.h>
#include "ImgDiffFinder.h"

/**
 * @brief Класс, содержащий логику для взаимодействия с пользователями.
 * @details Задача 1 - Отслеживание изменений в камерах путем сравнения изображений посредством
 *                     использования класса ImgDiffFinder и оповещение пользователя при наличии
 *                     какой-либо активности \n
 *          Задача 2 - Выполнение пользовательских команд
 * @note Для работы с классом необходимо просто создать объект, что, думаю, не совсем правильно.
 *       Будет лучше создать метод WatcherBot::run для запуска бота.
 */
class WatcherBot
{
public:
    /**
     * @brief Конструктор
     * @param token - Токен для авторизации в телеграм
     * @param chat_list - Список id-чатов (пользователей), которые могут взаимодействовать с ботом
     * @param cameras - Список камер (см. HttpCamera), которые будут использоваться ботом
     */
    WatcherBot( const std::string&           token,
                const std::list<uint64_t>&   chat_list,
                const std::list<HttpCamera>& cameras
              );
    /**
     * @brief Конструктор с дополнительным аргументом для установления соединения через прокси
     * @param token - Токен для авторизации в телеграм
     * @param chat_list - Список id-чатов (пользователей), которые могут взаимодействовать с ботом
     * @param cameras - Список камер (см. HttpCamera), которые будут использоваться ботом
     * @param proxy - Адрес прокси сервера
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
    /// Набор возможных состояний бота
    typedef enum {
        sleep, ///< При наличии активности в камерах бот будет молчать
        run    ///< Бот работает в штатном режиме
    } BotState;

    BotState                    _botState;
    std::shared_ptr<TgBot::Bot> _bot;
    TgBot::CurlHttpClient       _curl;
    std::list<uint64_t>         _chat_list;
    std::list<ImgDiffFinder*>   _diff_finders;
    std::list<HttpCamera>       _cameras;

    /**
     * @brief Внутренняя функция для выполнения инициализации бота
     * @internal
     */
    void init                (                                );
    /**
     * @brief Отправить пользователю diff-изображение
     * @details Будет вызван при наличии активности в камерах
     * @param imgDiffPath - Путь к изображению, который содержит
     */
    void sendImageDiffToChat ( const std::string& imgDiffPath );
    /**
     * @brief Команда start для возобновления процесса мониторинга камер
     * @param id - Id чата (пользователя)
     */
    void doCmdStart          ( const int64_t id               );
    /**
     * @brief Команда stop для остановки процесса мониторинга камер
     * @param id - Id чата (пользователя)
     */
    void doCmdStop           ( const int64_t id               );
    /**
     * @brief Метод для выполнения команды '/help'
     * @details Выдает список доступных команд с описание
     * @param id - Id чата (пользователя)
     */
    void doCmdHelp           ( const int64_t id               );
    /**
     * @brief Метод для выполнения команды '/id'
     * @details Выдает id чата или пользователя
     * @param id - Id чата (пользователя)
     */
    void doCmdId             ( const int64_t id               );
    /**
     * @brief Метод для выполнения команды '/camlist'
     * @details Выдает список камер
     * @param id - Id чата (пользователя)
     */
    void doCmdCamList        ( const int64_t id               );
    /**
     * @brief Метод для выполнения команды '/photo'
     * @details В ответ отправляет фото с камер
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
