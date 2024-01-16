#include "Convolution.h"
#include "ImageMatrix.h"

// Default constructor
Convolution::Convolution() {
    kernelHeight = 3;
    kernelWidth = 3;
    customKernel = nullptr;
    stride = 1;
    padding = true;

}
// Parametrized constructor for custom customKernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad){
    this->customKernel = new double*[kh];
    for (int i = 0; i < kh; ++i) {
        this->customKernel[i] = new double[kw];
    }
    for (int i = 0; i < kh; ++i) {
        for (int j = 0; j < kw; j++) {
            this->customKernel[i][j] = customKernel[i][j];
        }
    }
    this->kernelHeight = kh;
    this->kernelWidth = kw;
    this->stride = stride_val;
    this->padding = pad;


}

// Copy constructor
Convolution::Convolution(const Convolution &other) {
    this->stride = other.stride;
    this->padding = other.padding;
    this->kernelHeight = other.kernelHeight;
    this->kernelWidth = other.kernelWidth;

    this->customKernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        this->customKernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            this->customKernel[i][j] = other.customKernel[i][j];
        }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other) {
        return *this;
    }
    this->stride = other.stride;
    this->padding = other.padding;
    this->kernelHeight = other.kernelHeight;
    this->kernelWidth = other.kernelWidth;

    this->customKernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        this->customKernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            this->customKernel[i][j] = other.customKernel[i][j];
        }
    }

    return *this;
}
// Destructor
Convolution::~Convolution() {
    if (customKernel != nullptr) {
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] customKernel[i];
        }
        delete[] customKernel;
    }
}

// Convolve Function: Responsible for convolving the input image with a customKernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const{
    int outputWidth = static_cast<int>((input_image.get_width() - kernelWidth + 2 * padding) / stride) + 1;
    int outputHeight = static_cast<int>((input_image.get_height() - kernelHeight + 2 * padding) / stride) + 1;

    if (padding){
        ImageMatrix paddedMatrix(outputHeight + 2, outputWidth + 2);
        ImageMatrix temporaryMatrix(outputHeight, outputWidth);
        for (int i =0; i < outputHeight + 2; i++){
            for (int j =0;j<outputWidth+2;j++){
                if(i ==0 || j == 0 || i == outputHeight + 1 || j == outputWidth + 1){
                    paddedMatrix.set_data(i,j,0);
                }
                else{
                    paddedMatrix.set_data(i,j,input_image.get_data(i-1,j-1));
                }
            }
        }
        for (int i = 1; i < outputHeight + 1; ++i){
            for (int j = 1;j<outputWidth+1;++j){
                double sum = 0;
                for (int l = -1;l <kernelHeight-1;++l){
                    for(int m = -1;m<kernelWidth-1;++m){
                        double s = paddedMatrix.get_data(i+l,j+m);
                        double r = customKernel[l+1][m+1];
                        sum += s * r;
                    }
                }
                temporaryMatrix.set_data(i-1,j-1,sum);

            }
        }
        return temporaryMatrix;
    }
    else {
        ImageMatrix unPaddedMatrix(outputHeight, outputWidth);
        for (int i = 0; i < outputHeight; ++i){
            for (int j = 0;j<outputWidth;++j){
                double sum = 0;
                for (int l = 0;l <kernelHeight;++l){
                    for(int m = 0;m<kernelWidth;++m){
                        double s = input_image.get_data(i+l,j+m);
                        double r = customKernel[l][m];
                        sum += s * r;
                    }
                }
                unPaddedMatrix.set_data(i,j,sum);

            }
        }
        return unPaddedMatrix;

    }

}

