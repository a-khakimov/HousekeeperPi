# HousekeeperBot

## Описание

TODO

### Цель

TODO

### Конфигурирование

Для конфигурации используется json-файл

```json
{
	"token" : "TOKEN",
	"proxy_host" : "socks5://localhost:9050",
	"chat_id" : 123456789
}
``` 
* *token* - для авторизации в telegram
* *chat_id* - id-пользователя, которого будет уведомлять бот
* *proxy_host* - адрес прокси

### Сборка

Для сборки используется [qbs](https://doc.qt.io/qbs/index.html).

```bash
cd HousekeeperBot
qbs build
```

### Запуск бота

TODO

## Стек используемых технологий

* [opencv](https://github.com/opencv/opencv) - используется для обработки, сравнения изображений
* [tgbot-cpp](https://github.com/reo7sp/tgbot-cpp) - библиотека для создания телеграм чат-ботов
* [plog](https://github.com/SergiusTheBest/plog) - логгер
* [cpp-httplib](https://github.com/yhirose/cpp-httplib) - используется для получения изображений от http-сервера
* [cxxopts]() - используется для парсинга аргументов коммандной строки
