/** 
 *  @file   ImgDiffFinder.h
 *  @brief  Класс предназначенный для определения наличия разности изображений
 *  @author a-khakimov 
 ***********************************************/

#ifndef IMGDIFF_H
#define IMGDIFF_H

#include "HttpCamera.h"
#include "timercpp.h"
#include <functional>
#include <queue>
#include <memory>

/**
 * @brief Класс предназначенный для определения наличия разности изображений
 *  @details Класс выполняет постоянный мониторинг, выполняя запрос нового изображения раз в секунду
 *           сравнивая с предыдущим изображением. При наличии разности производится вызов ImgDiffFinder::onImgDiffFinded.
 */
class ImgDiffFinder
{
private:
    Timer               _timer;
    HttpCamera          _camera;
    std::queue<cv::Mat> _images_queue;

    /**
     * @brief ImgDiffHandler
     */
    typedef std::function <void ( const std::string&, const bool )> ImgDiffHandler;

    /**
     * @brief Метод возвращает числовое значение разности двух изображений
     * @param imgA - Первое изображение
     * @param imgB - Второе изображение
     * @return Mean Square Error — число характеризующее разность двух изображений
     */
    inline cv::Scalar makeMse       ( const cv::Mat&  imgA, const cv::Mat& imgB      );
    /**
     * @brief Метод возвращает diff изображение, являющееся разностью двух переданных изображений
     * @param imgA - Первое изображение
     * @param imgB - Второе изображение
     * @return diff изображение, являющееся разностью двух переданных изображений
     */
    inline cv::Mat makeDiffImg      ( const cv::Mat&, const cv::Mat&                 );
    /**
     * @brief Производит горизонтальное соединение трех изображений
     */
    inline cv::Mat makeConcatImg    ( const cv::Mat&, const cv::Mat&, const cv::Mat& );
    /**
     * @brief makeDiffInfo
     */
    inline std::string makeDiffInfo ( const std::string& camInfo, const cv::Scalar& mse );
    /**
     * @brief Возвращает строковое значение, содержащее информацию об источнике изображения (см. HttpCamera::info)
     *        и MSE
     */
    inline bool differenceIsLarge   ( const cv::Scalar&                              );

public:
    /**
     * @brief Конструктор класса ImgDiffFinder
     * @param cam - аргументом является объект класса HttpCamera, т.е. источник изображений
     */
    explicit ImgDiffFinder ( HttpCamera& cam                );
    /**
     * @brief Деструктор
     */
    virtual ~ImgDiffFinder (                                );
    /**
     * @brief Метод, который запускает процесс мониторинга
     * @details - При достижении определенного значения разности изображений будет вызван обработчик ImgDiffHandler
     * @param ms - частота с которой будет производится запрос к серверу изображений
     * @param handler Обработчик, который будет вызван при достижении определенного значения разности изображений
     */
    void onImgDiffFinded   ( int ms, ImgDiffHandler handler );
};

#endif // IMGDIFF_H
