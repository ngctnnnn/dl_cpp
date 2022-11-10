#include <torch/torch.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <any>

std::vector<std::any> tensor2vec(torch::Tensor tensor) {
    int vector_size = tensor.sizes().size();
    std::vector<std::any> vector_tensor;

    std::cout << "Tensor size: " << tensor.sizes() << std::endl;
    if (vector_size == 0) {
        std::cout << BOLD(FRED("[Error] Undefined dimension!\n"));
        assert(vector_size != 0);
    }
    switch (vector_size) {
        case 1:
            for (int i = 0; i < tensor.sizes()[0]; ++i) {
                vector_tensor.emplace_back(tensor[i].item<double>());
            }
            return vector_tensor;
        case 2: 
            for (int i = 0; i < tensor.sizes()[0]; ++i) {
                std::vector<double> vec_dim1;
                for (int j = 0; j < tensor.sizes()[1]; ++j) {
                    vec_dim1.push_back(tensor[i][j].item<double>());              
                }
                vector_tensor.emplace_back(vec_dim1);
            }
            return vector_tensor;
        default: 
            std::cout << BOLD(FRED("[Error] Transform torch tensor into vector for this dimension is not implemented!\n"));
            assert(vector_size > 0 || vector_size <= 2);
            break;
    }

    if (vector_tensor.size() == 0) {
        std::cout << "[Status] Transform torch::Tensor to std::vector unsuccessfully!\n";
        assert(vector_tensor.size() != 0);
    }
    return vector_tensor;
}

void getTensorVector(std::vector<std::any> vector_tensor) {
    if (vector_tensor.empty()) { 
        std::cout << FYELLOW("[Warning] Tensor vector has nothing to print.\n");
        assert(vector_tensor.empty() != true);
    }

    for (auto&& e : vector_tensor) {
        if (auto ptr = std::any_cast<std::vector<double>>(&e)) {
            for (auto it = ptr->begin(); it != ptr->end(); ++it) {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        } else if (auto ptr = std::any_cast<double>(&e)){
            std::cout << *ptr << " ";
        } else {
            std::cout << BOLD(FRED("[Error] Not implemented for this dimension.\n"));
        }
    }
    std::cout << std::endl;
}

int main (int argc, char **argv) { 
    torch::Tensor test = torch::rand({2, 3, 4});
    std::cout << "Original tensor:\n" << test << std::endl;

    if (!test.numel()) {
        std::cout << "Tensor is empty." << std::endl;
    } else {
        std::cout << "Tensor is not empty." << std::endl;
    }

    std::vector<std::any> vector_tensor = tensor2vec(test);
    getTensorVector(vector_tensor);
    
    return 0;
}
