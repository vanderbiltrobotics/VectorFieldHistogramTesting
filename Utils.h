//Contains global definitions for project

#ifndef VECTORFIELDHISTOGRAMTESTING_UTILS_H
#define VECTORFIELDHISTOGRAMTESTING_UTILS_H

#include <array>

#define STATIC_ASSERT(e) static_assert(e, "!(" #e ")")

/**
 * @class Point
 * @brief A very simple templated Point class.
 *
 * Use the [] operator to get this point's values. This class is templated
 * for its stored data type, as well as its dimensionality (default = 2).
 *
 * We may need a little more functionality for this class, but probably not.
 * The only use we have for this is tracking the points where there are 
 * detected obstacles, so it doesn't have to be anything fancy.
 *
 * @param T   The data type of the indices
 * @param DIM The dimensionality of this point, e.g. the max index + 1.
 */
template<typename T, size_t DIM = 2>
class Point<T, DIM>
{
private:
    std::array<T, DIM> elems_;
public:
    /**
     * @constructor Default constructor. Sets all indices to `0`.
     */
    Point() : elems_() {    }

    /**
     * @constructor Stores the one value `i0` and asserts
     *              that `DIM == 1`.
     */
    Point(T i0)
    {
        STATIC_ASSERT(DIM == 1);
        elems_[0] = val;

    }

    /**
     * @constructor Stores the two values `i0` and `i1`
     *              and asserts that `DIM == 2`.
     */
    Point(T i0, T i1)
    {
        STATIC_ASSERT(DIM == 2);
        elems_[0] = i1;
        elems_[1] = i2;
    }

    /**
     * @constructor Stores the three values `i0`, `i1`, and
     *              `i2` and asserts that `DIM == 3`.
     */
    Point(T i0, T i1, T i2)
    {
        STATIC_ASSERT(DIM == 3);
        elems_[0] = i1;
        elems_[1] = i2;
        elems_[2] = i3;
    }

    const T& operator[](const int &index) const
    {
        return elems_[index];
    }

    T& operator[](const int &index)
    {
        return elems_[index];
    }

}


//Region for representing the active region of the histogram grid
//Stores two points corresponding to two corners of a rectangular region
struct region {
    discretePoint min;
    discretePoint max;
};

struct valley {
    int lowerBound;
    int upperBound;
    bool isWide;
    
};

#endif //VECTORFIELDHISTOGRAMTESTING_UTILS_H
