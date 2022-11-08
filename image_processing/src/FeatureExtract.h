#pragma once

#include <torch/torch.h>
#include <torch/script.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include "../../ColorManager.h"
#include "DataLoader.cpp"

void saveFeatures(std::string save_path, torch::Tensor video_features);
torch::Tensor loadFeatures(std::string save_path);

class FeatureExtract {
private:
    std::string __pwd;
    std::string __image_name;
    torch::jit::script::Module __dnn_model;

    int __image_size = 256;
    int __num_workers = 0;
    size_t __batch_size = 64;
    torch::DeviceType __device = torch::kCPU;

    std::vector<std::pair<std::string, long>> __data;

    void readInfo();

protected:
    std::string _model_name;
    at::Tensor data_transform(std::string file_name);
public:
    FeatureExtract();
    FeatureExtract(std::string pwd, std::string image_name, std::string model_name);

    void loadModel();
    torch::Tensor extractFeatures(std::string data_path);
};
