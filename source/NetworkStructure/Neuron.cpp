#include "Neuron.h"

#include <math.h>

Neuron::Neuron() :
    myActivation(0)
{

}

void Neuron::computeActivation()
{
    double sum = 0;
    for (auto connection : myPrevConnections)
    {
        const double weight = connection->getWeight();
        const double connectedActivation = connection->getConnectedNeuron()->getActivation();

        sum += weight * connectedActivation;
    }

    sum += myBias;

    myActivation = (tanh(sum) + 1.0) / 2.0;
}

void Neuron::setActivation(const double activation)
{
    myActivation = activation;
}

double Neuron::getActivation() const
{
    return myActivation;
}

void Neuron::setBias(const double bias)
{
    myBias = bias;
}

double Neuron::getBias() const
{
    return myBias;
}

void Neuron::setPrevConnections(std::vector<NeuronConnection*> connections)
{
    myPrevConnections = connections;
}

std::vector<NeuronConnection*> Neuron::getPrevConnections()
{
    return myPrevConnections;
}

void Neuron::appendPrevConnection(NeuronConnection* connection)
{
    myPrevConnections.push_back(connection);
}

void Neuron::setNextConnections(std::vector<NeuronConnection*> connections)
{
    myNextConnections = connections;
}

std::vector<NeuronConnection*> Neuron::getNextConnections()
{
    return myNextConnections;
}

void Neuron::appendNextConnection(NeuronConnection* connection)
{
    myNextConnections.push_back(connection);
}
