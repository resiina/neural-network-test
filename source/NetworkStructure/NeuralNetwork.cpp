#include "NeuralNetwork.h"

#include "NeuronLayer.h"

NeuralNetwork::NeuralNetwork()
{

}

NeuronLayer* NeuralNetwork::getNeuronLayer(const unsigned int index) const
{
    return myNeuronLayers.at(index);
}

void NeuralNetwork::setNeuronLayer(const unsigned int index, NeuronLayer* neuronLayer)
{
    myNeuronLayers[index] = neuronLayer;
}

void NeuralNetwork::appendNeuronLayer(NeuronLayer* neuronLayer)
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

