#include "imgsource.h"

#include <opencv2/imgcodecs.hpp>
#include <iostream>

ImgSource::ImgSource()
{
    client = std::make_shared<httplib::Client>("localhost", 1234);
}

ImgSource::~ImgSource()
{

}

bool ImgSource::isAlive()
{
    auto res = client->Get("/test");
    if (res && res->status == 200) {
        std::cout << res->body << std::endl;
    }

    return (res && res->status == 200 && res->body == "I am ok!");
}

cv::Mat ImgSource::get()
{
    cv::Mat result;
    std::vector<char> inputImgArray;
    auto res = client->Get("/webcamera.png", [&](const char *data, uint64_t data_length) {
        inputImgArray.insert(inputImgArray.end(), data, data + data_length);
        return true;
    });

    if (res && res->status == 200) {
        result = cv::imdecode(cv::Mat(inputImgArray), 1);
    }
    return result;
}
