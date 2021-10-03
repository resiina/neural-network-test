#include "DataCollection.h"

#include <filesystem>

#include "lodepng.h"

DataCollection::DataCollection(const std::string & dataPath)
{
    for (size_t number = 0; number < 10; number++)
    {
        namespace fs = std::filesystem;

        std::shared_ptr<LabelData> labelData = std::make_shared<LabelData>(); 
        for (auto & entry : fs::directory_iterator(dataPath + std::to_string(number)))
        {
            const std::string filePath = entry.path().string();
            
            std::vector<unsigned char> imageData;
            unsigned int imageWidth;
            unsigned int imageHeight;

            const unsigned int pngDecodeResult = lodepng::decode(imageData, imageWidth, imageHeight, filePath);
            if (pngDecodeResult != 0)
            {
                std::stringstream ss;
                ss << "Loading PNG Failed. Decoder error " << pngDecodeResult << ": " << lodepng_error_text(pngDecodeResult);
                throw std::runtime_error(ss.str());
            }

            std::shared_ptr<LabelExample> labelExample = std::make_shared<LabelExample>();
            const size_t bytesPerPixel = 4;

            for (size_t i = 0; i < imageData.size(); i += bytesPerPixel)
            {
                const unsigned char & red   = imageData.at(i);
                const unsigned char & green = imageData.at(i + 1);
                const unsigned char & blue  = imageData.at(i + 2);
                //const unsigned char & alpha = imageData.at(i + 3);

                const double activation = (red + green + blue) / 3.0 / 255.0; // Gray [0, 1]
                labelExample->inputLayerActivations.push_back(activation);
            }

            labelExample->goalOutputLayerActivations = std::vector<double>(10, 0.0);
            labelExample->goalOutputLayerActivations.at(number) = 1.0;

            labelData->examples.push_back(labelExample);
        }

        myLabelsData.push_back(labelData);
    }
}

const std::vector<std::shared_ptr<LabelData>> & DataCollection::getLabelsData() const
{
    return myLabelsData;
}