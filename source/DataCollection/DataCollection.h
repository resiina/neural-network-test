#ifndef DataCollection_H
#define DataCollection_H

#include <vector>
#include <memory>
#include <string>

struct LabelExample
{
    std::vector<double> inputLayerActivations; // BW [0, 1]
    std::vector<double> goalOutputLayerActivations; // [0, 1], only one index should have value 1.0, rest 0.0
};

struct LabelData
{
    std::vector<std::shared_ptr<LabelExample>> examples; // individual training examples
};

class DataCollection
{
public:
    DataCollection(const std::string & dataPath);
    const std::vector<std::shared_ptr<LabelData>> & getLabelsData() const;

private:

    std::vector<std::shared_ptr<LabelData>> myLabelsData;
};

#endif