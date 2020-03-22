/** 
 *  @file   HttpCamera.h
 *  @brief  Класс предоставляет метод для получения изображений от http-сервера.
 *  @author a-khakimov 
 ***********************************************/

#ifndef HttpCamera_H
#define HttpCamera_H

#include <memory>
#include <tuple>
#include <opencv2/core.hpp>
#include "httplib.h"

/**
 * @brief Класс предоставляет метод для получения изображений от HTTP-сервера.
 * @details Для получения изображений выполняется GET-запрос.
 * @details Предполагается, что сервер отвечает на GET-запрос PNG изображением
 * */
class HttpCamera
{
public:
    /**
     * @brief HttpCamera
     * @param host - хост сервера который предоставляет изображения
     * @param port - порт сервера
     * @param path - путь по которому сервер предоставляет изображения
     */
    HttpCamera( const std::string& host,
                const uint64_t port,
                const std::string& path
               );
    /**
     * @brief Деструктор
     */
    virtual ~HttpCamera();

    /**
     * @brief get - метод для выполнения запроса к серверу
     * @return cv::Mat - Возвращаемое изображение
     * @return bool isOk - Статус выполнения запроса
     * @details false - при выполнении запроса произошла ошибка (например, сервер недоступен),
     *          true - запрос выполнен успешно
     * @note В первую очередь необходимо проверить значение возвращаемой переменной isOk
     * @public
     */
    std::tuple<cv::Mat, bool> get();
    /**
     * @brief info - метод возвращает информацию о данных, используемых для выполнения запроса
     * @return Строковое значение
     * @details Содержит хост, порт сервера и путь по которому можно получить изображение
     * @public
     */
    std::string info() const;

private:
    std::shared_ptr<httplib::Client> _client;
    std::string _path;
    std::string _info;
};

#endif // HttpCamera_H
