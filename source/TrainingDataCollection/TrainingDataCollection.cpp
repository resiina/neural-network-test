#include "TrainingDataCollection.h"

#include <iostream>
#include <filesystem>

#include "lodepng.h"

const bool TrainingDataCollection::collect(const std::string & trainingDataPath)
{
    for (size_t number = 0; number < 10; number++)
    {
        namespace fs = std::filesystem;

        std::shared_ptr<LabelTrainingData> labelTrainingData = std::make_shared<LabelTrainingData>(); 
        for (auto & entry : fs::directory_iterator(trainingDataPath + std::to_string(number)))
        {
            const std::string trainingFilePath = entry.path().string();
            
            std::vector<unsigned char> trainingImageData;
            unsigned int imageWidth;
            unsigned int imageHeight;

            const unsigned int pngDecodeResult = lodepng::decode(trainingImageData, imageWidth, imageHeight, trainingFilePath);
            if (pngDecodeResult != 0)
            {
                std::cout << "Loading PNG failed." << std::endl
                          << "Decoder error " << pngDecodeResult << ": " << lodepng_error_text(pngDecodeResult) << std::endl;
                return false;
            }

            std::shared_ptr<LabelTrainingExample> trainingExample = std::make_shared<LabelTrainingExample>();
            const size_t bytesPerPixel = 4;

            for (size_t i = 0; i < trainingImageData.size(); i += bytesPerPixel)
            {
                const unsigned char & red   = trainingImageData.at(i);
                const unsigned char & green = trainingImageData.at(i + 1);
                const unsigned char & blue  = trainingImageData.at(i + 2);
                //const unsigned char & alpha = trainingImageData.at(i + 3);

                const double activation = (red + green + blue) / 3.0 / 255.0; // Gray [0, 1]
                trainingExample->inputLayerActivations.push_back(activation);
            }

            trainingExample->goalOutputLayerActivations = std::vector<double>(10, 0.0);
            trainingExample->goalOutputLayerActivations.at(number) = 1.0;

            labelTrainingData->trainingExamples.push_back(trainingExample);
        }

        myLabelsTrainingData.push_back(labelTrainingData);
    }

    return true;
}

const std::vector<std::shared_ptr<LabelTrainingData>> & TrainingDataCollection::getLabelsTrainingData() const
{
    return myLabelsTrainingData;
}