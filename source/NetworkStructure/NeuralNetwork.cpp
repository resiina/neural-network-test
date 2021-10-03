#include "NeuralNetwork.h"

#include "NeuronLayer.h"
#include "Neuron.h"

#include "TrainingDataCollection.h"

#include <stdexcept>
#include <iostream>

NeuralNetwork::NeuralNetwork()
{

}

void NeuralNetwork::initialize(const size_t inputLayerNeuronCount,
                               const size_t outputLayerNeuronCount,
                               const size_t hiddenLayerCount,
                               const size_t hiddenLayerNeuronCount)
{
    // 1. Define input layer
    appendNeuronLayer(inputLayerNeuronCount);

    // 2. Define some hidden layers
    for (size_t hiddenLayerIndex = 0; hiddenLayerIndex < hiddenLayerCount; hiddenLayerIndex++)
    {
        appendNeuronLayer(hiddenLayerNeuronCount);
    }
    
    // 3. Define output layer
    appendNeuronLayer(outputLayerNeuronCount);

    connectLayers();
}

void NeuralNetwork::compute()
{
    for (auto & neuronLayer : myNeuronLayers)
    {
        for (auto & neuron : neuronLayer->getNeurons())
        {
            neuron->computeActivation();
        }
    }
}

std::vector<std::shared_ptr<NeuronLayer>> NeuralNetwork::getNeuronLayers() const
{
    return myNeuronLayers;
}

std::shared_ptr<NeuronLayer> NeuralNetwork::getNeuronLayer(const size_t index) const
{
    return myNeuronLayers.at(index);
}

std::shared_ptr<NeuronLayer> NeuralNetwork::getFirstNeuronLayer() const
{
    return *(myNeuronLayers.begin());
}

std::shared_ptr<NeuronLayer> NeuralNetwork::getLastNeuronLayer() const
{
    return *(myNeuronLayers.rbegin());
}

void NeuralNetwork::setNeuronLayer(const size_t index, std::shared_ptr<NeuronLayer> neuronLayer)
{
    myNeuronLayers[index] = neuronLayer;
}

void NeuralNetwork::appendNeuronLayer(std::shared_ptr<NeuronLayer> neuronLayer)
{
    myNeuronLayers.push_back(neuronLayer);
}

void NeuralNetwork::appendNeuronLayer(const size_t neuronCount)
{
    std::shared_ptr<NeuronLayer> newLayer = std::make_shared<NeuronLayer>();

    for (size_t neuronIndex = 0; neuronIndex < neuronCount; neuronIndex++)
    {
        std::shared_ptr<Neuron> neuron = std::make_shared<Neuron>();
        newLayer->appendNeuron(neuron);
    }

    myNeuronLayers.push_back(newLayer);
}

void NeuralNetwork::connectLayers()
{
    for (size_t i = 0; i < myNeuronLayers.size(); i++)
    {
        if (i > 0)
        {
            myNeuronLayers[i - 1]->connectPrevLayerNeurons(myNeuronLayers[i]);
        }

        if (i < myNeuronLayers.size() - 1)
        {
            myNeuronLayers[i]->connectNextLayerNeurons(myNeuronLayers[i + 1]);
        }
    }
}

void NeuralNetwork::clear()
{
    myNeuronLayers.clear();
}

void NeuralNetwork::mutate(const double factor)
{
    auto getUnitMutator = []()
    {
        return (std::rand() % 200) / 100.0 - 1.0; // Something between -1 and 1
    };

    for (size_t i = 0; i < myNeuronLayers.size(); i++)
    {
        auto & neurons = myNeuronLayers.at(i)->getNeurons();
        for (auto & neuron : neurons)
        {
            neuron->setBias(neuron->getBias() + getUnitMutator() * factor);

            for (auto & nextConnection : neuron->getNextConnections())
            {
                nextConnection->setWeight(nextConnection->getWeight() + getUnitMutator() * factor);
            }
        }
    }
}

