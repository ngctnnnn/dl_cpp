import torch
import torch.nn as nn
from torchvision.models.resnet import ResNet18_Weights
def loadModel(model_name: str = 'resnet18'):
    """
    Function to load DNN model

    Args:
    model_name (str) -- Name of model desired to use (default: 'resnet18')
    
    Returns:
    All layers of the chosen DNN model (except for the fully connected layers -> to extract features and not to do classification)
    """
    model = torch.hub.load('pytorch/vision:v0.10.0', model_name, weights = ResNet18_Weights.IMAGENET1K_V1)
    modules = list(model.children())[:-1]
    model = nn.Sequential(*modules)
    for layer in model.parameters():
        layer.requires_grad = False

    example_input = torch.rand(1, 3, 224, 224) 
    script_module = torch.jit.trace(model, example_input)
    script_module.save('resnet18.pt')

if __name__ == "__main__":
    loadModel("resnet18")