// 
// PolarHistogram class creates an object to represent 
// the Polar Histogram
// 

#ifndef VECTORFIELDHISTOGRAMTESTING_POLARHISTOGRAM_H
#define VECTORFIELDHISTOGRAMTESTING_POLARHISTOGRAM_H

#include <cmath>

class PolarHistogram {
private:
    int nBins; //Number of bins to divide polar space around robot into
    double binWidth; //Angular width around robot included in each bin
    double* histogram; //Array storing the values of the polar histogram

public:
    // Constructor.
    // Creates a Polar Histogram object with the number of bins passed.
    PolarHistogram(int numBins) : nBins(numBins), binWidth(360.0/double(numBins)), histogram(new double[nBins])
    {
        for(int i = 0; i < nBins; i++)
        {
            histogram[i] = 0;
        }
    }

    // getBinFromAngle
    // Returns the index of the bin based on the angle relative to the absolute coordinate
    // system with which the histogram is represented
    int getBinFromAngle(double angle)
    {
        return int(angle/binWidth);
    }

    // getAngleFromBin
    // Returns the angle in the middle of the bin
    double getAngleFromBin(int bin)
    {
        return binWidth * (0.5 + double(bin));
    }

    // getNumBins
    // Retrieves the number of bins the 360 degrees is discretized into
    int getNumBins()
    {
        return nBins;
    }

    //getValue
    //Returns the value of the histogram for the specified bin
    double getBinValue(int bin)
    {
        return histogram[bin];
    }

    //addValue
    //Adds the passed value to the current value of the histogram grid
    void addValue(double angle, double value)
    {
        histogram[getBinFromAngle(angle)] += value;
    }

    //smoothHistogram
    //Smoothing function that smooths the values of the histogram using a moving average
    void smoothHistogram(int l)
    {
        for(int i = 0; i < nBins; i++)
        {
            double sum = histogram[i]*l;

            for(int j = i-l, k=1; j < i; j++, k++)
            {
                sum += k*histogram[nBins+(j%nBins)];
            }
            for(int j = i+1, k=l; j <= i+l; j++, k--)
            {
                sum += k*histogram[j%nBins];
            }
            histogram[i] = sum/(2*l+1);
        }
    }

    void printHistogram()
    {
        for(int i = 0; i < nBins; i++)
        {
            std::cout << getAngleFromBin(i) << " " << histogram[i] << "\n";
        }
    }

    void reset()
    {
        for(int i = 0; i < nBins; i++)
        {
            histogram[i] = 0;
        }
    }

};


#endif //VECTORFIELDHISTOGRAMTESTING_POLARHISTOGRAM_H
