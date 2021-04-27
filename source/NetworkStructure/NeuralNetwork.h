#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>

class NeuronLayer;

class NeuralNetwork
{
public:
    NeuralNetwork();

    NeuronLayer* getNeuronLayer(const unsigned int index) const;
    void setNeuronLayer(const unsigned int index, NeuronLayer* neuron);
    void appendNeuronLayer(NeuronLayer* neuron);
    void connectLayers();
    void clear();

private:
    std::vector<NeuronLayer*> myNeuronLayers;
};

#endif // NEURALNETWORK_H
