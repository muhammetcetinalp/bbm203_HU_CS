#include "EncodeMessage.h"
#include <cmath>
#include <bitset>
#include "iostream"



// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {

}
bool isPrime(int value){
    if (value <= 1) {
        return false;
    }
    for (int i = 2; i * i <= value; i++) {
        if (value % i == 0) {
            return false;
        }
    }
    return true;
}
int fibonacciNumber(int n) {
    if (n < 2 ) {
        return n;
    }
    else
        return fibonacciNumber(n-2) + fibonacciNumber(n-1);
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    std::string newMessage;

    for (int i = 0; i < message.length(); i++) {
        char character = message[i];
        if (isPrime(i)) {
            std::bitset<7> bitsValue(character);
            int intValue = static_cast<int>(bitsValue.to_ulong());
            int asciiValue = intValue + fibonacciNumber(i);
            if (asciiValue < 33) {
                asciiValue = 33;
            } else if (asciiValue > 126) {
                asciiValue = 126;
            }
            char asciiCharacter = static_cast<char>(asciiValue);
            character = asciiCharacter;
        }
        newMessage += character;

    }
    int shift = static_cast<int>(std::floor(newMessage.length() / 2));

    std::string temporaryMessage;
    for (int i =0; i<=shift;i++){
        temporaryMessage += newMessage[i];
    }
    for (int i = newMessage.length() - 1; i > shift; i--){
        temporaryMessage = newMessage[i] + temporaryMessage;
    }
    newMessage = "";
    for (char ch : temporaryMessage){
        std::bitset<7> bitsValue(ch);
        char asciiCharacter = static_cast<char>(ch);
        newMessage += asciiCharacter;
    }

    int index = 0;
    for (const auto& pixel : positions) {
        int x = pixel.first;
        int y = pixel.second;
        int pixelValue = img.get_data(x, y);

        for (int bitIndex = 0; bitIndex < 7; ++bitIndex) {
            if (index < temporaryMessage.length()) {
                char character = temporaryMessage[index];
                int bitValue = (character >> bitIndex) & 1;
                pixelValue = (pixelValue & 0xFE) | bitValue;
                img.set_data(x, y, pixelValue);

                ++index;
            } else {
                break;
            }
        }
    }


    return img;
}