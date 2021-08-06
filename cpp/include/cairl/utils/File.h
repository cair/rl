//
// Created by per on 8/3/21.
//

#ifndef CLASSIC_CONTROL_FILE_H
#define CLASSIC_CONTROL_FILE_H
#include <curl/curl.h>
#include <filesystem>
#include <math.h>

using std::filesystem::path;

namespace cairl::utils{


    class FileUtil{

    private:
        /// Progress bar for libcurl
        /// \param bar
        /// \param t
        /// \param d
        /// \return



    static int progress_func(void* ptr, double TotalToDownload, double NowDownloaded,
                      double TotalToUpload, double NowUploaded){
            if(TotalToDownload <= 0.0){
                return 0;
            }
            // how wide you want the progress meter to be
            int totaldotz=40;
            double fractiondownloaded = NowDownloaded / TotalToDownload;
            // part of the progressmeter that's already "full"
            int dotz = (int) round(fractiondownloaded * totaldotz);

            // create the "meter"
            int ii=0;
            printf("%3.0f%% [",fractiondownloaded*100);
            // part  that's full already
            for ( ; ii < dotz;ii++) {
                printf("=");
            }
            // remaining part (spaces)
            for ( ; ii < totaldotz;ii++) {
                printf(" ");
            }
            // and back to line begin - do not forget the fflush to avoid output buffering problems!
            printf("]\r");
            fflush(stdout);
            // if you don't return 0, the transfer will be aborted - see the documentation
            return 0;

        }


    public:
        static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
            size_t written = fwrite(ptr, size, nmemb, stream);
            return written;
        }


        static path download(const std::string& url, const std::string& destination){
            auto filename = path(url).filename();
            auto fullDestination = (path(destination) / filename);
            if(std::filesystem::exists(fullDestination) && filesize(fullDestination) > 0){
                return fullDestination;
            }

                CURL *curl;
            FILE *fp;
            CURLcode res;

            curl = curl_easy_init();
            if (curl) {
                fp = fopen(fullDestination.c_str(),"wb");
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
                curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
                res = curl_easy_perform(curl);
                /* always cleanup */
                curl_easy_cleanup(curl);
                fclose(fp);
            }
            return fullDestination;
        }

        static long filesize(const std::string& filename){
            std::uintmax_t size = std::filesystem::file_size(filename);
            return (long)size;
        }

        static std::ifstream read_file(const std::string& filename){
            if(!std::filesystem::exists(filename)){
                LOG(LOG_INFO, "Could not find file!");
            }
            std::ifstream f(filename);
            f.seekg(0);
            return f;
        }

        static std::string get_filename(const std::string& fullPath){
            return path(fullPath).filename();
        }

        static std::string get_path(const std::string& fullPath){
            return path(fullPath).parent_path();
        }


    };

}

#endif //CLASSIC_CONTROL_FILE_H
