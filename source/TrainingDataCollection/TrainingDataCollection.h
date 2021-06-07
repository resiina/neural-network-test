#ifndef TrainingDataCollection_H
#define TrainingDataCollection_H

#include <vector>
#include <memory>
#include <string>

struct LabelTrainingExample
{
    std::vector<double> inputLayerActivations; // BW [0, 1]
    std::vector<double> goalOutputLayerActivations; // BW [0, 1], only one index should have value 1.0, rest 0.0
};

struct LabelTrainingData
{
    std::vector<std::shared_ptr<LabelTrainingExample>> trainingExamples; // individual training examples
};

class TrainingDataCollection
{
public:

    const bool collect(const std::string & trainingDataPath);
    const std::vector<std::shared_ptr<LabelTrainingData>> & getLabelsTrainingData() const;

private:

    std::vector<std::shared_ptr<LabelTrainingData>> myLabelsTrainingData;
};

#endif