//
// Created by Swapnil on 2/3/2018.
//

#ifndef VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H
#define VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H

#include <iostream>
#include <vector>
#include <thread>


#include "PolarHistogram.h"
#include "HistogramGrid.h"


class VFHPather{
private:

    PolarHistogram hist; //Object used to store the polar histogram
    HistogramGrid* grid; //Object used to store the grid/map of obstacles


    double a; //Constants for the weighting function for histogram generation
    double b;

    int l; //Smoothing constant for polar histogram

    int smax; //Maximum number of nodes that define a wide valley

    double valleyThreshold;

public:
    //Default Constructor, implements testing mode of Pather with hard-coded default values
    VFHPather(): hist(true), grid(new HistogramGrid(10, 10, 0.1)), a(50), b(100), l(5), valleyThreshold(15)
    {}

    //Alternate constructor takes all parameters
    //histIn - PolarHistogram object to store the histogram
    //gridIn - HistogramGrid object to represent object density
    //aIn - Shifting constant for weighting polar histogram
    //bIn - Scaling constant for weighting polar histogram
    //lIn - Smoothing algorithm constant - determines the number of bins included in smoothing
    //valleyThreshold - Threshold for determining whether bin should be included in valley
    //                  If polar object density falls below valley threshold, bin is considered to be part of valley
    VFHPather(PolarHistogram &histIn, HistogramGrid *gridIn, double aIn, double bIn,
              double lIn, double valleyThresholdIn): hist(histIn), grid(gridIn), a(aIn), b(bIn), l(lIn),
                                                     smax(10), valleyThreshold(valleyThresholdIn) {}
    //TODO: Add ability to dynamically set certainty value
    //TODO This function may be deprecated as we restructure the robot code for ROSMOD
    //updateRobotPosition
    void updateRobotPosition(discretePoint pos)
    {
        (*grid).setRobotLoc(pos);
    }


    //generateHistogram
    //Builds the vector field histogram based on current position of robot and surrounding obstacles
    void generateHistogram()
    {


        hist.reset(); //Resetting the histogram

        region activeRegion =  (*grid).getActiveRegion();

        discretePoint curNode; //Node currently being iterated over

        for(curNode.x = activeRegion.min.x; curNode.x < activeRegion.max.x; curNode.x++)
        {
            for(curNode.y = activeRegion.min.y; curNode.y < activeRegion.max.y; curNode.y++)
            {
                double val = pow( (*grid).getCertainty(curNode),2)*(a-b* (*grid).getDistance(curNode,  (*grid).getRobotLoc()));
                hist.addValue( (*grid).getAngle( (*grid).getRobotLoc(), curNode), val);
                //std::cout << curNode.x << " " << curNode.y << " " << val << "\n";

                (*grid).getCertainty(curNode);
                    //std::cout << curNode.x << " " << curNode.y << " " << pow( (*grid).getCertainty(curNode),2)*(a-b* (*grid).getDistance(curNode,  (*grid).getRobotLoc())) << "\n";
            }
        }
        //std::cout << "End Histogram Generation\n";

        std::cout << "\n";

    }



