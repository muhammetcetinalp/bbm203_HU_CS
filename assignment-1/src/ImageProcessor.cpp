#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageSharpening imageSharpening;
    ImageMatrix sharpedImage = imageSharpening.sharpen(img,2);

    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> edges = edgeDetector.detectEdges(sharpedImage);

    DecodeMessage decodeMessage;
    std::string hiddenMessage = decodeMessage.decodeFromImage(sharpedImage,edges);

    return hiddenMessage;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    ImageSharpening imageSharpening;
    ImageMatrix sharpedImage = imageSharpening.sharpen(img,2);

    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> edges = edgeDetector.detectEdges(sharpedImage);
    EncodeMessage encodeMessage;
    ImageMatrix lastImage = encodeMessage.encodeMessageToImage(img,message,edges);

    return img;
}
