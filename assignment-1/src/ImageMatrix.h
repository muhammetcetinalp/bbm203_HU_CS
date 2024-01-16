#ifndef IMAGE_MATRIX_H
#define IMAGE_MATRIX_H

#include <string>

#include "ImageLoader.h"

class ImageMatrix {
public:
    // Constructors and Destructor
    ImageMatrix(); // Default constructor
    ImageMatrix(const std::string &filepath); // Parameterized constructor for loading image from file
    ImageMatrix(int imgHeight, int imgWidth); // Constructor for creating a blank image of given size
    ImageMatrix(const double ** inputMatrix, int imgHeight, int imgWidth); // Parameterized constructor - direct initialization with 2D matrix
    ImageMatrix(const ImageMatrix &other); // Copy constructor
    ImageMatrix& operator=(const ImageMatrix &other); // Copy assignment operator
    ~ImageMatrix(); // Destructor

    // Overloaded operators
    ImageMatrix operator+(const ImageMatrix &other) const;
    ImageMatrix operator-(const ImageMatrix &other) const;
    ImageMatrix operator*(const double &scalar) const; // Scalar multiplication

    void set_data(int i, int j, double value)const;
    double get_data(int i, int j) const;
    double** get_data()const;
    int getLSB(int x, int y)const;
    int get_height() const;
    int get_width() const;

public:
    double** data;
    int height;
    int width;


};

#endif // IMAGE_MATRIX_H
