#ifndef NEURONLAYER_H
#define NEURONLAYER_H

#include <vector>
#include <memory>

class Neuron;

class NeuronLayer
{
public:
    NeuronLayer();
    
    const std::vector<double> getActivations() const;
    bool setActivations(const std::vector<double> & activations);

    std::shared_ptr<Neuron> getNeuron(const size_t index) const;
    std::vector<std::shared_ptr<Neuron>> getNeurons() const;
    void setNeuron(const size_t index, std::shared_ptr<Neuron> neuron);
    void appendNeuron(std::shared_ptr<Neuron> neuron);
    void connectNextLayerNeurons(std::shared_ptr<NeuronLayer> nextLayer);
    void connectPrevLayerNeurons(std::shared_ptr<NeuronLayer> prevLayer);
    void clear();

private:
    std::vector<std::shared_ptr<Neuron>> myNeurons;
};

#endif // NEURONLAYER_H
