#include <chrono>
#include <filesystem>

#include "FeatureExtract.cpp"
#include "../../ColorManager.h"

namespace fs = std::filesystem;

const double duplicated_threshold = 0.85;

int main (int argc, char *argv[]) {
    // argv[1] <- image_name.extension
    // argv[2] <- image_name
    // argv[3] <- path to duplicated_detection

    auto start = std::chrono::steady_clock::now();
 
    const std::string PWD = argv[3];
    const std::string image_url = PWD + "/data";
    if (!fs::exists(image_url)) {
        std::cout << image_url << std::endl;
        std::cout << BOLD(FRED("[Error] Data folder does not exist!\n"));
        return -1;
    }
    const std::string feature_path = PWD + "/data/" + argv[2] + ".pt";

    FeatureExtract feature_extraction = FeatureExtract(PWD, argv[2], "resnet18");
    feature_extraction.loadModel();
    torch::Tensor image_features = feature_extraction.extractFeatures(image_url);

    try {
        saveFeatures(feature_path, image_features);
        std::cout << FCYAN("[Status] Save image features to ") << feature_path << FCYAN(" successfully!") << std::endl;
    }
    catch (const c10::Error& e) {
        std::cout << BOLD(FRED("[Error] Can not save image features!\n"));
    }
    
    auto end = std::chrono::steady_clock::now();
    std::cout << FCYAN("[Status] Process in: ") << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << FCYAN(" seconds.") << std::endl;
    
    return 0;
}
