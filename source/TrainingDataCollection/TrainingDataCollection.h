#ifndef TrainingDataCollection_H
#define TrainingDataCollection_H

#include <vector>
#include <memory>

struct LabelTrainingExample
{
    size_t label; // output layer index that should have activation 1 and the rest 0 [0, 9]
    //std::vector<unsigned char> data; // RGBA [0, 255]
    std::vector<double> activations; // BW [0, 1]
};

struct LabelTrainingData
{
    size_t label; // output layer index that should have activation 1 and the rest 0 [0, 9]
    std::vector<std::shared_ptr<LabelTrainingExample>> trainingExamples; // individual training examples
};

struct TrainingDataCollection
{
    std::vector<std::shared_ptr<LabelTrainingData>> labelTrainingData;
};

#endif