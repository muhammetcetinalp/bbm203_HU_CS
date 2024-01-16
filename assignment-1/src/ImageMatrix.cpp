#include "ImageMatrix.h"


// Default constructor
ImageMatrix::ImageMatrix() {
    data = nullptr;
    height = 0;
    width = 0;
}
// Parameterized constructor for loading image from file. PROVIDED FOR YOUR CONVENIENCE
ImageMatrix::ImageMatrix(const std::string &filepath) {
    // Create an ImageLoader object and load the image
    ImageLoader imageLoader(filepath);

    // Get the dimensions of the loaded image
    height = imageLoader.getHeight();
    width = imageLoader.getWidth();

    // Allocate memory for the matrix
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
    }

    // Copy data from imageLoader to data
    double** imageData = imageLoader.getImageData();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            data[i][j] = imageData[i][j];
        }
    }
}
// Parameterized constructor for creating a blank image of given size
ImageMatrix::ImageMatrix(int imgHeight, int imgWidth) {
    height = imgHeight;
    width = imgWidth;
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
        for (int j = 0; j < width; j++) {
            data[i][j] = 0;
        }
    }
}

ImageMatrix::ImageMatrix(const double **inputMatrix, int imgHeight, int imgWidth) {
    height = imgHeight;
    width = imgWidth;
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
        for (int j = 0; j < width; j++) {
            data[i][j] = inputMatrix[i][j];
        }
    }
}

// Copy constructor
ImageMatrix::ImageMatrix(const ImageMatrix &other) {
    height = other.height;
    width = other.width;

    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
        for (int j = 0; j < width; j++) {
            data[i][j] = other.get_data(i, j);
        }
    }

}

// Copy assignment operator
ImageMatrix& ImageMatrix::operator=(const ImageMatrix &other) {
    if (this == &other) {
        return *this;
    }
    height = other.height;
    width = other.width;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            data[i][j] = other.get_data(i, j);
        }
    }
    return *this;
}

// Destructor
ImageMatrix::~ImageMatrix() {
    // Deallocate memory
    if (data != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
}



// Overloaded operators

// Overloaded operator + to add two matrices
ImageMatrix ImageMatrix::operator+(const ImageMatrix &other) const {
    ImageMatrix result(*this);
    for (int i = 0; i < result.get_height(); i++) {
        for (int j = 0; j < result.get_width(); j++) {
            result.data[i][j] += other.get_data(i, j);
        }
    }
    return result;
}

// Overloaded operator - to subtract two matrices
ImageMatrix ImageMatrix::operator-(const ImageMatrix &other) const {
    ImageMatrix result(*this);
    for (int i = 0; i < result.get_height(); i++) {
        for (int j = 0; j < result.get_width(); j++) {
            result.data[i][j] -= other.get_data(i, j);
        }
    }
    return result;

}

// Overloaded operator * to multiply a matrix with a scalar
ImageMatrix ImageMatrix::operator*(const double &scalar) const {
    ImageMatrix result(*this);
    for (int i = 0; i < result.get_height(); i++) {
        for (int j = 0; j < result.get_width(); j++) {
            result.data[i][j] *=scalar;
        }
    }
    return result;

}

int ImageMatrix::get_height() const {
    return ImageMatrix::height;
}


int ImageMatrix::get_width() const {
    return ImageMatrix::width;
}

double ImageMatrix::get_data(int i, int j)const{
    return ImageMatrix::data[i][j];
}
double** ImageMatrix::get_data() const{
    return ImageMatrix::data;
}
void ImageMatrix::set_data(int i, int j, double value)const{
    ImageMatrix::data[i][j] = value;
}

int ImageMatrix::getLSB(int x, int y) const{
    int pixel_value = static_cast<int>(data[x][y]);
    int lsb = pixel_value & 1;
    return lsb;
}





