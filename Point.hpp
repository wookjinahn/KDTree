//
// Created by canine on 22. 12. 3.
//

#ifndef KDTREE_POINT_HPP
#define KDTREE_POINT_HPP

#include <algorithm>
#include <array>
#include <iostream>

/**
 * Class for representing a Point. coordinate_type must be a numeric type.
 */
template<typename coordinate_type, size_t dimensions>
class Point
{
public:
    Point(std::array<coordinate_type, dimensions> c)
        : mCoords(c)
    {
    }

    Point(std::initializer_list<coordinate_type> list)
    {
        size_t n = std::min(dimensions, list.size());
        std::copy_n(list.begin(), n, mCoords.begin());
    }

    /**
     * Returns the coordinate in the given dimension.
     *
     * @param index dimension index (zero based)
     * @return coordinate in the given dimension
     */
    coordinate_type get(size_t index) const
    {
        return mCoords[index];
    }

    /**
     * Returns the Distance squared from this Point to another
     * Point.
     *
     * @param pt another Point
     * @return Distance squared from this Point to the other Point
     */
    double getDistanceSquared(const Point& pt) const
    {
        double dist = 0;
        for (size_t i = 0; i < dimensions; ++i)
        {
            double d = get(i) - pt.get(i);
            dist += d * d;
        }
        return dist;
    }

private:
    std::array<coordinate_type, dimensions> mCoords;
};

template<typename coordinate_type, size_t dimensions>
std::ostream& operator<<(std::ostream& out, const Point<coordinate_type, dimensions>& pt)
{
    out << '(';
    for (size_t i = 0; i < dimensions; ++i)
    {
        if (i > 0)
        {
            out << ", ";
        }
        out << pt.get(i);
    }
    out << ')';
    return out;
}

#endif //KDTREE_POINT_HPP
