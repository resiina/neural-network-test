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

    std::shared_ptr<Neuron> getConnectedNeuron() const;
    void setConnectedNeuron(std::shared_ptr<Neuron> neuron);

private:
    double myWeight;

    std::shared_ptr<Neuron> myConnectedNeuron;
};

#endif // NEURONCONNECTION_H
