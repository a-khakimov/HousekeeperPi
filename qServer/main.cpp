#include <fstream>
#include <iostream>
#include <memory>
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

    server.Get("/test", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("I am ok!", "text/plain");
    });


    server.Get("/webcamera.png", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "Request webcam image" << std::endl;
        WebCamCapture wcam;
        std::vector<uchar> img = wcam.takeImage();
        res.set_content_provider(img.size(), [img](uint64_t offset, uint64_t length, httplib::DataSink &sink) {
            const char* data = (char*)img.data();
            sink.write(&data[offset], length);
        });
    });

    std::cout << "Start server..." << std::endl;

    server.listen("localhost", 1234);
}
