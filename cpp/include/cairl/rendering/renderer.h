//
// Created by per on 7/25/21.
//

#ifndef CLASSIC_CONTROL_RENDERER_H
#define CLASSIC_CONTROL_RENDERER_H
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cairl/utils/Timer.h>
#include "omp.h"

// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
namespace cairl::classic_control::renderer{

    class Drawable{
    public:
        int x;
        int y;
        int w;
        int h;
        double rot;
        cv::Mat object;
        cv::Mat object_rotated;
        cv::Point2f center;

        Drawable(int x_, int y_, int w_, int h_)
        : x(x_)
        , y(y_)
        , w(w_)
        , h(h_)
        , rot(0)
        , object(h_, w_, CV_8UC4, cv::Scalar(0, 0, 0, 0))
        , center((object.cols-1)/2.0, (object.rows-1)/2.0)

        {}


        cv::Mat& data(){
            if(rot != 0){
                return object_rotated;
            }
            return object;
        }
        [[nodiscard]] const cv::Mat& data()const{
            if(rot != 0){
                return object_rotated;
            }
            return object;
        }
    };

    enum Fill{
        FILLED=cv::FILLED,
        LINE_1=1,
        LINE_2=2,
        LINE_4=4
    };


    class Rectangle: public Drawable{

    public:

        Rectangle(int x_, int y_, int w_, int h_, Fill thickness, std::array<double, 3> color)
        : Drawable(x_, y_, w_, h_)
        {
            cv::rectangle(
                    object,
                    cv::Point(0, 0),
                    cv::Point(w, h),
                    cv::Scalar(color[2] * 255.0, color[1] * 255.0, color[0] * 255.0, 255.0),
                    thickness,
                    cv::LINE_4);
        }

        void rotate(double rad){
            auto newAngle = rad * (180.0/CV_PI);
            auto angle = newAngle;
            rot = angle;

            // get rotation matrix for rotating the image around its center in pixel coordinates
            cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);


            // determine bounding rectangle, center not relevant
            cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), object.size(), angle).boundingRect2f();

            // adjust transformation matrix
            rot.at<double>(0,2) += bbox.width/2.0 - object.cols/2.0;
            rot.at<double>(1,2) += bbox.height/2.0 - object.rows/2.0;

            cv::warpAffine(object, object_rotated, rot, bbox.size(), cv::INTER_NEAREST, cv::BORDER_CONSTANT, cv::Scalar(
                255, 255, 255, 0)
            );


        }

    };

    class Circle: public Drawable{


    public:
        Circle(int x_, int y_, int r_, Fill thickness, std::array<double, 3> color)
        : Drawable(x_, y_, r_ * 2, r_ * 2){

            cv::circle(
                    object,
                    cv::Point(w / 2.0, h / 2.0),
                    r_,
                    cv::Scalar(color[2] * 255.0, color[1] * 255.0, color[0] * 255.0, 255.0),
                    thickness
            );

        }

        void set_position(float x, float y){

        }

        void move(float dx, float dy){

        }
    };

    class Renderer {
        int w;
        int h;
    public:

        [[maybe_unused]] Renderer(int screen_width, int screen_height)
        : w(screen_width)
        , h(screen_height)
        , canvas(h, w, CV_8UC4, cv::Scalar(0, 0, 0, 255))
        {
            clear();

        }

        cv::Mat canvas;
    public:


        static void overlayImage2(cv::Mat& src, const cv::Mat& overlay){
            std::vector<cv::Mat> roi_split;
            cv::split(src, roi_split);

            std::vector<cv::Mat> overlay_split;
            cv::split(overlay, overlay_split);

            cv::Mat opacity, opacity_sub;
            opacity = overlay_split[3] / 255;
            opacity_sub = 1 - roi_split[3];


            roi_split[0] = roi_split[0].mul(opacity_sub) + overlay_split[0].mul(opacity);
            roi_split[1] = roi_split[1].mul(opacity_sub) + overlay_split[1].mul(opacity);
            roi_split[2] = roi_split[2].mul(opacity_sub) + overlay_split[2].mul(opacity);
            roi_split[3] = roi_split[3].mul(opacity_sub) + overlay_split[3].mul(opacity);
        }

        static void overlayImage(cv::Mat& src, const cv::Mat& overlay)
        {
            for (int y = 0; y < src.rows; ++y){
                for (int x = 0; x < src.cols; ++x)
                {
                    const double opacity = (static_cast<double>(overlay.data[y * overlay.step + x * overlay.channels() + 3])) / 255;


                    for (int c = 0; c < src.channels(); ++c)
                    {
                        if(opacity <= 0) continue;

                        const unsigned char overlayPx = overlay.data[y * overlay.step + x * overlay.channels() + c];
                        const unsigned char srcPx = src.data[y * src.step + x * src.channels() + c];
                        src.data[y * src.step + src.channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
                    }
                }
            }
        }

#define BOUNDS_CHECK(axis, dimprop, dim, dim2, fnName) \
    int axis = other.axis; \
    int dimprop = overlay.dim2; \
    if (other.axis < 0) { \
    if (other.axis + o_dim_w <= 0) { return; } \
    overlay = overlay.fnName(-other.axis, dimprop); \
        axis += -other.axis; \
    } \
    else if (other.axis + dimprop >= canvas.dim2) { \
        if (other.axis >= canvas.dim2) { return; } \
        overlay = overlay.fnName(0, canvas.dim2 - other.axis ); \
    } \


        void blit(const Drawable& other) {
            auto overlay = other.data();



            BOUNDS_CHECK(x, o_dim_w, w, cols, colRange);
            BOUNDS_CHECK(y, o_dim_h, h, rows, rowRange);

            auto roi = canvas
                .colRange(x, x + overlay.cols)
                .rowRange(y, y + overlay.rows);

            overlayImage(roi, overlay);

        }

        void clear(){
            //canvas.

            canvas.setTo(cv::Scalar(255, 255,255)); // background
        }

    };

}


#endif //CLASSIC_CONTROL_RENDERER_H
