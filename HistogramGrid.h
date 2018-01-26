//
// Created by Swapnil on 1/25/2018.
//

#ifndef VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H
#define VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H

#include <cmath>
#include "Utils.h"

//TODO Implement Assignment operator, copy constructor, and destructors
class HistogramGrid {
private:
    int iMax; //Size in the i direction of the histogram grid
    int jMax; //Size in the j direction of the histogram grid

    double nodeSize; //Side dimension of each node. Assumes that each node is square


    double** histGrid; //Histogram grid object. Stores the certainty values for all nodes in grid
    int** objectGrid; //Stores the type of object that exists at each node

public:
    //HistogramGrid
    //Creates a new histogram grid object with no objects present in the grid
    // int histWidth - Width of the entire histogram in meters
    // int histLength - Length of the entire histogram in meters
    // int nodeSideLen - Side dimension of each node. histWidth and histLength should be divisible by this number
    HistogramGrid(int histWidth, int histLength, double nodeSideLen):
            iMax((int)(histWidth/nodeSideLen)), jMax((int)(histLength/nodeSideLen)), nodeSize(nodeSideLen)
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
        out.x = (int)((pos.x)/nodeSize);
        out.y = (int)((pos.y)/nodeSize);
        if(out.x < iMax && out.y < jMax) return out;
        //TODO ERROR HANDLING
        throw;
    }

    //updateCertainty
    //Updates the certainty value for the node at which the object is located
    //contPoint - the continuous point at which object is located
    //certainty - certainty value to set
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

    //getDistance
    //Returns scalar distance between two discretePoints (pos1 & pos2) on the histogram grid
    double getDistance(discretePoint pos1, discretePoint pos2)
    {
        return sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2));
    }
};

#endif //VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H
