#pragma once
#include "FeatureExtract.h"

#include <memory>

FeatureExtract::FeatureExtract() {
    torch::nn::Sequential __dnn_modlel = nullptr;
    std::string _model_name = "resnet18";
}

FeatureExtract::FeatureExtract(std::string pwd, std::string image_name, std::string model_name) { 
    this->__pwd = pwd;
    this->__image_name = image_name;
    this->_model_name = model_name;
}

void FeatureExtract::loadModel() {
    /*
    Function to load model
    */
    std::string model_link;

    if (this->_model_name == "resnet18") { 
        model_link = this->__pwd + "/deeplearning_pytorch/src/model/resnet18.pt";
        if (!std::filesystem::exists(model_link)) {
            std::cout << BOLD(FRED("[Error] Model save path is not found.")) << std::endl; 
            return;
        }
    }

    try {
        this->__dnn_model = torch::jit::load(model_link);
        std::cout << FGREEN("[Status] Load model completed!") << std::endl;
    }
    catch (const c10::Error& e) {
        std::cout << BOLD(FRED("[Error] Can not load model!\n"));
    }
}

void FeatureExtract::readInfo() {
    
    std::string info_path = "../../data/info.txt";
    
    const char* info_url = info_path.c_str();

    if (!std::filesystem::exists(info_url)) {
        std::cout << BOLD(FRED("[Error] Info file not found!\n"));
        return;
    }
    
    freopen(info_url, "r", stdin);
    
    long label;
    std::string path, type;
    std::cin >> path >> label >> type;

    while (std::cin >> path >> label >> type) {
        std::cout << path << " " << label << std::endl;
        this->__data.push_back(std::make_pair(path, label));
    }
}

torch::Tensor FeatureExtract::extractFeatures(std::string data_path) {

    readInfo();

    std::vector<torch::Tensor> image_features;
    const std::string datasetPath = this->__pwd + "/data/";
    auto dataset = CustomDataset(datasetPath, this->__data).map(torch::data::transforms::Stack<>());
    auto dataloader = torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(std::move(dataset), torch::data::DataLoaderOptions().batch_size(this->__batch_size).workers(this->__num_workers));

    for (torch::data::Example<> &batch : *dataloader) {
        torch::Tensor data = batch.data.to(this->__device);

        std::vector<torch::jit::IValue> input;
        input.push_back(data);
        torch::Tensor output = this->__dnn_model.forward(input).toTensor();
        
        for (int batch_idx = 0; batch_idx < output.sizes()[0]; ++batch_idx) {
            image_features.push_back(output[batch_idx].reshape(512));
        }
    }

    torch::Tensor image_features_tensor = torch::stack(image_features, 0);
    std::cout << FGREEN("[Status] Extract features completed!\n");
    return image_features_tensor;
}

void saveFeatures(const std::string save_path, torch::Tensor image_features) {
    auto bytes = torch::jit::pickle_save(image_features);
    std::ofstream fout(save_path, std::ios::out | std::ios::binary);
    fout.write(bytes.data(), bytes.size());
    fout.close();
}

torch::Tensor loadFeatures(const std::string save_path) { 
    if (!std::filesystem::exists(save_path)) {
        std::cout << BOLD(FRED("[Error] Link to image features is not found!")) << std::endl;
        assert(std::filesystem::exists(save_path));
    }
    torch::Tensor image_features;
    try {
        std::ifstream input(save_path, std::ios::binary);
        std::vector<char> f(
            (std::istreambuf_iterator<char>(input)),
            (std::istreambuf_iterator<char>()));

        input.close();
        image_features = torch::pickle_load(f).toTensor();
    }
    catch (const c10::Error& e) {
        std::cout << BOLD(FRED("[Error] Can not load image features!\n"));
    }
    return image_features;
}

