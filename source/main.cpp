#include <iostream>

#include "bitmap.h"

#include "NeuralNetwork.h"
#include "NeuronLayer.h"
#include "Neuron.h"

int main()
{
    Bitmap testBitmap;
    testBitmap.open("data/testBitmap.bmp");

    PixelMatrix pixelRows = testBitmap.toPixelMatrix();

    int pixelCount = 0;

    for (const auto & pixelRow : pixelRows)
    {
        for (const auto & pixel : pixelRow)
        {
            pixelCount = pixelRow.size() * pixelRows.size();

            //std::cout << std::to_string(pixel.blue) << std::endl;
        }
    }

    NeuralNetwork network;

    const int layerCount = 4; // input, 2 hidden, output

    for (int i = 0; i < layerCount; i++)
    {
        NeuronLayer* layer = new NeuronLayer;
        int neuronCount = 0;
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
            Neuron* neuron = new Neuron;

            // Initialize each neuron with random values
            const double activation = (rand() % 100) / 100.0;
            const double bias = (rand() % 100) / 100.0;
            neuron->setActivation(activation);
            neuron->setBias(bias);

            // TODO: random connection weights

            layer->appendNeuron(neuron);
        }

        network.appendNeuronLayer(layer);
    }

    network.connectLayers();

    NeuronLayer* inputLayer = network.getNeuronLayer(0);
    
    /*
    for (int y = 0; y < image.height(); y++)
    {
        for (int x = 0; x < image.width(); x++)
        {
            QRgb pixelColor = image.pixel(x, y);
            const int r = qRed(pixelColor);
            const int g = qGreen(pixelColor);
            const int b = qBlue(pixelColor);
            const double activation = qGray(r, g, b) / 255.0;

            unsigned int neuronIndex = static_cast<unsigned int>(image.width() * y + x);
            Neuron* neuron = inputLayer->getNeuron(neuronIndex);
            neuron->setActivation(activation);

            std::cout << neuron->getActivation() << "\t";
        }
        std::cout << std::endl;
    }
    */










    return 0;
}