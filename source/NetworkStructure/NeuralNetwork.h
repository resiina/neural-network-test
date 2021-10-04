#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <memory>

class NeuronLayer;
class DataCollection;

class NeuralNetwork
{
public:
    NeuralNetwork();

    void initialize(const size_t inputLayerNeuronCount,
                    const size_t outputLayerNeuronCount,
                    const size_t hiddenLayerCount,
                    const size_t hiddenLayerNeuronCount);

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

    void mutate(const double factor);

    double train(const std::shared_ptr<DataCollection> & trainingDataCollection,
                 const size_t exampleActualNumber,
                 const double exampleRelativePosition);
    double test(const std::shared_ptr<DataCollection> & testingDataCollection,
                const size_t exampleActualNumber,
                const double exampleRelativePosition);

    // Deep copy
    void operator=(const NeuralNetwork & other);

    static double calculateCost(const std::vector<double> & actualActivations,
                                const std::vector<double> & goalActivations);

private:
    std::vector<std::shared_ptr<NeuronLayer>> myNeuronLayers;
    
};

#endif // NEURALNETWORK_H
