#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {
    kernel_height = 3;
    kernel_width = 3;
    blurring_kernel = new double*[kernel_height];
    for (int i = 0; i < kernel_height; ++i) {
        blurring_kernel[i] = new double[kernel_width];
    }
    for (int i = 0; i < kernel_height; ++i) {
        for (int j = 0; j < kernel_width; j++) {
            blurring_kernel[i][j] = 1.0/9;
        }
    }
}

ImageSharpening::~ImageSharpening(){
    if (blurring_kernel != nullptr) {
        for (int i = 0; i < kernel_height; ++i) {
            delete[] blurring_kernel[i];
        }
        delete[] blurring_kernel;
    }

}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    Convolution convolution(blurring_kernel,3,3,1,true);
    ImageMatrix blurredImage = convolution.convolve(input_image);

    ImageMatrix sharpedImage = input_image +  (input_image - blurredImage)*k;
    for (int i = 0;i<sharpedImage.get_height(); i++) {
        for (int j = 0; j < sharpedImage.get_width(); j++) {
            double compareData = sharpedImage.get_data(i, j);
            if (compareData < 0) {
                compareData = 0;
            } else if (compareData > 255) {
                compareData = 255 ;
            }
            sharpedImage.set_data(i, j, compareData);
        }
    }
    return sharpedImage;
}