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

