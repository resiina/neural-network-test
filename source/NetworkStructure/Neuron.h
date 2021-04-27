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

    void setPrevConnections(std::vector<NeuronConnection*> connections);
    std::vector<NeuronConnection*> getPrevConnections();
    void appendPrevConnection(NeuronConnection* connection);

    void setNextConnections(std::vector<NeuronConnection*> connections);
    std::vector<NeuronConnection*> getNextConnections();
    void appendNextConnection(NeuronConnection* connection);

private:
    double myActivation;
    double myBias;

    std::vector<NeuronConnection*> myPrevConnections;
    std::vector<NeuronConnection*> myNextConnections;
};

#endif // NEURON_H
