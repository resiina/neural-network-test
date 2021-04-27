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
    Bitmap testBitmap;
    testBitmap.open("data/1.bmp");

    PixelMatrix pixelMatrix = testBitmap.toPixelMatrix();

    size_t height = pixelMatrix.size();
    size_t width = pixelMatrix.at(0).size();

    size_t pixelCount = width * height;

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
            neuronCount = pixelCount;
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

            // TODO: random connection weights

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
            const int r = pixel.red;
            const int g = pixel.green;
            const int b = pixel.blue;
            const double activation =  (r + g + b) / 3.0 / 255.0; // [0, 1]

            unsigned int neuronIndex = static_cast<unsigned int>(width * y + x);
            std::shared_ptr<Neuron> neuron = inputLayer->getNeuron(neuronIndex);
            neuron->setActivation(activation);

            if (activation != 1.0)
            {
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