#ifndef COMMANDLINEINTERFACE_H
#define COMMANDLINEINTERFACE_H

#include <string>

class NeuronLayer;

void printSectionTitle(const std::string & sectionTitle);

void printTimeSinceStart();

void printNetworkResults(const int generation,
                         const double cost,
                         const std::shared_ptr<NeuronLayer> & neuronLayer,
                         const size_t width,
                         const size_t actualNumber);

#endif