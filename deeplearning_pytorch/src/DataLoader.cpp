#include <opencv2/opencv.hpp>
#include <torch/torch.h>

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../../ColorManager.h"

using Data = std::vector<std::pair<std::string, long>>;
using Example = torch::data::Example<>;

class CustomDataset : public torch::data::datasets::Dataset<CustomDataset> {
private:
    std::string __datasetPath;

    int __image_size = 256;
    size_t __batch_size = 64;
    torch::DeviceType __device = torch::kCPU;

    Data data;

 public:
    CustomDataset(std::string __datasetPath, const Data& data) : __datasetPath(__datasetPath), data(data) {}

    Example get(size_t index) {
        std::string path = data[index].first;
        cv::Mat mat = cv::imread(path);
        if (mat.empty()) {
            std::cout << BOLD(FRED("Can not read image!")) << std::endl;
            assert(!mat.empty());
        }

        cv::resize(mat, mat, cv::Size(this->__image_size, this->__image_size));
        std::vector<cv::Mat> channels(3);
        cv::split(mat, channels);

        torch::Tensor R = torch::from_blob(
            channels[2].ptr(),
            {this->__image_size, this->__image_size},
            torch::kUInt8);
        torch::Tensor G = torch::from_blob(
            channels[1].ptr(),
            {this->__image_size, this->__image_size},
            torch::kUInt8);
        torch::Tensor B = torch::from_blob(
            channels[0].ptr(),
            {this->__image_size, this->__image_size},
            torch::kUInt8);

        torch::Tensor tdata = torch::cat({R, G, B})
                        .view({3, this->__image_size, this->__image_size})
                        .to(torch::kFloat);
        torch::Tensor tlabel = torch::from_blob(&data[index].second, {1}, torch::kLong);
        return {tdata, tlabel};
    }

    torch::optional<size_t> size() const {
        return data.size();
    }
};
