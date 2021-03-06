#include "NeuralNetwork.h"

#include "NeuronLayer.h"
#include "Neuron.h"

#include "TrainingDataCollection.h"

#include <stdexcept>
#include <iostream>

NeuralNetwork::NeuralNetwork()
{

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

void NeuralNetwork::train(const std::shared_ptr<TrainingDataCollection> & trainingDataCollection)
{
    std::shared_ptr<NeuronLayer> inputLayer = getFirstNeuronLayer();

    const std::vector<std::shared_ptr<LabelTrainingData>> & labelsTrainingData = trainingDataCollection->getLabelsTrainingData();
    for (const auto & labelTrainingData : labelsTrainingData)
    {
        for (const auto & trainingExample : labelTrainingData->trainingExamples)
        {
            // 1. Set input layer activations based on input image
            inputLayer->setActivations(trainingExample->inputLayerActivations);

            // 2. Compute results
            compute();

            // 3. Calculate output cost
            const double cost = calculateCost(getLastNeuronLayer()->getActivations(),
                                              trainingExample->goalOutputLayerActivations);

            std::cout << "Cost: " << cost << std::endl;

            // TODO:
            // 4. Do back propagation
        }

        // TODO:
        // - Calculate the gradient descent
        // - Adjust the weights and biases according to the gradient
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
    for (int i = 0; i < actualActivations.size(); i++)
    {
        const double difference = actualActivations.at(i) - goalActivations.at(i);
        totalCost += difference * difference;
    }

    return totalCost;
}
