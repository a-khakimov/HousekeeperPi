<p align="left">
    <a href="https://hub.docker.com/r/ainr/who-drank-my-beer">
    	<img src="https://img.shields.io/docker/cloud/build/ainr/who-drank-my-beer" />
	</a>
</p>

# Who drank my beer

## Описание

_TODO_

### Конфигурирование

Для конфигурирования используется json-файл

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
* *http_cameras* - список 

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

#### qbs

```bash
cd WhoDrankMyBeerBot
qbs build
```

#### cmake

```bash
cd WhoDrankMyBeerBot
mkdir build
cd build
cmake ..
make
```

### Запуск бота

Список доступных команд можно получить отправив команду `'/help'`
