#include <iostream>
#include "HistogramGrid.h"
#include "PolarHistogram.h"

int main()
{
    PolarHistogram test(10);
    for(int i = 0; i < 10; i++)
    {
        test.setValue(i*36, i*10);
        std::cout << test.getValue(i) << std::endl;
    }
    test.smoothHistogram(5);
    for(int i = 0; i < 10; i++)
    {
        std::cout << test.getValue(i) << std::endl;
    }

}