#include <iostream>
#include <iomanip>
#include <filesystem>

#include "lodepng.h"

#include "NeuralNetwork.h"
#include "NeuronLayer.h"
#include "Neuron.h"

#include "TrainingDataCollection.h"

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // -----------------------------------------------------
    const unsigned int inputImageWidth = 28;
    const unsigned int inputImageHeight = 28;

    // -----------------------------------------------------
    // Construct the network
    // -----------------------------------------------------
    NeuralNetwork network;

    // 1. Define input layer
    // Input layer takes all image pixels as activations
    const size_t inputLayerNeuronCount = inputImageWidth * inputImageHeight;
    network.appendNeuronLayer(inputLayerNeuronCount);

    // 2. Define some hidden layers
    const size_t hiddenLayerCount = 2;
    const size_t hiddenLayerNeuronCount = 16;
    for (size_t hiddenLayerIndex = 0; hiddenLayerIndex < hiddenLayerCount; hiddenLayerIndex++)
    {
        network.appendNeuronLayer(hiddenLayerNeuronCount);
    }
    
    // 3. Define output layer
    // Network outputs 10 different possible results, an integer from 0 to 9
    const int outputLayerNeuronCount = 10;
    network.appendNeuronLayer(outputLayerNeuronCount);

    network.connectLayers();

    // -----------------------------------------------------
    // Train the network
    // -----------------------------------------------------
    
    // Collect training data
    std::shared_ptr<TrainingDataCollection> trainingDataCollection = std::make_shared<TrainingDataCollection>();
    trainingDataCollection->collect("../../data/training/");

    // TODO

    //network.train(trainingDataCollection);

    // -----------------------------------------------------
    // Use network for recognition
    // -----------------------------------------------------
    std::cout << std::setprecision(2);

    // 1. Set input layer activations based on input image
    std::shared_ptr<NeuronLayer> inputLayer = network.getFirstNeuronLayer();

    const std::string testFilePath = "../../data/testing/0/3.png";
    std::vector<unsigned char> testImageData;
    unsigned int testImageWidth;
    unsigned int testImageHeight;
    const unsigned int pngDecodeResult = lodepng::decode(testImageData, testImageWidth, testImageHeight, testFilePath.c_str());
    if (pngDecodeResult != 0)
    {
        std::cout << "Loading PNG failed." << std::endl
                  << "Decoder error " << pngDecodeResult << ": " << lodepng_error_text(pngDecodeResult) << std::endl;
        return -1;
    }

    const size_t bytesPerPixel = 4;
    for (size_t i = 0; i < testImageData.size(); i += bytesPerPixel)
    {
        const unsigned char & red   = testImageData.at(i);
        const unsigned char & green = testImageData.at(i + 1);
        const unsigned char & blue  = testImageData.at(i + 2);
        //const unsigned char & alpha = imageData.at(i + 3);
        
        const size_t neuronIndex = i / bytesPerPixel;
        std::shared_ptr<Neuron> neuron = inputLayer->getNeuron(neuronIndex);

        const double activation = (red + green + blue) / 3.0 / 255.0; // Gray [0, 1]
        neuron->setActivation(activation);
    }

    // 2. Compute results
    network.compute();

    // 3. Show output
    std::cout << "Output layer activations:" << std::endl;
    const auto & outputLayerNeurons = network.getLastNeuronLayer()->getNeurons();
    for (size_t i = 0; i < outputLayerNeurons.size(); i++)
    {
        const auto & outputNeuron = outputLayerNeurons.at(i);
        std::cout << i << ": " << outputNeuron->getActivation() << std::endl;
    }

    return 0;
}