#include <fstream>
#include <iostream>
#include "httplib.h"

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>

class WebCamCapture {
public:
    WebCamCapture () {
        int deviceID = 0;             // 0 = open default camera
        int apiID = cv::CAP_ANY;      // 0 = autodetect default API
        cap.open(deviceID + apiID);

        if (!cap.isOpened()) {
            std::cerr << "ERROR! Unable to open camera\n";
            // TODO: throw exception
        }
    }

    std::vector<uchar> takeImage()
    {
        cap.read(frame);

        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            // TODO: return error
        }

        std::vector<uchar> png_image;
        imencode(".png", frame, png_image);
        return png_image;
    }

    ~WebCamCapture() {

    }
private:
    cv::Mat frame;
    cv::VideoCapture cap;
};

int main()
{
    httplib::Server server;

    server.Get("/hi", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello World!", "text/plain");
    });

    server.Get("/image.png", [](const httplib::Request&, httplib::Response& res) {
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

    server.Get("/webcamera.png", [](const httplib::Request&, httplib::Response& res) {
        WebCamCapture wcam;
        std::vector<uchar> img = wcam.takeImage();
        res.set_content_provider(img.size(), [img](uint64_t offset, uint64_t length, httplib::DataSink &sink) {
            const char* data = (char*)img.data();
            sink.write(&data[offset], length);
        });
    });

    server.listen("localhost", 1234);
}
