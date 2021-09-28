#include <iostream>
#include <iomanip>
#include <filesystem>
#include <limits>

#include "lodepng.h"

#include "NeuralNetwork.h"
#include "NeuronLayer.h"
#include "Neuron.h"

#include "TrainingDataCollection.h"

void printSectionTitle(const std::string & sectionTitle)
{
    using namespace std;

    static unsigned int sectionNumber = 1;
    const string finalSectionTitle = to_string(sectionNumber) + ". " + sectionTitle;

    const string horizontalLine = "  " + string(finalSectionTitle.length() + 4, '#');
    const string spacingRow = "  # " + string(finalSectionTitle.length(), ' ') + " #";
    const string sectionRow = "  # " + finalSectionTitle + " #";
    
    cout << string(70, '_') << endl << endl; // Section separator line

    cout << horizontalLine << endl;
    cout << spacingRow << endl;
    cout << sectionRow << endl;
    cout << spacingRow << endl;
    cout << horizontalLine << endl << endl;

    sectionNumber++;
}

int main()
{
    try
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // -----------------------------------------------------
        const unsigned int inputImageWidth = 28;
        const unsigned int inputImageHeight = 28;

        printSectionTitle("Construct the network");

        std::vector<NeuralNetwork> networks;

        for (int i = 0; i < 20; i++)
        {
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

            networks.push_back(network);
        }
        

        printSectionTitle("Collect training data");
        
        std::shared_ptr<TrainingDataCollection> trainingDataCollection = std::make_shared<TrainingDataCollection>();
        trainingDataCollection->collect("../../data/training/");

        printSectionTitle("Train the network");

        //network.train(trainingDataCollection);
        
        const size_t trainingExamplesSize = trainingDataCollection->getLabelsTrainingData()[0]->trainingExamples.size();

        for (int generation = 0; generation < 10000; generation++)
        {
            size_t bestNetworkIndex = 0;
            double bestNetworkCost = std::numeric_limits<double>::max();
            for (size_t networkIndex = 0; networkIndex < networks.size(); networkIndex++)
            {
                NeuralNetwork & network = networks.at(networkIndex);
                //printSectionTitle("Use network for recognition");

                for (int trainingExampleNumber = 0; trainingExampleNumber < 10; trainingExampleNumber++)
                {
                    // 1. Get a random image of 0
                    const size_t trainingExampleIndex = std::rand() % trainingExamplesSize;
                    const auto & trainingExampleActivations = trainingDataCollection->getLabelsTrainingData()[0]->trainingExamples[trainingExampleIndex]->inputLayerActivations;

                    network.getFirstNeuronLayer()->setActivations(trainingExampleActivations);
                
                    std::cout << std::setprecision(2);

                    // 2. Compute results
                    network.compute();

                    // 3. Compare output to goal
                    const auto & goalOutputActivations = trainingDataCollection->getLabelsTrainingData()[0]->trainingExamples[trainingExampleIndex]->goalOutputLayerActivations;

                    const double cost = NeuralNetwork::calculateCost(network.getLastNeuronLayer()->getActivations(), goalOutputActivations);

                    if (cost < bestNetworkCost)
                    {
                        bestNetworkCost = cost;
                        bestNetworkIndex = networkIndex;
                    }
                }
            }

            NeuralNetwork bestNetwork = networks.at(bestNetworkIndex);
            if (generation % 50 == 0)
            {
                // Show performance of current best network
                std::cout << "Best cost of generation " << generation << ": " << bestNetworkCost << std::endl;
                std::cout << "Output layer activations:" << std::endl;
                const auto & outputLayerNeurons = bestNetwork.getLastNeuronLayer()->getNeurons();
                for (size_t i = 0; i < outputLayerNeurons.size(); i++)
                {
                    const auto & outputNeuron = outputLayerNeurons.at(i);
                    std::cout << i << ": " << outputNeuron->getActivation() << std::endl;
                }
                std::cout << std::endl << std::endl;
            }

            // Copy best network everywhere
            for (size_t networkIndex = 0; networkIndex < networks.size(); networkIndex++)
            {
                NeuralNetwork & network = networks.at(networkIndex);
                
                network = bestNetwork;

                if (networkIndex != bestNetworkIndex)
                {
                    // Randomize values a bit
                    network.randomize(0.1);
                }
            }
        }
    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught." << std::endl;
        return -1;
    }

    return 0;
}