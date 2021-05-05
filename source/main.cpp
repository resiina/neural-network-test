#include <iostream>
#include <iomanip>

#include "bitmap.h"

#include "NeuralNetwork.h"
#include "NeuronLayer.h"
#include "Neuron.h"

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // -----------------------------------------------------
    Bitmap inputImage;
    inputImage.open("data/1.bmp");

    PixelMatrix pixelMatrix = inputImage.toPixelMatrix();

    const size_t inputImageHeight = pixelMatrix.size();
    const size_t inputImageWidth = pixelMatrix.at(0).size();
    const size_t inputImagePixelCount = inputImageWidth * inputImageHeight;

    // -----------------------------------------------------

    NeuralNetwork network;

    // Input layer takes all image pixels as activations
    const size_t inputLayerNeuronCount = inputImagePixelCount;
    network.appendNeuronLayer(inputLayerNeuronCount);

    // Define some hidden layers
    const size_t hiddenLayerCount = 2;
    const size_t hiddenLayerNeuronCount = 16;
    for (size_t hiddenLayerIndex = 0; hiddenLayerIndex < hiddenLayerCount; hiddenLayerIndex++)
    {
        network.appendNeuronLayer(hiddenLayerNeuronCount);
    }
    
    // Network outputs 10 different possible results, an integer from 0 to 9
    const int outputLayerNeuronCount = 10;
    network.appendNeuronLayer(outputLayerNeuronCount);

    network.connectLayers();

    std::shared_ptr<NeuronLayer> inputLayer = network.getNeuronLayer(0);

    std::cout << std::setprecision(2);

    for (size_t y = 0; y < pixelMatrix.size(); y++)
    {
        const std::vector<Pixel> & pixelRow = pixelMatrix.at(y);
        for (size_t x = 0; x < pixelRow.size(); x++)
        {
            const Pixel & pixel = pixelRow.at(x);
            const size_t neuronIndex = inputImageWidth * y + x;
            std::shared_ptr<Neuron> neuron = inputLayer->getNeuron(neuronIndex);

            const double activation = (pixel.red + pixel.green + pixel.blue) / 3.0 / 255.0; // Gray [0, 1]
            neuron->setActivation(activation);

            if (activation == 1.0)
            {
                // Don't print full activation (white)
                std::cout << " ";
            }
            else
            {
                std::cout << activation;
            }

            std::cout << "\t";
        }
        
        std::cout << std::endl;
    }

    return 0;
}