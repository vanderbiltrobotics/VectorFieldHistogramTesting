#include <iostream>
#include "HistogramGrid.h"

int main() {
    HistogramGrid grid(10,10);
    std::cout << grid.getProbability(1,1);
    return 0;
}