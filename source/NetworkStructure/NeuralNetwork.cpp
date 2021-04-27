#include "NeuralNetwork.h"

#include "NeuronLayer.h"

NeuralNetwork::NeuralNetwork()
{

}

std::shared_ptr<NeuronLayer> NeuralNetwork::getNeuronLayer(const unsigned int index) const
{
    return myNeuronLayers.at(index);
}

void NeuralNetwork::setNeuronLayer(const unsigned int index, std::shared_ptr<NeuronLayer> neuronLayer)
{
    myNeuronLayers[index] = neuronLayer;
}

void NeuralNetwork::appendNeuronLayer(std::shared_ptr<NeuronLayer> neuronLayer)
{
    myNeuronLayers.push_back(neuronLayer);
}

void NeuralNetwork::connectLayers()
{
    for (unsigned int i = 0; i < myNeuronLayers.size(); i++)
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

