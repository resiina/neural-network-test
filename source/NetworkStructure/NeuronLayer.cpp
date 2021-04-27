#include "NeuronLayer.h"

#include <iostream>

#include "Neuron.h"
#include "NeuronConnection.h"

NeuronLayer::NeuronLayer()
{

}

std::shared_ptr<Neuron> NeuronLayer::getNeuron(const unsigned int index) const
{
    return myNeurons.at(index);
}

std::vector<std::shared_ptr<Neuron>> NeuronLayer::getNeurons() const
{
    return myNeurons;
}

void NeuronLayer::setNeuron(const unsigned int index, std::shared_ptr<Neuron> neuron)
{
    myNeurons[index] = neuron;
}

void NeuronLayer::appendNeuron(std::shared_ptr<Neuron> neuron)
{
    myNeurons.push_back(neuron);
}

void NeuronLayer::clear()
{
    myNeurons.clear();
}

void NeuronLayer::connectNextLayerNeurons(std::shared_ptr<NeuronLayer> nextLayer)
{
    std::cout << "Neuron count: " << myNeurons.size() << std::endl;

    for (auto neuron : myNeurons)
    {
        std::vector<std::shared_ptr<Neuron>> nextLayerNeurons = nextLayer->getNeurons();
        for (auto nextLayerNeuron : nextLayerNeurons)
        {
            std::shared_ptr<NeuronConnection> connection = std::make_shared<NeuronConnection>();
            connection->setConnectedNeuron(nextLayerNeuron);

            neuron->appendNextConnection(connection);
        }
    }
}

void NeuronLayer::connectPrevLayerNeurons(std::shared_ptr<NeuronLayer> prevLayer)
{
    for (auto neuron : myNeurons)
    {
        for (auto prevLayerNeuron : prevLayer->getNeurons())
        {
            std::shared_ptr<NeuronConnection> connection = std::make_shared<NeuronConnection>();
            connection->setConnectedNeuron(prevLayerNeuron);

            neuron->appendPrevConnection(connection);
        }
    }
}
