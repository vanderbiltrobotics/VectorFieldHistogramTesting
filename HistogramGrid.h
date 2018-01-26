//
// Created by Swapnil on 1/25/2018.
//

#ifndef VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H
#define VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H

#include "Utils.h"


class HistogramGrid {
private:
    int iMax; //Size in the i direction of the histogram grid
    int jMax; //Size in the j direction of the histogram grid

    double nodeWidth; //Width of each of the nodes in meters
    double nodeLength; //Length of each node in meters



    double** histGrid; //Histogram grid object. Stores the certainty values for all nodes in grid
    int** objectGrid; //Stores the type of object that exists at each node

public:
    //HistogramGrid
    //Creates a new histogram grid object with no objects present in the grid
    // int histWidth - Width of the entire histogram in meters
    // int histLength - Length of the entire histogram in meters
    // int iNum - Number of nodes in the i (width) direction
    // int jNum - Number of nodes in the j (length) direction
    HistogramGrid(int histWidth, int histLength, int iNum, int jNum):
            iMax(iNum>0?(iNum-1):0), jMax(jNum>0?(jNum-1):0), nodeWidth(histWidth/iMax), nodeLength(histLength/jMax)
    {
        //Initializing the histGrid and objectGrid
        histGrid = new double*[iMax];
        objectGrid = new int*[iMax];
        for(int i = 0; i < iMax; i++)
        {
            histGrid[i] = new double[jMax];
            objectGrid[i] = new int[jMax];
            for(int j = 0; j < jMax; j++)
            {
                histGrid[i][j] = 0;
                objectGrid[i][j] = 0;
            }
        }
    }

    //getDiscretePointFromCont
    //Calculates in which node an object exists based on the continuous (exact) coordinates
    //Returns a discrete point struct
    discretePoint getDiscretePointFromCont(contPoint pos)
    {
        discretePoint out;
        out.x = (int)((pos.x)/nodeWidth);
        out.y = (int)((pos.y)/nodeLength);
        if(out.x < iMax && out.y < jMax) return out;
        //TODO ERROR HANDLING
        throw;
    }

    void updateCertainty(contPoint pos, double certainty)
    {
        discretePoint objLoc = getDiscretePointFromCont(pos);
        histGrid[objLoc.x][objLoc.y] = certainty;
    }

    // getProbability
    // Returns the certainty of an object being present at the given node
    double getCertainty(int i, int j)
    {
        return histGrid[i][j];
    }


};

#endif //VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H
