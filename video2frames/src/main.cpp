#include <iostream>
#include <filesystem>
#include <fstream>
#include <any>
#include <unistd.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "../ColorManager.h"

namespace fs = std::filesystem;

inline void getType(std::any x) {
    std::cout << typeid(x).name() << std::endl;
}

inline double getFps(cv::VideoCapture cap) {
    return cap.get(cv::CAP_PROP_FPS);
}

void getFramesFromVideo(std::string video_url, std::string output_url, cv::VideoCapture &cap) {
    int frame_counter = 0;

    for(;;) {
        cv::Mat frame;
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty()) {
            if (frame_counter == 0) {
                std::cout << BOLD(FRED("[Error] No frame is found!")) << std::endl;
                return;
            }
            std::cout << BOLD(FGREEN("[Status] Process video completed!")) << std::endl;
            break;
        }

        // Capture frame-by-frame
        ++frame_counter;
        cap.set(cv::CAP_PROP_POS_MSEC, frame_counter * 1000.0);
        cv::imwrite(output_url + "Frame_" + std::to_string(frame_counter) + ".png", frame);
        info << output_url + "Frame_" + std::to_string(frame_counter) + ".png" << " " << 0 << " " << "meow" << std::endl;
    } 
    std::cout << BOLD(FGREEN("[Status] Extracted ")) << frame_counter << BOLD(FGREEN(" frames in total.")) << std::endl;
}

int main (int argc, char *argv[]) {  

    auto start = std::chrono::steady_clock::now();
 
    /*
    Config constant url
    */
    std::string PWD = argv[3];
    std::string MASTER_FOLDER = argv[4];
    std::string output_url = PWD + "/frames-db/";
    std::string video_url = MASTER_FOLDER + "/data/";

    /*
    Config video name
    */
    video_url += argv[1];

    std::cout << BOLD(FCYAN("[Status] Processing ")) << video_url << std::endl;
    /*
    Config out frames folder
    */
    std::string FRAME_URL = "/frames/";
    if (!fs::is_directory(output_url) || !fs::exists(output_url)) {
        fs::create_directory(output_url);
    } // Create directory `frames-db`
    
    output_url += argv[2];
    // If there is no output folder for frames -> Create 
    if (!fs::is_directory(output_url) || !fs::exists(output_url)) {
        fs::create_directory(output_url);
    } // Create directory `frames-db/frames/`
    
    output_url += FRAME_URL;
    if (!fs::is_directory(output_url) || !fs::exists(output_url)) {
        fs::create_directory(output_url);
    } // Create directory `frames-db/frames/video_name`

    cv::VideoCapture cap(video_url); 

    if (!std::filesystem::exists(video_url)) {
        throw std::runtime_error("Video file not found!");
    }

    if(!cap.isOpened()){
        throw std::runtime_error("Error opening video stream or file!");
    }

    getFramesFromVideo(video_url, output_url, cap);

    cap.release();
    auto end = std::chrono::steady_clock::now();
    std::cout << FCYAN("[Status] Capture frames from video in ") << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << FCYAN(" seconds.") << std::endl;
    
    return 0;
}
