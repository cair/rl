//
// Created by per on 8/4/21.
//

#ifndef CLASSIC_CONTROL_CAIRLENGINEDATA_H
#define CLASSIC_CONTROL_CAIRLENGINEDATA_H
#include <condition_variable>

class CaiRLTicker: public lightspark::ITickJob{

    void tick() override{
        if(tickfn){
            tickfn();
        }

    }

    void tickFence() override{

    }

public:
    std::function<void()> tickfn = nullptr;

    CaiRLTicker(){

    }
};

class CaiRLEngineData: public lightspark::EngineData
        {
        public:
            using RenderCallback = std::function<void(const cv::Mat& frame)>;
            std::mutex m;
            std::mutex m3;
            std::condition_variable cond;
            std::unique_lock<std::mutex> lk{m3};

    cv::Mat frame;
private:
        SDL_GLContext mSDLContext;
        lightspark::SystemState* sys;


    const RenderCallback* renderCallback = nullptr;

        public:

            float fpsOverride;
            CaiRLEngineData(lightspark::SystemState* sys_, const lightspark::tiny_string& datapath, float fpsOverride_)
            : sys(sys_)
            , fpsOverride(fpsOverride_){
                sharedObjectDatapath = datapath;



            }
            ~CaiRLEngineData(){
                SPDLOG_INFO("remove this directory: {}", lightspark::Config::getConfig()->getDataDirectory().c_str());
            }

            void setRenderCallback(const CaiRLEngineData::RenderCallback& rC){
                renderCallback = &rC;
            }


            SDL_Window* createWidget(uint32_t w, uint32_t h){
                auto window = SDL_CreateWindow(
                        "Lightspark",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        w,
                        h,
                        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN
                        );


                if (window == 0){
                    LOG(LOG_ERROR,"createWidget failed:" << SDL_GetError());
                }
                return window;
            }

            void renderContextMenu() override{}


            void DoSwapBuffers() override
            {
                uint32_t err;
                if (getGLError(err))
                    LOG(LOG_ERROR,"swapbuffers:"<<widget<<" "<<err);
                SDL_GL_SwapWindow(widget);

                if(fpsOverride > 0.0 && sys->getRenderRate() != fpsOverride){
                    sys->setRenderRate(static_cast<float>(fpsOverride));
                    sys->mainClip->setFrameRate(fpsOverride);
                }
                if(frame.empty()){
                    auto rT = lightspark::getRenderThread();
                    frame = cv::Mat(rT->windowHeight, rT->windowWidth, CV_8UC3);
                }

                {
                    std::scoped_lock g(m);

                    glPixelStorei(GL_PACK_ROW_LENGTH, frame.step/frame.elemSize());
                    glReadPixels(0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
                    cv::flip(frame, frame, 0);
                    cond.notify_all();
                }

            }
            void InitOpenGL() override
            {
                mSDLContext = SDL_GL_CreateContext(widget);
                if (!mSDLContext)
                    LOG(LOG_ERROR,"failed to create openGL context:"<<SDL_GetError());
                initGLEW();
            }
            void DeinitOpenGL() override
            {
                SDL_GL_DeleteContext(mSDLContext);
            }


            void exec_glFlush() override {

                EngineData::exec_glFlush();

            }



            uint32_t getWindowForGnash() override {
                // todo callback function here.
                return 0;
            }


            void stopMainDownload() override {}
            [[nodiscard]] bool isSizable() const override { return false; }
            void grabFocus() override { }
            void openPageInBrowser(const lightspark::tiny_string& url, const lightspark::tiny_string& window) override {}
            bool getScreenData(SDL_DisplayMode* screen) override { return true;}
            double getScreenDPI() override{ return 96.0;}
        };
#endif //CLASSIC_CONTROL_CAIRLENGINEDATA_H
