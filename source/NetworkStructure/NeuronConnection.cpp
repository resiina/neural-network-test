#include "NeuronConnection.h"

NeuronConnection::NeuronConnection() :
    myWeight(0.5)
{

}

double NeuronConnection::getWeight() const
{
    return myWeight;
}

void NeuronConnection::setWeight(const double weight)
{
    myWeight = weight;
}

std::weak_ptr<Neuron> NeuronConnection::getConnectedNeuron() const
{
    return myConnectedNeuron;
}

void NeuronConnection::setConnectedNeuron(std::weak_ptr<Neuron> neuron)
{
    myConnectedNeuron = neuron;
}
