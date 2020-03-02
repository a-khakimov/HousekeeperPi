#include <QCoreApplication>
#include "httplib.h"
#include <fstream>
#include <iostream>

int main()
{
    httplib::Server server;

    server.Get("/hi", [](const httplib::Request& req, httplib::Response& res) {
        Q_UNUSED(req);
        res.set_content("Hello World!", "text/plain");
    });


    server.Get("/image.png", [](const httplib::Request& req, httplib::Response& res) {
        Q_UNUSED(req);
        std::ifstream f("/home/ainr/img.png", std::ios::binary);
        if(f.is_open()) {
            f.seekg(0, std::ios::end);
            size_t size = f.tellg();
            f.seekg(0, std::ios::beg);
            std::cout << "Image size: " << size << std::endl;
            char* data = new char[size];
            f.read(data, size);
            std::cout << ">>"<< std::endl;
            for (size_t i = 0; i < size; ++i) {
                std::cout << static_cast<int>(data[i]) << " ";
            }
            std::cout << std::endl << ">>"<< std::endl;
            f.close();
            res.set_content_provider(size, [data](uint64_t offset, uint64_t length, httplib::DataSink &sink) {
                sink.write(&data[offset], length);
            }, [data] {
                delete[] data;
            });
        }
    });

    server.Get("/webcamera.png", [](const httplib::Request& req, httplib::Response& res) {
        Q_UNUSED(req);

    });

    server.listen("localhost", 1234);
}
