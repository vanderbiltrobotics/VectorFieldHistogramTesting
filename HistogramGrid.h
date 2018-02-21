//
// Created by Swapnil on 1/25/2018.
//

#ifndef VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H
#define VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include "Utils.h"

class HistogramGrid {
private:
    int iMax; //Size in the i direction of the histogram grid
    int jMax; //Size in the j direction of the histogram grid

    double nodeSize; //Side dimension of each node. Assumes that each node is square

    double** histGrid; //Histogram grid object. Stores the certainty values for all nodes in grid
    int** objectGrid; //Stores the type of object that exists at each node

    int iSizeActiveRegion; //i-dimension size of the active region measured in number of nodes
    int jSizeActiveRegion; //j-dimension size of the active region measured in number of nodes

    discretePoint robotLoc; //Stores the location of the robot
    discretePoint target;

public:
    //HistogramGrid
    //Creates a new histogram grid object with no objects present in the grid
    // int histWidth - Width of the entire histogram in meters
    // int histLength - Length of the entire histogram in meters
    // int nodeSideLen - Side dimension of each node. histWidth and histLength should be divisible by this number
    HistogramGrid(int histWidth, int histLength, double nodeSideLen):
            iMax((int)(histWidth/nodeSideLen)), jMax((int)(histLength/nodeSideLen)),
            nodeSize(nodeSideLen), histGrid(new double*[iMax]), objectGrid(new int*[jMax]),
            iSizeActiveRegion(20), jSizeActiveRegion(20)
    {
        //Initializing the histGrid and objectGrid
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

    //HistogramGrid
    //Alternate constructor used for ingesting grid from file. Used only for testing
    HistogramGrid(std::string fName)
    {
        std::string data; //Temporary string to store ingested data
        int histWidth;
        int histLength;

        std::ifstream file(fName);
        if (file.is_open())
        {
            file >> data;
            histWidth = std::stoi(data);

            file >> data;
            histLength = std::stoi(data);

            file >> data;
            nodeSize = std::stod(data);

            iMax = (int)(histWidth/nodeSize);
            jMax = (int)(histLength/nodeSize);
            histGrid = new double*[iMax];
            objectGrid = new int*[jMax];
            iSizeActiveRegion = 20;
            jSizeActiveRegion = 20;

            for(int i = 0; i < iMax; i++)
            {
                histGrid[i] = new double[jMax];
                objectGrid[i] = new int[jMax];
                for(int j = 0; j < jMax; j++)
                {
                    file >> data;
                    if(data != "0")
                    {
                        histGrid[i][j] = 1;
                        objectGrid[i][j] = std::stoi(data);
                    }
                    else
                    {
                        histGrid[i][j] = 0;
                        objectGrid[i][j] = 0;
                    }
                }
            }

            file.close();
        }

        else std::cout << "Unable to open file";
    }

    ~HistogramGrid()
    {
        for(int i = 0; i < iMax;i++)
        {
            delete[] histGrid[i];
            histGrid[i] = nullptr;

            delete[] objectGrid[i];
            objectGrid[i] = nullptr;
        }
        delete[] histGrid;
        delete[] objectGrid;
        histGrid = nullptr;
        objectGrid = nullptr;
    }


    const HistogramGrid& operator= (const HistogramGrid &rhs)
    {
        if(this == &rhs)
        {
            return *this;
        }
        HistogramGrid tmp(rhs); //make a copy of the right hand side via copy constructor

        //next swap the data between *this and tmp
        std::swap(iMax, tmp.iMax);
        std::swap(jMax, tmp.jMax);

        std::swap(nodeSize, tmp.nodeSize);

        std::swap(histGrid, tmp.histGrid);
        std::swap(objectGrid, tmp.objectGrid);

        std::swap(iSizeActiveRegion, tmp.iSizeActiveRegion);
        std::swap(jSizeActiveRegion, tmp.jSizeActiveRegion);

        return *this;
    }

    HistogramGrid(const HistogramGrid &rhs):iMax(rhs.iMax), jMax(rhs.jMax), nodeSize(rhs.nodeSize),
                                                           histGrid(new double*[iMax]), objectGrid(new int*[jMax]),
                                                           iSizeActiveRegion(rhs.iSizeActiveRegion),
                                                           jSizeActiveRegion(rhs.jSizeActiveRegion)
    {
        //Initializing the histGrid and objectGrid
        for(int i = 0; i < iMax; i++)
        {
            histGrid[i] = new double[jMax];
            objectGrid[i] = new int[jMax];
            for(int j = 0; j < jMax; j++)
            {
                histGrid[i][j] = rhs.histGrid[i][j];
                objectGrid[i][j] = rhs.objectGrid[i][j];
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
        //if(out.x < iMax && out.y < jMax) return out;
        //TODO ERROR HANDLING
        //throw;
        return out;
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
    double getCertainty(discretePoint pos)
    {
        return histGrid[pos.x][pos.y];
    }

    //getDistance
    //Returns scalar distance between two discretePoints (pos1 & pos2) on the histogram grid
    double getDistance(discretePoint pos1, discretePoint pos2)
    {
        return sqrt(pow((double)pos2.x - pos1.x, 2) + pow((double)pos2.y - pos1.y, 2));
    }

    //getAngle
    //Returns the angle between the line between pos2 and posRef and the horizontal along positive i direction.
    double getAngle(discretePoint posRef, discretePoint pos2)
    {
        double out = atan2((double)pos2.y - posRef.y, (double)pos2.x - posRef.x)*180/M_PI;
        if(out < 0) out += 360;
        return out;
    }

    region getActiveRegion()
    {
        region activeRegion;

        activeRegion.min.x = (robotLoc.x - iSizeActiveRegion/2);
        if(activeRegion.min.x < 0)
        {
            activeRegion.min.x = 0;
        }

        activeRegion.min.y = (robotLoc.y - jSizeActiveRegion/2);
        if(activeRegion.min.y < 0)
        {
            activeRegion.min.y = 0;
        }

        activeRegion.max.x = (robotLoc.x + (iSizeActiveRegion - iSizeActiveRegion/2));
        if(activeRegion.max.x >= iMax)
        {
            activeRegion.max.x = iMax;
        }

        activeRegion.max.y = (robotLoc.x + (jSizeActiveRegion - jSizeActiveRegion/2));
        if(activeRegion.max.y >= jMax)
        {
            activeRegion.max.y = jMax;
        }

        return activeRegion;
    }

    //getRobotLoc
    //Returns a discretePoint describing the current position of the robot
    discretePoint getRobotLoc()
    {
        return robotLoc;
    }

    //getTargetLoc
    //Returns a discretePoint describing the current position of the target
    discretePoint getTargetLoc()
    {
        return target;
    }

    //setTargetLoc
    //Returns a discretePoint describing the current position of the target
    void setTargetLoc(discretePoint targetLoc)
    {
        target = targetLoc;
    }

    void setRobotLoc(discretePoint robotLocIn)
    {
        robotLoc = robotLocIn;
    }

    void printGrid()
    {
        for(int i = 0; i < iMax; i++) {
            for (int j = 0; j < jMax; j++) {
                std::cout << histGrid[i][j];
            }
            std::cout << '\n';
        }
    }

};

#endif //VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H
