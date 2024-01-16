// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <bitset>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string message;
    for (const auto& pixel : edgePixels){
        int LSB = image.getLSB(pixel.first,pixel.second);
        message += std::to_string(LSB);
    }
    int x = message.length() % 7;
    if (x){
        std::string extraBits;
        for (int i = 1;i<=7-x;i++){
            extraBits += "0";
        }
        message = extraBits + message;
    }

    std::string decodedMessage;
    for (int i = 0;i<message.length();i+=7){
        std::string segment = message.substr(i, 7);
        int segmentValue = 0;
        for (int j = 0; j < 7; j++) {
            if (segment[j] == '1') {
                segmentValue |= (1 << (6 - j));
            }
        }
        if (segmentValue <= 32) {
            segmentValue += 33;
        } else if (segmentValue == 127) {
            segmentValue = 126;
        }
        char character = static_cast<char>(segmentValue);
        decodedMessage += character;

    }
    return decodedMessage;


}

