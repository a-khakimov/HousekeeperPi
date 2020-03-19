/** 
 *  @file   HttpCamera.h
 *  @brief  Get image from http-server 
 *  @author a-khakimov 
 ***********************************************/

#ifndef HttpCamera_H
#define HttpCamera_H

#include <memory>
#include <tuple>
#include <opencv2/core.hpp>
#include "httplib.h"


class HttpCamera
{
public:
    HttpCamera(const std::string& host, const uint64_t port, const std::string& path);
    virtual ~HttpCamera();

    std::tuple<cv::Mat, bool> get();
    std::string info() const;

private:
    std::shared_ptr<httplib::Client> _client;
    std::string _path;
    std::string _info;
};

#endif // HttpCamera_H
