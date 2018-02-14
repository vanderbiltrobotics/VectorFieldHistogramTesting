//Contains global definitions for project

#ifndef VECTORFIELDHISTOGRAMTESTING_UTILS_H
#define VECTORFIELDHISTOGRAMTESTING_UTILS_H


//Continuous point for representing exact positions in meters
struct contPoint {
    double x;
    double y;
};

//Discrete Point for representing positions discretized into nodes on histogram grid
struct discretePoint {

    // discretePoint(int x = -99999, int y = -99999) : x{x}, y{y} {} // REVIEW

    int x;
    int y;
};

//Region for representing the active region of the histogram grid
//Stores two points corresponding to two corners of a rectangular region
struct region {
    discretePoint min;
    discretePoint max;
};

struct valley {
    int lowerBound;
    int upperBound;
    bool isWide;

};

#endif //VECTORFIELDHISTOGRAMTESTING_UTILS_H
