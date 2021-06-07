#include "NeuralNetwork.h"

#include "NeuronLayer.h"
#include "Neuron.h"

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
    // TODO:
    // 1. Define cost function
    // 2. Train the network by minimizing the cost function using gradient descent
}

double costFunction(const std::vector<double> & actualActivations,
                    const std::vector<double> & goalActivations)
{
    // In the goal activations, one index should have activation of 1.0 and the rest should be 0.0
    // The actual activations should be the actual output activations that the network currently outputs with specified inputs
    if (actualActivations.size() != goalActivations.size())
    {
        // Error
    }

    double totalCost = 0.0;
    for (int i = 0; i < actualActivations.size(); i++)
    {
        const double difference = actualActivations.at(i) - goalActivations.at(i);
        totalCost += difference * difference;
    }

    return totalCost;
}
