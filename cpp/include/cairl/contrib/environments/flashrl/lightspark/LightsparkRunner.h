#pragma  once

/// Lightspark
#include <backends/security.h>
#include <swf.h>
#include <logger.h>
#include <platforms/engineutils.h>
#include <compat.h>
#include <parsing/streams.h>
#include <backends/config.h>
#include <backends/rendering.h>


#include <scripting/abc.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <filesystem>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <GL/glew.h>
#include <cairl/contrib/environments/flashrl/FlashEngineRunner.h>
#include "cairl/utils/File.h"
#include "CaiRLEngineData.h"
#include <thread>


using namespace lightspark;
using cairl::utils::FileUtil;



namespace cairl::envs::flashrl {

class LightsparkRunner: public FlashEngineRunner {

    bool useInterpreter = false;
    bool useFastInterpreter = true;
    bool useJit = true;
    bool ignoreUnhandledExceptions = false;
    int fpsOverride;
    bool isReset = true;
    std::ifstream dataStream;
    const std::filesystem::path url;
    std::filesystem::path downloadPath;
    std::filesystem::path filePath;
    std::string gameName;
    std::filesystem::path gameBasePath;
    std::filesystem::path urlOriginOverride;
    long gameFilesize;
public:
    static bool initSDL;
    LightsparkRunner(const std::filesystem::path& url_, int fpsOverride_ = 0)
    : fpsOverride(fpsOverride_)
    , url(url_){

        /// Download SWF if missing!
        SPDLOG_DEBUG("Downloading file....");
        downloadPath = std::filesystem::current_path() / "swf";
        std::filesystem::create_directory(downloadPath);
        filePath = FileUtil::download(url, downloadPath);
        SPDLOG_DEBUG("Downloading file.... COMPLETE");

        /// Get game metadata
        gameName = FileUtil::get_filename(filePath);
        gameBasePath = FileUtil::get_path(filePath);
        gameFilesize = FileUtil::filesize(filePath);
        urlOriginOverride = url.parent_path();
        }

        void stop(){
            if(isReset){
                SPDLOG_ERROR("Cannot stop a reset environment");
                return;
            }
            sys->destroy();
            bool isonerror = sys->exitOnError==SystemState::ERROR_ANY && sys->isOnError();
            SDL_Event event;
            SDL_zero(event);
            event.type = LS_USEREVENT_QUIT;
            SDL_PushEvent(&event);
            SDL_WaitThread(EngineData::mainLoopThread,nullptr);

            //pt.reset();
            //sys.reset();
            //SystemState::staticDeinit();
        }

        void restart(){
            auto absolutePath = std::filesystem::absolute(gameName).parent_path() / "data";
            //dataStream = FileUtil::read_file(filePath);

            /*sys = std::make_shared<SystemState>(gameFilesize, SystemState::FLASH);
            pt = std::make_shared<ParseThread>(dataStream, sys->mainClip);
            ed = std::make_shared<CaiRLEngineData>(sys.get(), absolutePath.string(), fpsOverride);
            sys->addJob(pt.get());*/
        }

        void start(){
        // TODO eventually we want to do this!
        // todo at that point we also want to disable SDL_WINDOW_HIDDEN in EngineData;
        //putenv("SDL_VIDEODRIVER=offscreen");

        if(sys != nullptr){
            sys->setShutdownFlag();
            sys->destroy();

            delete pt;
            delete sys;
            //sys = nullptr;
            //ed = nullptr;
            //pt = nullptr;
        }

        dataStream = FileUtil::read_file(filePath);


        /// Setup SystemState
        setlocale(LC_ALL, "");
        SecurityManager::SANDBOXTYPE sandboxType=SecurityManager::LOCAL_TRUSTED;
        SystemState::ERROR_TYPE exitOnError=SystemState::ERROR_PARSING;
        SystemState::FLASH_MODE flashMode=SystemState::FLASH;

        /// Logging
        Log::setLogLevel(LOG_INFO);
        bindtextdomain("lightspark", "/usr/share/locale");  // TODO hardcoded for linux
        textdomain("lightspark");


        /// Startup Lightspark engine
        SystemState::staticInit();

        if(!LightsparkRunner::initSDL)
        {
            if (!EngineData::startSDLMain()){
                SPDLOG_INFO("Failed to load SDL. aborting");
                SystemState::staticDeinit();
                return;
            }
            LightsparkRunner::initSDL = true;
        }


        //
        sys = new SystemState(gameFilesize, flashMode);
        pt = new ParseThread(dataStream, sys->mainClip);
        setTLSSys(sys);


        sys->setDownloadedPath(url.string());

        sys->allowFullscreen = true;
        sys->allowFullscreenInteractive = true;
        sys->mainClip->setOrigin(urlOriginOverride.string(), gameName);


        sys->useInterpreter = useInterpreter;
        sys->useFastInterpreter = useFastInterpreter;
        sys->useJit = useJit;
        sys->ignoreUnhandledExceptions = ignoreUnhandledExceptions;
        sys->exitOnError = exitOnError;
        // TODO SUPPORT PARAMETER FILE
        //if(paramsFileName)
        //sys->parseParametersFromFile(paramsFileName);


        // create path for shared object local storage
        auto absolutePath = std::filesystem::absolute(gameName).parent_path() / "data";
        if(!std::filesystem::exists(absolutePath)){
            std::filesystem::create_directory(absolutePath);
        }
        ed = new CaiRLEngineData(sys, absolutePath.string(), fpsOverride);
        sys->setParamsAndEngine(ed, true);
        sys->securityManager->setSandboxType(sandboxType);
        sys->downloadManager = new StandaloneDownloadManager();

        sys->addJob(pt);
    }

        void setRenderCallback(const CaiRLEngineData::RenderCallback& rC){
            ed->setRenderCallback(rC);
        }

        void pause(){
            //if(!ed){
            //    return;
           // }
            //ed->fpsOverride = sys->mainClip->getFrameRate();
            sys->mainClip->setFrameRate(0.0001); // this will eventually round to 0

        }

        void unpause(){
            sys->mainClip->setFrameRate(ed->fpsOverride);
        }

    SystemState* sys;
    ParseThread* pt;
    CaiRLEngineData* ed;
};
bool LightsparkRunner::initSDL = false;


}

