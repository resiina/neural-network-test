#ifndef NEURONCONNECTION_H
#define NEURONCONNECTION_H

#include <memory>

class Neuron;

class NeuronConnection
{
public:
    NeuronConnection();

    double getWeight() const;
    void setWeight(const double weight);

    std::weak_ptr<Neuron> getConnectedNeuron() const;
    void setConnectedNeuron(std::weak_ptr<Neuron> neuron);

private:
    double myWeight;

    std::weak_ptr<Neuron> myConnectedNeuron;
};

#endif // NEURONCONNECTION_H
