/**
 * VFHPather.h
 *
 * Implements the PolarHistogram and the HistogramGrid in the way that the paper by 
 * J. Borenstein and Y. Koren discuss in the paper "The Vector Field Histogram, et al."
 * See <URL> for more information.
 * 
 * @author Swapnil Pande
 * @author Joshua Petrin
 */

#ifndef VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H
#define VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H

#include <iostream>
#include <vector>
#include <thread>


#include "PolarHistogram.h"
#include "HistogramGrid.h"


class VFHPather {
private:

    PolarHistogram polarHist;  ///< The polar histogram
    HistogramGrid* grid;  ///< The grid of obstacle likelihood


    double a;  ///< Constant for histogram generating function
    double b;  ///< Constant for histogram generating function

    int l;  ///< Smoothing constant for polar histogram

    int smax;  ///< Maximum number of nodes that define a wide valley

    int valleyThreshold;  ///< Threshold for deciding if bin is a 'valley' (see paper)

public:
    /// Default constructor. Initializes a 10x10 (0.1) HistogramGrid, a PolarHistogram
    /// with 10-degree resolution, a = 50, b = 100, smoothing constant = 5, 
    /// consec. threshold for wide valleys = 5, and valley score threshold = 15.
    VFHPather() :
            polarHist(36),
            grid(new HistogramGrid(10, 10, 0.1)),
            a(50.0),
            b(100.0),
            l(5),
            smax(5),
            valleyThreshold(15)
    {
    }

    //Alternate constructor takes all parameters
    //histIn - PolarHistogram object to store the histogram
    //gridIn - HistogramGrid object to represent object density
    //aIn - Shifting constant for weighting polar histogram
    //bIn - Scaling constant for weighting polar histogram
    //lIn - Smoothing algorithm constant - determines the number of bins included in smoothing
    //valleyThreshold - Threshold for determining whether bin should be included in valley
    //                  If polar object density falls below valley threshold, bin is considered to be part of valley
    VFHPather(
        PolarHistogram &histIn, 
        HistogramGrid *gridIn, 
        double aIn, 
        double bIn,
        int lIn, 
        int valleyThresholdIn) : 
            polarHist(histIn), 
            grid(gridIn), 
            a(aIn), 
            b(bIn), 
            l(lIn),
            smax(5),  // should probably be a function of bin width
            valleyThreshold(valleyThresholdIn) 
    {
    }


    //TODO: Add ability to dynamically set certainty value
    //TODO: This function may be deprecated as we restructure the robot code for ROSMOD
    //updateRobotPosition
    void updateRobotPosition(discretePoint pos)
    {
        grid->setRobotLoc(pos);
    }


    //generateHistogram
    //Builds the vector field histogram based on current position of robot and surrounding obstacles
    void generateHistogram()
    {
        polarHist.reset(); //Resetting the histogram

        region activeRegion =  grid->getActiveRegion();

        discretePoint curNode; //Node currently being iterated over

        // std::cout << "Active Region: " << activeRegion.min.x << " "
                  // << activeRegion.min.y << " " << activeRegion.max.x << " " << activeRegion.max.y << "\n";
        //std::cout << "Histogram generation: \n";

        for(curNode.x = activeRegion.min.x; curNode.x < activeRegion.max.x; curNode.x++)
        {
            for(curNode.y = activeRegion.min.y; curNode.y < activeRegion.max.y; curNode.y++)
            {
                double val = pow( grid->getCertainty(curNode), 2) * (a - b*grid->getDistance(curNode, grid->getRobotLoc()));
                polarHist.addValue( grid->getAngle( grid->getRobotLoc(), curNode), val);
                //std::cout << curNode.x << " " << curNode.y << " " << val << "\n";

                grid->getCertainty(curNode);
                    //std::cout << curNode.x << " " << curNode.y << " " << pow( grid->getCertainty(curNode),2)*(a-b* grid->getDistance(curNode,  grid->getRobotLoc())) << "\n";
            }
        }
        //std::cout << "End Histogram Generation\n";
        std::cout << "\n";
    }

    //getIndex
    //Returns index 0 <= i < n that corresponds to a. "Wraps" a.
    int getIndex(int a, int n) {
        return ((a % n) + n) % n;
    }

