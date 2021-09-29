#ifndef NEURON_H
#define NEURON_H

#include <vector>

#include "NeuronConnection.h"

class Neuron
{
public:
    Neuron();

    void computeActivation();

    void setActivation(const double activation);
    double getActivation() const;

    void setBias(const double bias);
    double getBias() const;

    void setPrevConnections(std::vector<std::shared_ptr<NeuronConnection>> connections);
    std::vector<std::shared_ptr<NeuronConnection>> getPrevConnections();
    void appendPrevConnection(std::shared_ptr<NeuronConnection> connection);

    void setNextConnections(std::vector<std::shared_ptr<NeuronConnection>> connections);
    std::vector<std::shared_ptr<NeuronConnection>> getNextConnections();
    void appendNextConnection(std::shared_ptr<NeuronConnection> connection);

    void operator=(const Neuron & other);

private:
    double myActivation;
    double myBias;

    std::vector<std::shared_ptr<NeuronConnection>> myPrevConnections;
    std::vector<std::shared_ptr<NeuronConnection>> myNextConnections;
};

#endif // NEURON_H
