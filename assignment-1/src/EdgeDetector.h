#ifndef EDGE_DETECTOR_H
#define EDGE_DETECTOR_H

#include "ImageMatrix.h"
#include "Convolution.h"
#include <vector>

class EdgeDetector {
    public:
        EdgeDetector();
        ~EdgeDetector();

        std::vector<std::pair<int, int>> detectEdges(const ImageMatrix& input_image);
        bool isPrime(int value);
        std::vector<int> fibo(int n);

    private:
        double** Gx;
        double** Gy;
        int kernel_height;
        int kernel_width;
};

#endif // EDGE_DETECTOR_H


