//
// Created by Swapnil on 1/25/2018.
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
    //Creates a Polar Histogram object with the number of bins passed
    PolarHistogram(int numBins): nBins(numBins), binWidth(360.0/numBins), histogram(new double[nBins])
    {
        for(int i = 0; i < nBins; i++)
        {
            histogram[i] = 0;
        }
    }

    int getBinFromAngle(double angle)
    {
        return (int)(angle/binWidth);
    }

    double getValue(int bin)
    {
        return histogram[bin];
    }

    void setValue(double angle, double value)
    {
        histogram[getBinFromAngle(angle)] = value;
    }

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



};


#endif //VECTORFIELDHISTOGRAMTESTING_POLARHISTOGRAM_H
