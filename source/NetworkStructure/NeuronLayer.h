#ifndef NEURONLAYER_H
#define NEURONLAYER_H

#include <vector>

class Neuron;

class NeuronLayer
{
public:
    NeuronLayer();

    Neuron* getNeuron(const unsigned int index) const;
    std::vector<Neuron*> getNeurons() const;
    void setNeuron(const unsigned int index, Neuron* neuron);
    void appendNeuron(Neuron* neuron);
    void connectNextLayerNeurons(NeuronLayer* nextLayer);
    void connectPrevLayerNeurons(NeuronLayer* prevLayer);
    void clear();

private:
    std::vector<Neuron*> myNeurons;
};

#endif // NEURONLAYER_H
