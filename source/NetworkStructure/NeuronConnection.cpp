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

Neuron* NeuronConnection::getConnectedNeuron() const
{
    return myConnectedNeuron;
}

void NeuronConnection::setConnectedNeuron(Neuron* neuron)
{
    myConnectedNeuron = neuron;
}
