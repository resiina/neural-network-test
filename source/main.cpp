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
        std::cout << std::setprecision(2);

        // -----------------------------------------------------
        const unsigned int inputImageWidth = 28;
        const unsigned int inputImageHeight = 28;

        printSectionTitle("Construct the network");

        const size_t networksPerGeneration = 20;

        std::vector<NeuralNetwork> networks;
        for (int networkIndex = 0; networkIndex < networksPerGeneration; networkIndex++)
        {
            NeuralNetwork network;

            // Input layer takes all image pixels as activations
            // Network outputs 10 different possible results, an integer from 0 to 9
            network.initialize(inputImageWidth * inputImageHeight, 10, 2, 16);

            networks.push_back(network);
        }
        

        printSectionTitle("Collect training data");
        
        std::shared_ptr<TrainingDataCollection> trainingDataCollection = std::make_shared<TrainingDataCollection>();
        trainingDataCollection->collect("../../data/training/");

        printSectionTitle("Train the network");
        
        const size_t totalGenerations = 10000;

        const size_t trainingExamplesSize = trainingDataCollection->getLabelsTrainingData()[0]->trainingExamples.size();
        for (int generation = 1; generation <= totalGenerations; generation++)
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

            NeuralNetwork bestNetwork = networks.at(bestNetworkIndex); // TODO: Do deep copy
            if (generation == 1 ||
                generation == totalGenerations ||
                generation % 50 == 0)
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

            for (size_t networkIndex = 0; networkIndex < networks.size(); networkIndex++)
            {
                NeuralNetwork & network = networks.at(networkIndex);
                
                if (networkIndex == bestNetworkIndex)
                {
                    // Keep the best one from last generation as it is
                    continue;
                }

                if (networkIndex <= networksPerGeneration / 2)
                {
                    // Copy best network to half of the population
                    network = bestNetwork;

                    // Mutate values a bit
                    const double mutationStrength = 0.01;
                    network.mutate(mutationStrength);
                }
                else
                {
                    // Generate totally new ones for the other half of the population
                    network = NeuralNetwork();
                    network.initialize(inputImageWidth * inputImageHeight, 10, 2, 16);
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