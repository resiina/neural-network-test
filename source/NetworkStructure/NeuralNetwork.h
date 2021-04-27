#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <memory>

class NeuronLayer;

class NeuralNetwork
{
public:
    NeuralNetwork();

    std::shared_ptr<NeuronLayer> getNeuronLayer(const unsigned int index) const;
    void setNeuronLayer(const unsigned int index, std::shared_ptr<NeuronLayer> neuron);
    void appendNeuronLayer(std::shared_ptr<NeuronLayer> neuron);
    void connectLayers();
    void clear();

private:
    std::vector<std::shared_ptr<NeuronLayer>> myNeuronLayers;
};

#endif // NEURALNETWORK_H