    //computeTravelDirection
    //Determines the optimal travel direction for the robot based on the generated polar histogram
    //Works by finding the valley whose direction most closely matches the direction of the target
    double computeTravelDirection()
    {

        generateHistogram(); //Computes the polar histogram
        printHistogram();

        //hist.smoothHistogram(l); //Smoothing histogram

        //startBin represent bin at which valley finding starts
        int startBin = hist.getBinFromAngle((*grid).getAngle((*grid).getRobotLoc(), grid->getTargetLoc())); //Determine the bin in which the target falls

        //EDGE CASE: Determining if the target direction falls within a bin
        //This handled by finding the edges of the valleys
        if(hist.getValue(startBin) < valleyThreshold) //Desired travel direction falls within a valley
        {
            std::cout << "\n\n\nREACHED EDGE CASE\n\n\n";
            int leftIndex = -1, rightIndex = -1; //Store the indices of the edges of the valley

            int negative = 1; //Used to the flip the direction of search
            int divide = 2; //Used to control the direction search occurs

            int count = 1; //Counter to store number of bins tested
            int i; //Stores index to search
            //Iterating over the histogram to find valley.
            //Iteration occurs alternating in left & right direction of target
            while(count <= hist.getNumBins() && count <= smax)
            {

                i = startBin + negative * count / divide; //Index of bin to check next

                if(hist.getValue(i) > valleyThreshold)
                {
                    if(negative == 1) rightIndex = i + 1;
                    else leftIndex = i;

                    //One bound of index is found. Only need to search in other direction
                    negative *= -1;
                    divide = 1;
                }

                //Flipping search direction if both ends of the valley have not been found
                if(leftIndex == -1 && rightIndex == -1) negative *= -1;

                count++; //Increment counter
            }
            if(count > smax) //The maximum size of a valley was reached. Write the edges at the last searched bins
            {
                //Stores the edges of the valley if the size has reached smax.
                //If divide is 2, neither edge of the boundary has been found.
                //If divide is 1, one edge of the boundary has been found.
                for(int j = 0; j < divide; j++)
                {
                    count--;
                    i = startBin + negative * count / divide; //Index of bin to check next

                    if(negative == 1) rightIndex = i + 1; //Storing edge of valley
                    else leftIndex = i;
                }
            }
            std::cout << leftIndex << " " << rightIndex << "\n";
            //Returns the average value of the bins
            return hist.getAngleFromBin((rightIndex+leftIndex)/2);
        }
        else //Not edge case, process histogram normally
        {
            int rightTravelDir = -1, leftTravelDir = -1; //Travel direction for valleys nearest to target on left and right of target
            //Finding nearest left and right valleys
            //Looping over histogram to left of target, then to the right of target
            //Stores the suggested travel direction for each of side of the histogram and selects direction closest to target
            int i;
            //Checking left side
            std::cout << "\n Left Side parameters: \n";
            for (i = startBin + 1; i <= hist.getNumBins()/2; i++) {
                if (hist.getValue(i) < valleyThreshold) //Found valley
                {
                    int rightIndex = i;
                    //Iterating over valley to find other edge of the valley.
                    while(hist.getValue(i) < valleyThreshold && abs(i - rightIndex) < smax) i++;
                    int leftIndex = i;
                    leftTravelDir = (rightIndex + leftIndex)/2;
                    std::cout << hist.getIndex(leftIndex) << " " << hist.getIndex(rightIndex) << " " << hist.getIndex(leftTravelDir) << "\n";
                    break; //Since loop begins iterating from the target direction, the valley must be the closest valley
                }
            }

            if(i < hist.getNumBins()/2) i = hist.getNumBins() - hist.getNumBins()/2; //setting max iteration for left side
            i -= hist.getNumBins();
            std::cout << "\ni: " <<  i << " " << hist.getIndex(i) << std::endl;
            int j;
            //Checking right side
            std::cout << "\n Right Side parameters: \n";
            for (j = startBin - 1; j > i; j--) {
                std::cout << "\nj: " << j << " " << i << " " << hist.getValue(j) << "\n";
                if (hist.getValue(j) < valleyThreshold) //Found valley
                {
                    int rightIndex = j+1;
                    //Iterating over valley to find other edge of the valley.
                    while(hist.getValue(j) < valleyThreshold && abs(j-rightIndex)  < smax)
                    {
                        j--;
                    }
                    int leftIndex = j+1;
                    leftTravelDir = (rightIndex + leftIndex)/2;
                    std::cout << hist.getIndex(leftIndex) << " " << hist.getIndex(rightIndex) << " " << hist.getIndex(rightTravelDir) << "\n";
                    break; //Since loop begins iterating from the target direction, the valley must be the closest valley
                }
            }
            std::cout << "\nTarget Direction: " << startBin << "\n";
            if(abs(rightTravelDir-startBin) < abs(leftTravelDir-startBin)) {
                std::cout << "\nSelected Direction: " << hist.getIndex(rightTravelDir) << "\n";
                return hist.getAngleFromBin(rightTravelDir);
            }
            else {
                std::cout << "\nSelected Direction: " << hist.getIndex(leftTravelDir) << "\n";
                return hist.getAngleFromBin(leftTravelDir);
            }
        }
    }

    void printHistogram()
    {
        hist.printHistogram();
    }

    int** getObjectGrid()
    {
      return (*grid).getObjectGrid();
    }

    int getCellValue(int i, int j)
    {
        return (*grid).getCellValue(i, j);
    }

    int getIMax()
    {
        return (*grid).getIMax();
    }

    int getJMax()
    {
        return (*grid).getJMax();
    }
};
#endif //VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H
