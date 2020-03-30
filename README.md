<p align="left">
    <a href="https://hub.docker.com/r/ainr/who-drank-my-beer">
    	<img src="https://img.shields.io/docker/cloud/build/ainr/who-drank-my-beer" />
	</a>
</p>

# Who drank my beer

## Описание

Простой чат-бот, выполняющий список следующих функций:
* мониторинг активности в камерах
* уведомление пользователя
* выполнение команд пользователя

## Зависимости

* [plog](https://github.com/SergiusTheBest/plog) - логгирование
* [opencv](https://github.com/opencv/opencv) - используется для обработки и сравнения изображений
* [tgbot-cpp](https://github.com/reo7sp/tgbot-cpp) - библиотека для создания чат-ботов в телеграм
* [cpp-httplib](https://github.com/yhirose/cpp-httplib) - для выполнения http-запросов серверу изображений
* [cxxopts](https://github.com/jarro2783/cxxopts.git) - используется для парсинга аргументов коммандной строки
* [json](https://github.com/nlohmann/json.git) - для работы с json-файлами

### Сборка

Перед сборкой необходимо установить вышеперечисленные зависимости.
Склонировать проект и вытянуть модули

```bash
git clone https://github.com/a-khakimov/WhoDrankMyBeer.git
cd WhoDrankMyBeer
git submodule update --recursive --init
```

Для сборки используется **cmake**:

```bash
cd WhoDrankMyBeerBot
mkdir build
cd build
cmake ..
make
```
### Конфигурирование

Для конфигурирования бота необходимо создать json-файл со следующей структурой:

```json
{
    "token" : "",
    "proxy_host" : "socks5://localhost:9050",
    "http_cameras": [
        { "host" : "192.168.0.2", "port" : 1234, "path" : "/PiCamera.png"     },
        { "host" : "192.168.0.5", "port" : 8890, "path" : "/path/to/img.png"  },
    ],
    "chats" : [
        123456789
    ]
}
``` 
* *token* - для авторизации бота в telegram
* *chats* - id-пользователей (чатов), которых будет уведомлять бот
* *proxy_host* - адрес прокси
* *http_cameras* - список камер

### Запуск бота

При запуске бота необходимо задать путь к конфигурационному файлу.

```bash
./WhoDrankMyBeer --configurations ./conf.json
```

После запуска бот начнет процесс мониторинга активности в камерах. При возникновении какой либо активности бот уведомит пользователя передав в чат изображение.

Так же бот поддерживает команды с помощью которых возможно управление ботом.

```
/help - help message
/start - start monitoring
/stop - stop monitoring
/id - send chat id
/list - list of cameras
/photo - take photo and send
```

Со врменем команды могут быть изменены. Поэтому список актуальных команд можно получить отправив команду `'/help'`.

### Документация

Документация формируется с помощью Doxygen. 

Ссылка к документации - [https://a-khakimov.github.io/who-drank-my-beer/doxygen/](https://a-khakimov.github.io/who-drank-my-beer/doxygen/)

### Известные проблемы

#### Непойманное исключение приводящее к падению бота

```
terminate called after throwing an instance of 'std::runtime_error'
  what():  curl error: Couldn't resolve host name
Аварийный останов (стек памяти сброшен на диск)
```
