#include <iostream>
#include <chrono>
#include <cmath>
#include <algorithm>

#include "NeuronLayer.h"
#include "Neuron.h"

#define NOMINMAX // rang.hpp includes some windows stuff, so this is required for numeric_limits::max to work
#include "rang.hpp"

void printSectionTitle(const std::string & sectionTitle)
{
    using namespace std;

    static unsigned int sectionNumber = 1;
    const string finalSectionTitle = to_string(sectionNumber) + ". " + sectionTitle;

    const string horizontalLine = "  " + string(finalSectionTitle.length() + 4, '#');
    const string spacingRow = "  # " + string(finalSectionTitle.length(), ' ') + " #";
    const string sectionRow = "  # " + finalSectionTitle + " #";
    
    cout << string(70, '_') << endl << endl; // Section separator line

    cout << horizontalLine << endl;
    cout << spacingRow << endl;
    cout << sectionRow << endl;
    cout << spacingRow << endl;
    cout << horizontalLine << endl << endl;

    sectionNumber++;
}

void printTimeSinceStart()
{
    using namespace std::chrono;
    
    const static auto timeStart = std::chrono::high_resolution_clock::now();

    // Show seconds since starting the program
    const auto timeNow = high_resolution_clock::now();
    auto seconds = duration_cast<std::chrono::seconds>(timeNow - timeStart);
    std::cout << seconds.count() << " seconds since start" << std::endl;

    // Show current date and time
    auto now = std::chrono::system_clock::now();
    std::time_t currentDateTime = std::chrono::system_clock::to_time_t(now);
    std::cout << std::ctime(&currentDateTime) << std::endl << std::endl;
}

void printNetworkResults(const int generation,
                         const double cost,
                         const std::shared_ptr<NeuronLayer> & neuronLayer,
                         const size_t actualNumber,
                         const size_t width /* = 20 */)
{    
    // Show performance of the network
    std::cout << "Generation " << generation << " results" << std::endl;
    std::cout << "Network cost: " << cost << std::endl;
    std::cout << "Output layer activations for actual number " << rang::fg::green << actualNumber << rang::style::reset << ":" << std::endl;

    const auto & neurons = neuronLayer->getNeurons();

    for (size_t neuronIndex = 0; neuronIndex < neurons.size(); neuronIndex++)
    {
        const auto & neuron = neurons.at(neuronIndex);
        // Clamp activation between 0 and 1
        double activation = std::clamp(neuron->getActivation(), 0., 1.);
        size_t activationWidth = (size_t) std::round(width * activation);

        std::string activationString =
            std::string(activationWidth, '#') +
            std::string(width - activationWidth, ' ');

        std::cout << (neuronIndex == actualNumber ? rang::fg::green : rang::fg::red);
        std::cout << neuronIndex << ": [" << activationString << "] " << activation << std::endl;
        std::cout << rang::style::reset;
    }

    std::cout << std::endl << std::endl;
}
