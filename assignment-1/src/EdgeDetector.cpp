// EdgeDetector.cpp
#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() {
    kernel_height = 3;
    kernel_width = 3;
    Gx = new double*[3];
    for (int i = 0; i < kernel_height; ++i) {
        Gx[i] = new double[kernel_width];
    }
    Gx[0][0] = -1;
    Gx[0][1] = 0;
    Gx[0][2] = 1;
    Gx[1][0] = -2;
    Gx[1][1] = 0;
    Gx[1][2] = 2;
    Gx[2][0] = -1;
    Gx[2][1] = 0;
    Gx[2][2] = 1;

    Gy = new double*[3];
    for (int i = 0; i < kernel_height; ++i) {
        Gy[i] = new double[kernel_width];
    }
    Gy[0][0] = -1;
    Gy[0][1] = -2;
    Gy[0][2] = -1;
    Gy[1][0] = 0;
    Gy[1][1] = 0;
    Gy[1][2] = 0;
    Gy[2][0] = 1;
    Gy[2][1] = 2;
    Gy[2][2] = 1;
}

// Destructor
EdgeDetector::~EdgeDetector() {
    for (int i = 0; i < kernel_height; ++i) {
        delete[] Gx[i];
        delete[] Gy[i];
    }
    delete[] Gx;
    delete[] Gy;
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    Convolution convForGx(Gx,3,3,1, true);
    Convolution convForGy(Gy,3,3,1, true);
    ImageMatrix Ix = convForGx.convolve(input_image);
    ImageMatrix Iy = convForGy.convolve(input_image);



    double** gradiant = new double * [Ix.get_height()];
    for (int i = 0; i < Ix.get_height(); ++i) {
        gradiant[i] = new double[Ix.get_width()];
    }
    double sumOfPixels = 0;
    for (int i = 0;i<Ix.get_height(); i++){
        for (int j = 0;j< Iy.get_width(); j++){
            double G = std::sqrt((Ix.get_data(i, j) * Ix.get_data(i, j)) + (Iy.get_data(i, j) * Iy.get_data(i, j)));
            gradiant[i][j] = G;
            sumOfPixels += G;
        }
    }
    double threshold = sumOfPixels / (input_image.get_height() * input_image.get_width());
    std::vector<std::pair<int, int>> edge;
    for (int i = 0;i<input_image.get_height(); i++){
        for(int j = 0;j< input_image.get_width(); j++){
            if (threshold < gradiant[i][j]){
                edge.emplace_back(i,j);
            }
        }
    }
    for (int i = 0;i<Ix.get_height(); i++){
        delete[] gradiant[i];
    }
    delete[] gradiant;

    return edge;

}

