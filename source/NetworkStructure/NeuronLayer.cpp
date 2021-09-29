#include "NeuronLayer.h"

#include <iostream>

#include "Neuron.h"
#include "NeuronConnection.h"

NeuronLayer::NeuronLayer()
{

}
    
const std::vector<double> NeuronLayer::getActivations() const
{
    std::vector<double> activations;
    for (const auto & neuron : myNeurons)
    {
        activations.push_back(neuron->getActivation());
    }

    return activations;
}

bool NeuronLayer::setActivations(const std::vector<double> & activations)
{
    if (activations.size() != myNeurons.size())
    {
        throw std::runtime_error("Neurons and activations sizes mismatch");
    }

    for (size_t i = 0; i < myNeurons.size(); i++)
    {
        myNeurons.at(i)->setActivation(activations.at(i));
    }

    return true;
}

std::shared_ptr<Neuron> NeuronLayer::getNeuron(const size_t index) const
{
    return myNeurons.at(index);
}

std::vector<std::shared_ptr<Neuron>> NeuronLayer::getNeurons() const
{
    return myNeurons;
}

void NeuronLayer::setNeuron(const size_t index, std::shared_ptr<Neuron> neuron)
{
    myNeurons[index] = neuron;
}

void NeuronLayer::appendNeuron(std::shared_ptr<Neuron> neuron)
{
    myNeurons.push_back(neuron);
}

void NeuronLayer::clear()
{
    myNeurons.clear();
}

void NeuronLayer::connectNextLayerNeurons(std::shared_ptr<NeuronLayer> nextLayer)
{
    for (auto & neuron : myNeurons)
    {
        std::vector<std::shared_ptr<Neuron>> nextLayerNeurons = nextLayer->getNeurons();
        for (auto & nextLayerNeuron : nextLayerNeurons)
        {
            std::shared_ptr<NeuronConnection> connection = std::make_shared<NeuronConnection>();
            connection->setConnectedNeuron(nextLayerNeuron);
            
            // Set random initial weight
            const double weight = (std::rand() % 100) / 100.0;
            connection->setWeight(weight);

            neuron->appendNextConnection(connection);
        }
    }
}

void NeuronLayer::connectPrevLayerNeurons(std::shared_ptr<NeuronLayer> prevLayer)
{
    for (auto & neuron : myNeurons)
    {
        for (auto & prevLayerNeuron : prevLayer->getNeurons())
        {
            std::shared_ptr<NeuronConnection> connection = std::make_shared<NeuronConnection>();
            connection->setConnectedNeuron(prevLayerNeuron);

            // Set random initial weight
            const double weight = (std::rand() % 100) / 100.0;
            connection->setWeight(weight);

            neuron->appendPrevConnection(connection);
        }
    }
}
