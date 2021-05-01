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

    const int layerCount = 4; // input, 2 hidden, output

    for (int i = 0; i < layerCount; i++)
    {
        std::shared_ptr<NeuronLayer> layer = std::make_shared<NeuronLayer>();
        size_t neuronCount = 0;
        if (i == 0)
        {
            // Input layer
            neuronCount = inputImagePixelCount;
        }
        else if (i > 0 && i < layerCount - 1)
        {
            // Hidden layers
            neuronCount = 16;
        }
        else
        {
            // Output layer
            neuronCount = 10; // 0 - 9
        }

        for (int j = 0; j < neuronCount; j++)
        {
            std::shared_ptr<Neuron> neuron = std::make_shared<Neuron>();

            // Initialize each neuron with random values
            const double activation = (std::rand() % 100) / 100.0;
            const double bias = (std::rand() % 100) / 100.0;
            neuron->setActivation(activation);
            neuron->setBias(bias);

            layer->appendNeuron(neuron);
        }

        network.appendNeuronLayer(layer);
    }

    network.connectLayers();

    std::shared_ptr<NeuronLayer> inputLayer = network.getNeuronLayer(0);

    std::cout << std::setprecision(2);

    int y = 0;
    for (const auto & pixelRow : pixelMatrix)
    {
        int x = 0;
        for (const auto & pixel : pixelRow)
        {
            unsigned int neuronIndex = static_cast<unsigned int>(inputImageWidth * y + x);
            std::shared_ptr<Neuron> neuron = inputLayer->getNeuron(neuronIndex);

            const double activation = (pixel.red + pixel.green + pixel.blue) / 3.0 / 255.0; // Gray [0, 1]
            neuron->setActivation(activation);

            if (activation != 1.0)
            {
                // Don't print white colors
                std::cout << activation;
            }
            else
            {
                std::cout << " ";
            }

            std::cout << "\t";
            x++;
        }
        
        std::cout << std::endl;
        y++;
    }

    return 0;
}