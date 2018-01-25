//
// Created by Swapnil on 1/25/2018.
//

#ifndef VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H
#define VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H

class HistogramGrid {
private:
    int width; //Width of the matrix
    int height; //Height of the matrix
    double** histGrid; //Matrix object to store all objects

public:
    HistogramGrid(int w, int h) : width(w), height(h)
    {
        histGrid = new double*[width];
        for(int i = 0; i < width; i++)
        {
            histGrid[i] = new double[height];
            for(int j = 0; j < height; j++)
            {
                histGrid[i][j] = 0;
            }
        }
    }

    double getProbability(int i, int j)
    {
        return histGrid[i][j];
    }
};

#endif //VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H
