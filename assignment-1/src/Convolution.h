// Convolution.h

#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "ImageMatrix.h"

// Class `Convolution`: Provides the functionality to convolve an image with
// a customKernel. Padding is a bool variable, indicating whether to use zero padding or not.
class Convolution {
public:
    // Constructors and destructors
    Convolution(); // Default constructor
    Convolution(double** customKernel, int kernelHeight, int kernelWidth, int stride, bool padding); // Parametrized constructor for custom customKernel and other parameters
    ~Convolution(); // Destructor

    Convolution(const Convolution &other); // Copy constructor
    Convolution& operator=(const Convolution &other); // Copy assignment operator

    ImageMatrix convolve(const ImageMatrix& input_image)const; // Convolve Function: Responsible for convolving the input image with a customKernel and return the convolved image.
private:
    double** customKernel;
    int kernelHeight;
    int kernelWidth;
    int stride;
    bool padding;
};

#endif // CONVOLUTION_H