    //computeTravelDirection
    //Determines the optimal travel direction for the robot based on the generated polar histogram
    //Works by finding the valley whose direction most closely matches the direction of the target
    double computeTravelDirection()
    {
        std::cout << "TEST 0 --------------\n";

        generateHistogram(); //Computes the polar histogram
        printHistogram();
        //hist.smoothHistogram(l); //Smoothing histogram
        std::cout << "TEST 0.5 --------------\n";


        //startBin represent bin at which valley finding starts
        int startBin = polarHist.getBinFromAngle(grid->getAngle(grid->getRobotLoc(), grid->getTargetLoc())); //Determine the bin in which the target falls

        int negative = 1; //Used to alternate the direction of the array iteration
        int nearIndex = -1; //Index of the edge of valley closest to the target
        int farIndex = -1; //Index of the edge of the valley furthest from target
        //Determining if the target direction falls within a bin
        if(polarHist.getBinValue(startBin) < valleyThreshold) //Desired travel direction falls within a valley
        {
            std::cout << "TEST 1 --------------\n";
            //Find upper boundary of valley
            for(int i = startBin + 1; getIndex(i, polarHist.getNumBins()) != startBin; i++)
            {
                //std::cout << getIndex(i, hist.getNumBins()) << " ";
                if(polarHist.getBinValue(getIndex(i, polarHist.getNumBins())) > valleyThreshold)
                {
                    farIndex = i; //Found the further edge of the valley
                    break;
                }
            }
            std::cout << "\n";
            std::cout << "TEST 2 --------------\n";

            //Was not able to find the edge of a valley. Therefore, there are no obstacles.
            //Robot should travel straight to target.
            if(farIndex == -1)
            {
                return polarHist.getAngleFromBin(startBin);
            }
            std::cout << "TEST 7 --------------\n";


            //Find lower boundary of valley
            for(int i = startBin - 1; getIndex(i, polarHist.getNumBins()) != startBin; i--)
            {
                //std::cout << hist.getBinValue(hist.getBinValue(getIndex(i, hist.getNumBins()))) << "\n\n\n";
                if(polarHist.getBinValue(getIndex(i, polarHist.getNumBins())) > valleyThreshold)
                {
                    nearIndex = i + 1; //Found the nearer edge of the valley
                    break;
                }
            }
            std::cout << "TEST 3 --------------\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            return polarHist.getAngleFromBin(getIndex((farIndex+nearIndex)/2, polarHist.getNumBins()));
        }
        else
        {
            //Finding other valleys in histogram
            //Looping over the entire polar histogram
            //Count stores the number of bins that have been checked
            //Loop begins iterating at the startBin, and alternates back and forth to find nearest valley
            for (int count = 1; count <= polarHist.getNumBins(); count = count + 1) {
                int i = getIndex(startBin + negative * count / 2, polarHist.getNumBins()); //Index of bin to check next
                if (polarHist.getBinValue(i) < valleyThreshold) //Found valley
                {
                    nearIndex = i;
                    count++;
                    i = getIndex(i + negative, polarHist.getNumBins()); //Adds or subtracts index based on direction of array traversal.
                    while (count <= polarHist.getNumBins() && polarHist.getBinValue(i) < valleyThreshold) {
                        i = getIndex(i + negative, polarHist.getNumBins());
                    }
                    farIndex = i;
                    break; //Since loop begins iterating from the target direction, the valley must be the closest valley
                }
                negative *= -1; //Flipping direction
            }
        }

        //Edges of nearest valley are determined, must determine travel direction
        int travelDirectionIndex;
        if(startBin > nearIndex && farIndex > nearIndex)
        {
            travelDirectionIndex = getIndex((nearIndex + farIndex + polarHist.getNumBins())/2, polarHist.getNumBins());
        }
        else if(startBin < nearIndex && farIndex < nearIndex)
        {
            travelDirectionIndex = getIndex((nearIndex - polarHist.getNumBins() + farIndex )/2, polarHist.getNumBins());
        }
        else
        {
            travelDirectionIndex = (nearIndex + farIndex)/2;
        }
        return polarHist.getAngleFromBin(travelDirectionIndex);
    }

    void printHistogram()
    {
        polarHist.printHistogram();
    }

    int** getObjectGrid()
    {
      return grid->getObjectGrid();
    }

    int getCellValue(int i, int j)
    {
        return grid->getCellValue(i, j);
    }

    int getIMax()
    {
        return grid->getIMax();
    }

    int getJMax()
    {
        return grid->getJMax();
    }
};
#endif //VECTORFIELDHISTOGRAMTESTING_VFHPATHER_H