double NeuralNetwork::train(const std::shared_ptr<TrainingDataCollection> & trainingDataCollection)
{
    double totalExamplesCost = 0;

    for (int trainingExampleNumber = 0; trainingExampleNumber < 50; trainingExampleNumber++)
    {
        size_t testingActualNumber = 0; // Dummy
        totalExamplesCost += test(trainingDataCollection, testingActualNumber);
    }

    return totalExamplesCost;
}
    
double NeuralNetwork::test(const std::shared_ptr<TrainingDataCollection> & testingDataCollection, size_t & testingActualNumber)
{
    // Setup a test for the network
    testingActualNumber = std::rand() % 10; // Pick a random number between 0-9 for the test case
    const auto & testingExamples = testingDataCollection->getLabelsTrainingData()[testingActualNumber]->trainingExamples;
    const size_t testingExampleIndex = std::rand() % testingExamples.size();
    const auto & testingExampleInputActivations = testingExamples[testingExampleIndex]->inputLayerActivations;

    // Inputs
    getFirstNeuronLayer()->setActivations(testingExampleInputActivations);
    
    // Expected outputs
    const auto & testingExampleGoalActivations = testingExamples[testingExampleIndex]->goalOutputLayerActivations;

    // Execute test
    compute();

    // Check results
    const double testExampleCost = NeuralNetwork::calculateCost(getLastNeuronLayer()->getActivations(), testingExampleGoalActivations);

    return testExampleCost;
}

void NeuralNetwork::operator=(const NeuralNetwork & other)
{
    myNeuronLayers.clear();

    auto & otherNeuronLayers = other.getNeuronLayers();
    for (auto & otherNeuronLayer : otherNeuronLayers)
    {
        std::shared_ptr<NeuronLayer> neuronLayerCopy = std::make_shared<NeuronLayer>();
        *(neuronLayerCopy.get()) = *(otherNeuronLayer.get());

        myNeuronLayers.push_back(neuronLayerCopy);
    }

    // Create new connections
    connectLayers();

    // Copy connection weights
    for (size_t neuronLayerIndex = 0; neuronLayerIndex < myNeuronLayers.size(); neuronLayerIndex++)
    {
        auto & neurons = myNeuronLayers.at(neuronLayerIndex)->getNeurons();
        auto & otherNeurons = otherNeuronLayers.at(neuronLayerIndex)->getNeurons();
        for (size_t neuronIndex = 0; neuronIndex < neurons.size(); neuronIndex++)
        {
            auto & neuronConnections = neurons.at(neuronIndex)->getNextConnections();
            auto & otherNeuronConnections = otherNeurons.at(neuronIndex)->getNextConnections();
            for (size_t connectionIndex = 0; connectionIndex < neuronConnections.size(); connectionIndex++)
            {
                auto & neuronConnection = neuronConnections.at(connectionIndex);
                auto & otherNeuronConnection = otherNeuronConnections.at(connectionIndex);
                neuronConnection->setWeight(otherNeuronConnection->getWeight());
            }
        }
    }
}

double NeuralNetwork::calculateCost(const std::vector<double> & actualActivations,
                                    const std::vector<double> & goalActivations)
{
    // In the goal activations, one index should have activation of 1.0 and the rest should be 0.0
    // The actual activations should be the actual output activations that the network currently outputs with specified inputs
    if (actualActivations.size() != goalActivations.size())
    {
        throw std::runtime_error("Actual and goal activations sizes differ");
    }

    double totalCost = 0.0;
    for (size_t i = 0; i < actualActivations.size(); i++)
    {
        const double difference = actualActivations.at(i) - goalActivations.at(i);
        totalCost += difference * difference;
    }

    size_t correctIndex = 0;
    double highestGoalActivation = 0.0;
    for (size_t i = 0; i < goalActivations.size(); i++)
    {
        if (highestGoalActivation < goalActivations.at(i))
        {
            highestGoalActivation = goalActivations.at(i);
            correctIndex = i;
        }
    }

    // Add extra cost from difference with the correct classification result
    const double differenceCorrect = actualActivations.at(correctIndex) - goalActivations.at(correctIndex);
    totalCost += 10 * std::abs(differenceCorrect);

    return totalCost;
}
