//
// Created by per on 7/25/21.
//

#ifndef CLASSIC_CONTROL_RENDERER_H
#define CLASSIC_CONTROL_RENDERER_H
#include <opencv2/imgproc.hpp>

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

            cv::warpAffine(object, object_rotated, rot, bbox.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(
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

        static void overlayImage(cv::Mat& src, cv::Mat& overlay, const cv::Point& location)
        {
            // TODO. this can most definately be improved.
            for (int y = std::max(location.y, 0); y < src.rows; ++y)
            {
                const int fY = y - location.y;

                if (fY >= overlay.rows)
                    break;

                for (int x = std::max(location.x, 0); x < src.cols; ++x)
                {
                    const int fX = x - location.x;

                    if (fX >= overlay.cols)
                        break;

                    const double opacity = ((double)overlay.data[fY * overlay.step + fX * overlay.channels() + 3]) / 255;

                    for (int c = 0; opacity > 0 && c < src.channels(); ++c)
                    {
                        const unsigned char overlayPx = overlay.data[fY * overlay.step + fX * overlay.channels() + c];
                        const unsigned char srcPx = src.data[y * src.step + x * src.channels() + c];
                        src.data[y * src.step + src.channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
                    }
                }
            }
        }


        void blit(Drawable other) {
            overlayImage(canvas, other.data(), {other.x, other.y});
        }

        void clear(){
            //canvas.

            canvas.setTo(cv::Scalar(255, 255,255)); // background
        }

    };

}


#endif //CLASSIC_CONTROL_RENDERER_H
