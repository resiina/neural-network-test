#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <memory>

class NeuronLayer;

class NeuralNetwork
{
public:
    NeuralNetwork();

    std::shared_ptr<NeuronLayer> getNeuronLayer(const size_t index) const;
    void setNeuronLayer(const size_t index, std::shared_ptr<NeuronLayer> neuron);
    void appendNeuronLayer(std::shared_ptr<NeuronLayer> neuron);
    void appendNeuronLayer(const size_t neuronCount);
    void connectLayers();
    void clear();

private:
    std::vector<std::shared_ptr<NeuronLayer>> myNeuronLayers;
};

#endif // NEURALNETWORK_H
