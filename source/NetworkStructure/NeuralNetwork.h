#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <memory>

class NeuronLayer;
class TrainingDataCollection;

class NeuralNetwork
{
public:
    NeuralNetwork();

    void compute();

    std::vector<std::shared_ptr<NeuronLayer>> getNeuronLayers() const;
    
    std::shared_ptr<NeuronLayer> getNeuronLayer(const size_t index) const;
    std::shared_ptr<NeuronLayer> getFirstNeuronLayer() const;
    std::shared_ptr<NeuronLayer> getLastNeuronLayer() const;
    
    void setNeuronLayer(const size_t index, std::shared_ptr<NeuronLayer> neuron);
    void appendNeuronLayer(std::shared_ptr<NeuronLayer> neuron);
    void appendNeuronLayer(const size_t neuronCount);
    void connectLayers();
    void clear();

    void train(const std::shared_ptr<TrainingDataCollection> & trainingDataCollection);

private:
    std::vector<std::shared_ptr<NeuronLayer>> myNeuronLayers;
    
    static double costFunction(const std::vector<double> & actualActivations,
                               const std::vector<double> & goalActivations);
};

#endif // NEURALNETWORK_H
