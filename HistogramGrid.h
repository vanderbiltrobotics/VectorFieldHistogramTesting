/**
 * HistogramGrid.h
 * \author Swapnil Pande
 */

#ifndef VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H
#define VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include "Utils.h"


/**
 * \brief Represents the Histrogram Grid, which is a collection of obstacle likelihood
 *        scores. 
 *
 * When a sensor detects an obstacle that lies inside a square in the Histogram grid,
 * that square's "obstacle likelihood" value is incremented by 1. This likelihood value
 * affects the robot's willingness to traverse over it. 
 * 
 * \author Swapnil Pande
 */
class HistogramGrid {
private:
    int iMax;               ///< Size in the i direction of the histogram grid
    int jMax;               ///< Size in the j direction of the histogram grid

    double nodeSize;        ///< Side dimension of each node. Each node is a square

    double** histGrid;      ///< Histogram grid object. Stores the certainty values for all nodes in grid
    int** objectGrid;       ///< Stores the type of object that exists at each node

    int iSizeActiveRegion;  ///< i-dimension size of the active region measured in number of nodes
    int jSizeActiveRegion;  ///< j-dimension size of the active region

    discretePoint robotLoc; ///< The coordinates of the robot
    discretePoint target;   ///< The coordinates of the robot's destination

public:
    /**
     * Creates a new histogram grid object with no objects present in the grid
     * 
     * \param histWidth Width of the entire histogram in meters
     * \param histLength Length of the entire histogram in meters
     * \param nodeSideLen Side dimension of each node. histWidth and histLength 
     * should be a multiple of this number
     */
    HistogramGrid(int histWidth, int histLength, double nodeSideLen) :
            iMax(int(double(histWidth)/nodeSideLen) - 1),
            jMax(int(double(histLength)/nodeSideLen) - 1),
            nodeSize(nodeSideLen),
            histGrid(new double*[iMax]),
            objectGrid(new int*[iMax]),
            iSizeActiveRegion(10),
            jSizeActiveRegion(10) // why are these 10???
    {
        std::cout << "Initializing grid: iMax = " << iMax << ". jMax = " << jMax << "\n";
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

        // DEBUG!!!!
        // manually add obstacles
        objectGrid[iMax/2][jMax/2] = 1;
        objectGrid[iMax/2-1][jMax/2-1] = 1;
        objectGrid[iMax/2][jMax/2-1] = 1;
        objectGrid[iMax/2-1][jMax/2] = 1;
    }

    // DEBUG!!!
    // HistogramGrid
    // Alternate constructor used for reading grid from file. Used only for testing
    HistogramGrid(std::string fName, discretePoint robotLocIn)
    {
        std::cout << std::endl 
                  << std::endl
                  << "testing histogram grid initialization: "
                  << std::endl;

        robotLoc = robotLocIn;
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
            objectGrid = new int*[iMax];
            iSizeActiveRegion = 20;
            jSizeActiveRegion = 20;

            for(int i = 0; i < iMax; i++)
            {
                histGrid[i] = new double[jMax];
                objectGrid[i] = new int[jMax];
                for(int j = 0; j < jMax; j++)
                {
                    file >> data;
                    if(data == "1")
                    {
                        histGrid[i][j] = 1;
                        objectGrid[i][j] = 1;
                    }
                    else if(data == "2")
                    {
                        histGrid[i][j] = 0;
                        objectGrid[i][j] = 0;
                        target.x = i;
                        target.y = j;
                    }
                    else
                    {
                        histGrid[i][j] = 0;
                        objectGrid[i][j] = 0;
                    }
                    //std::cout << i << " " << j << " " << histGrid[i][j] << " " << objectGrid[i][j] << "\n";
                }
            }

            file.close();
        }

        else {
            std::cout << "Unable to open file";
        }
    }

    HistogramGrid(const HistogramGrid &rhs) : 
            iMax(rhs.iMax), 
            jMax(rhs.jMax), 
            nodeSize(rhs.nodeSize),
            histGrid(new double*[iMax]), 
            objectGrid(new int*[iMax]),
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


    const HistogramGrid& operator=(const HistogramGrid &rhs)
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


    // TODO: Consider making this private.  - Josh
    // TODO: Also, this assumes that obstacles are going to fall in exactly 
    //  one grid space. What if the obstacles had a radius and covered multiple
    //  grid spaces, so you wouldn't even need this method? You would just need
    //  one method that transforms your collection of obstacles into an occupied
    //  grid.  - Josh
    //getDiscretePointFromCont
    //Calculates the cell in which an object lies based on its continuous (exact) coordinates
    //Returns a discrete point struct
    discretePoint getDiscretePointFromCont(const contPoint& pos) const
    {
        discretePoint out;
        out.x = int(pos.x / nodeSize);
        out.y = int(pos.y / nodeSize);
        //if(out.x < iMax && out.y < jMax) return out;
        //TODO: Handle out-of-bounds arguments
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

    // getCertainty
    // Returns the certainty of an object being present at the given node
    double getCertainty(discretePoint pos) const
    {
        return histGrid[pos.x][pos.y];
    }

    // Consider making private.  - Josh
    //getDistance
    //Returns scalar distance between two discretePoints (pos1 & pos2) on the histogram grid
    double getDistance(discretePoint pos1, discretePoint pos2) const
    {
        return sqrt(pow(double(pos2.x - pos1.x), 2) + pow(double(pos2.y - pos1.y), 2));
    }

    // Private?
    // getAngle
    // Returns the angle between the line between pos2 and posRef and the horizontal along
    //  positive i direction.
    double getAngle(discretePoint posRef, discretePoint pos2) const
    {
        double out = atan2(double(pos2.y) - posRef.y, double(pos2.x) - posRef.x)*180/M_PI;
        if(out < 0) out += 360;
        return out;
    }


    region getActiveRegion() const
    {
        region activeRegion;

        activeRegion.min.x = (robotLoc.x - iSizeActiveRegion / 2);
        if(activeRegion.min.x < 0)
        {
            activeRegion.min.x = 0;
        }

        activeRegion.min.y = (robotLoc.y - jSizeActiveRegion / 2);
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

    // TODO: Consider making this return a 'continuous' point.  - Josh
    //getRobotLoc
    //Returns a discretePoint describing the current position of the robot
    discretePoint getRobotLoc() const
    {
        return robotLoc;
    }

    // TODO: See above. 
    // getTargetLoc
    // Returns a discretePoint describing the current position of the target
    discretePoint getTargetLoc() const
    {
        return target;
    }

    // TODO: See above.
    //setTargetLoc
    //Returns a discretePoint describing the current position of the target
    void setTargetLoc(discretePoint targetLoc)
    {
        target = targetLoc;
    }

    // TODO: See above.
    void setRobotLoc(discretePoint robotLocIn)
    {
        robotLoc = robotLocIn;
    }

    int** getObjectGrid() const
    {
        return objectGrid;
    }

    int getCellValue(int i, int j) const
    {
        return objectGrid[i][j];
    }

    int getIMax() const
    {
      return iMax;
    }

    int getJMax() const
    {
      return jMax;
    }
};

#endif //VECTORFIELDHISTOGRAMTESTING_HISTOGRAMGRID_H