#include "Neuron.h"

#include <math.h>

Neuron::Neuron()
{
    // Initialize each neuron with random values
    myActivation = (std::rand() % 100) / 100.0;
    myBias = (std::rand() % 100) / 100.0;
}

void Neuron::computeActivation()
{
    double sum = 0;
    for (auto connection : myPrevConnections)
    {
        const double weight = connection->getWeight();
        const double connectedActivation = connection->getConnectedNeuron().lock()->getActivation();

        sum += weight * connectedActivation;
    }

    sum += myBias;

    // myActivation = 1 / (1 + exp(-sum)); // Sigmoid alternative
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

void Neuron::setPrevConnections(std::vector<std::shared_ptr<NeuronConnection>> connections)
{
    myPrevConnections = connections;
}

std::vector<std::shared_ptr<NeuronConnection>> Neuron::getPrevConnections()
{
    return myPrevConnections;
}

void Neuron::appendPrevConnection(std::shared_ptr<NeuronConnection> connection)
{
    myPrevConnections.push_back(connection);
}

void Neuron::setNextConnections(std::vector<std::shared_ptr<NeuronConnection>> connections)
{
    myNextConnections = connections;
}

std::vector<std::shared_ptr<NeuronConnection>> Neuron::getNextConnections()
{
    return myNextConnections;
}

void Neuron::appendNextConnection(std::shared_ptr<NeuronConnection> connection)
{
    myNextConnections.push_back(connection);
}

void Neuron::operator=(const Neuron & other)
{
    myActivation = other.myActivation;
    myBias = other.myBias;
}