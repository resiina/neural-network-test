#include <iostream>
#include <iomanip>
#include <filesystem>
#include <limits>
#include <ctime>
#include <thread>
#include <future>

#include "lodepng.h"

#include "NeuralNetwork.h"
#include "NeuronLayer.h"
#include "Neuron.h"

#include "DataCollection.h"

#include "CommandLineInterface.h"

int main()
{
    try
    {
        // Basic initializations
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        std::cout << std::setprecision(2);

        // Input layer takes all image pixels as activations
        // Network outputs 10 different possible results, an integer from 0 to 9
        const unsigned int inputImageWidth = 28;
        const unsigned int inputImageHeight = 28;

        const size_t inputLayerNeuronCount = inputImageWidth * inputImageHeight;
        const size_t outputLayerNeuronCount = 10;
        const size_t hiddenLayerCount = 2;
        const size_t hiddenLayerNeuronCount = 16;

        printSectionTitle("Construct the initial network population");

        const size_t networksPerGeneration = 20;

        std::vector<NeuralNetwork> networks;
        for (int networkIndex = 0; networkIndex < networksPerGeneration; networkIndex++)
        {
            NeuralNetwork network;
            network.initialize(inputLayerNeuronCount,
                               outputLayerNeuronCount,
                               hiddenLayerCount,
                               hiddenLayerNeuronCount);
            networks.push_back(network);
        }

        printSectionTitle("Collect training data");
        std::shared_ptr<DataCollection> trainingDataCollection = std::make_shared<DataCollection>("../../mnist_png/training/");

        printSectionTitle("Start neuroevolution");
        printTimeSinceStart();

        std::vector<std::future<double>> networkCostFutures(networks.size());

        for (int generation = 1; true; generation++)
        {
            for (size_t networkIndex = 0; networkIndex < networks.size(); networkIndex++)
            {
                NeuralNetwork & network = networks.at(networkIndex);
                networkCostFutures[networkIndex] = std::async(&NeuralNetwork::train, network, trainingDataCollection);
            }

            size_t bestNetworkIndex = 0;
            double bestNetworkCost = std::numeric_limits<double>::max();
            for (size_t networkIndex = 0; networkIndex < networkCostFutures.size(); networkIndex++)
            {
                const double networkCost = networkCostFutures.at(networkIndex).get();
                if (networkCost < bestNetworkCost)
                {
                    bestNetworkCost = networkCost;
                    bestNetworkIndex = networkIndex;
                }
            }

            NeuralNetwork bestNetwork = networks.at(bestNetworkIndex);
            if (generation == 1 ||
                generation % 50 == 0)
            {
                // Test the best performing network
                size_t testingActualNumber = 0;
                const double testExampleCost = bestNetwork.test(trainingDataCollection, testingActualNumber);
                printNetworkResults(generation, testExampleCost, bestNetwork.getLastNeuronLayer(), testingActualNumber);
                printTimeSinceStart();
            }

            for (size_t networkIndex = 0; networkIndex < networks.size(); networkIndex++)
            {
                NeuralNetwork & network = networks.at(networkIndex);
                
                if (networkIndex == bestNetworkIndex)
                {
                    // Keep the best one from last generation as it is
                    continue;
                }

                if (networkIndex >= networksPerGeneration / 5)
                {
                    // Copy the best network to 4/5 of the population
                    network = bestNetwork;

                    // Mutate values a bit
                    const double mutationStrength = 0.01;
                    network.mutate(mutationStrength);
                }
                else
                {
                    // Generate totally new ones for the rest of the population
                    network = NeuralNetwork();
                    network.initialize(inputLayerNeuronCount,
                                       outputLayerNeuronCount,
                                       hiddenLayerCount,
                                       hiddenLayerNeuronCount);
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