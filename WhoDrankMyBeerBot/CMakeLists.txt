cmake_minimum_required(VERSION 3.1)

project(WhoDrankMyBeer
	DESCRIPTION ""
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(WhoDrankMyBeer
	main.cpp
	HttpCamera.cpp
	ImgDiffFinder.cpp
	WatcherBot.cpp
)

find_library(SSL_LIB NAMES ssl)
message(STATUS ${SSL_LIB})

find_library(crypto_LIB NAMES crypto)
message(STATUS ${crypto_LIB})

find_library(CURL_LIB NAMES curl)
message(STATUS ${CURL_LIB})

find_library(TGBOT_LIB NAMES TgBot)
message(STATUS ${TGBOT_LIB})

find_library(BOOST_SYSTEM_LIB NAMES boost_system)
message(STATUS ${BOOST_SYSTEM_LIB})

find_library(OPENCV_CORE_LIB NAMES opencv_core)
message(STATUS ${OPENCV_CORE_LIB})

find_library(opencv_quality_LIB NAMES opencv_quality)
message(STATUS ${opencv_quality_LIB})

find_library(opencv_imgproc_LIB NAMES opencv_imgproc)
message(STATUS ${opencv_imgproc_LIB})

find_library(opencv_imgcodecs_LIB NAMES opencv_imgcodecs)
message(STATUS ${opencv_imgcodecs_LIB})

target_link_libraries(WhoDrankMyBeer
	opencv_core
	opencv_quality
	opencv_imgproc
	opencv_imgcodecs
	boost_system
	TgBot
	curl
	pthread
	crypto
	ssl
)

include_directories("/usr/include"
                    "/usr/local/include"
                    "/usr/local/include/opencv4"
                    "../plog/include"
                    "../timercpp"
                    "../cpp-httplib"
                    "../cxxopts/include"
                    ${INCLUDE_PATH})

add_definitions(-DHAVE_CURL)
