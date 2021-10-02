#include <iostream>
#include <iomanip>
#include <filesystem>
#include <limits>
#include <chrono>
#include <ctime>
#include <thread>
#include <future>

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

void printTimeSince(const std::chrono::steady_clock::time_point & timeStart)
{
    using namespace std::chrono;

    // Show seconds since starting the program
    const auto timeNow = high_resolution_clock::now();
    auto seconds = duration_cast<std::chrono::seconds>(timeNow - timeStart);
    std::cout << seconds.count() << " seconds since start" << std::endl;

    // Show current date and time
    auto now = std::chrono::system_clock::now();
    std::time_t currentDateTime = std::chrono::system_clock::to_time_t(now);
    std::cout << std::ctime(&currentDateTime) << std::endl;

    std::cout << std::endl << std::endl;
}

double trainNetwork(NeuralNetwork & network, const std::shared_ptr<TrainingDataCollection> & trainingDataCollection)
{
    double totalExamplesCost = 0;

    for (int trainingExampleNumber = 0; trainingExampleNumber < 50; trainingExampleNumber++)
    {
        // 1. Get a random image of random number between 0-10
        const size_t trainingExampleActualNumber = std::rand() % 10;
        const auto & trainingExamples = trainingDataCollection->getLabelsTrainingData()[trainingExampleActualNumber]->trainingExamples;

        const size_t trainingExampleIndex = std::rand() % trainingExamples.size();
        const auto & trainingExampleActivations = trainingExamples[trainingExampleIndex]->inputLayerActivations;

        network.getFirstNeuronLayer()->setActivations(trainingExampleActivations);

        // 2. Compute results
        network.compute();

        // 3. Compare output to goal
        const auto & goalOutputActivations = trainingExamples[trainingExampleIndex]->goalOutputLayerActivations;

        const double cost = NeuralNetwork::calculateCost(network.getLastNeuronLayer()->getActivations(), goalOutputActivations);
        totalExamplesCost += cost;
    }

    return totalExamplesCost;
}

int main()
{
    try
    {
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

        printSectionTitle("Construct the network");

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
        
        std::shared_ptr<TrainingDataCollection> trainingDataCollection = std::make_shared<TrainingDataCollection>();
        trainingDataCollection->collect("../../mnist_png/training/");

        printSectionTitle("Train the network");

        const auto timeStart = std::chrono::high_resolution_clock::now();

        std::vector<std::future<double>> networkCostFutures(networks.size());

        const size_t totalGenerations = 45000;
        for (int generation = 1; generation <= totalGenerations; generation++)
        {
            for (size_t networkIndex = 0; networkIndex < networks.size(); networkIndex++)
            {
                NeuralNetwork & network = networks.at(networkIndex);
                networkCostFutures[networkIndex] = std::async(trainNetwork, network, trainingDataCollection);
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
                generation == totalGenerations ||
                generation % 50 == 0)
            {
                // Setup a test for the best performing network
                const size_t testingActualNumber = std::rand() % 10;
                const auto & testingExamples = trainingDataCollection->getLabelsTrainingData()[testingActualNumber]->trainingExamples;
                const size_t testingExampleIndex = std::rand() % testingExamples.size();
                const auto & testingExampleInputActivations = testingExamples[testingExampleIndex]->inputLayerActivations;

                bestNetwork.getFirstNeuronLayer()->setActivations(testingExampleInputActivations);

                // Execute test
                bestNetwork.compute();

                const auto & testingExampleGoalActivations = testingExamples[testingExampleIndex]->goalOutputLayerActivations;

                const double testExampleCost = NeuralNetwork::calculateCost(bestNetwork.getLastNeuronLayer()->getActivations(), testingExampleGoalActivations);

                // Show performance of current best network
                std::cout << "Best cost of generation " << generation << ": " << testExampleCost << std::endl;
                std::cout << "Output layer activations for actual number " << testingActualNumber << ":" << std::endl;
                
                const auto & outputLayerNeurons = bestNetwork.getLastNeuronLayer()->getNeurons();
                for (size_t neuronIndex = 0; neuronIndex < outputLayerNeurons.size(); neuronIndex++)
                {
                    const auto & outputNeuron = outputLayerNeurons.at(neuronIndex);
                    std::cout << neuronIndex << ": " << outputNeuron->getActivation() << std::endl;
                }
                
                std::cout << std::endl << std::endl;

                printTimeSince(timeStart);
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