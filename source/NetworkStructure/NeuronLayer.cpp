#include "NeuronLayer.h"

#include <iostream>

#include "Neuron.h"
#include "NeuronConnection.h"

NeuronLayer::NeuronLayer()
{

}

Neuron* NeuronLayer::getNeuron(const unsigned int index) const
{
    return myNeurons.at(index);
}

std::vector<Neuron*> NeuronLayer::getNeurons() const
{
    return myNeurons;
}

void NeuronLayer::setNeuron(const unsigned int index, Neuron* neuron)
{
    myNeurons[index] = neuron;
}

void NeuronLayer::appendNeuron(Neuron* neuron)
{
    myNeurons.push_back(neuron);
}

void NeuronLayer::clear()
{
    myNeurons.clear();
}

void NeuronLayer::connectNextLayerNeurons(NeuronLayer* nextLayer)
{
    std::cout << "Neuron count: " << myNeurons.size() << std::endl;

    for (auto neuron : myNeurons)
    {
        std::vector<Neuron*> nextLayerNeurons = nextLayer->getNeurons();
        for (auto nextLayerNeuron : nextLayerNeurons)
        {
            NeuronConnection* connection = new NeuronConnection;
            connection->setConnectedNeuron(nextLayerNeuron);

            neuron->appendNextConnection(connection);
        }
    }
}

void NeuronLayer::connectPrevLayerNeurons(NeuronLayer* prevLayer)
{
    for (auto neuron : myNeurons)
    {
        for (auto prevLayerNeuron : prevLayer->getNeurons())
        {
            NeuronConnection* connection = new NeuronConnection;
            connection->setConnectedNeuron(prevLayerNeuron);

            neuron->appendPrevConnection(connection);
        }
    }
}
