#ifndef NEURONCONNECTION_H
#define NEURONCONNECTION_H

class Neuron;

class NeuronConnection
{
public:
    NeuronConnection();

    double getWeight() const;
    void setWeight(const double weight);

    Neuron* getConnectedNeuron() const;
    void setConnectedNeuron(Neuron* neuron);

private:
    double myWeight;

    Neuron* myConnectedNeuron;
};

#endif // NEURONCONNECTION_H
