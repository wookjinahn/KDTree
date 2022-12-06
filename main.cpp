#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>

#include <camel-euclid/Vector.hpp>

#include "Point.hpp"
#include "KDTree.hpp"

void TestWikipedia()
{
    typedef Point<int, 2> point2d;
    typedef KDTree<int, 2> tree2d;

//    point2d points[] = {{ 2, 3 }, { 5, 4 }, { 9, 6 }, { 4, 7 }, { 8, 1 }, { 7, 2 }};
//    point2d points[] = {{ 8, 7 }, { 10, 2 }, { 2, 6 }, { 5, 4 }, { 13, 3 }, { 3, 1 }};
    point2d points[] = {{ 3, 1 }, { 13, 3 }, { 2, 6 }, { 10, 2 }, { 8, 7 }};

//    std::cout << sizeof(point2d) << std::endl;
//    std::cout << "check : " << std::begin(points) << ", " << std::end(points) << ", " << (std::begin(points)+1) << std::endl;
//    std::cout << "check : " << *std::begin(points) << ", " << *std::end(points) << ", " << *(std::begin(points)+1) << ", " << *(std::end(points)-1) << std::endl;
    tree2d tree(std::begin(points), std::end(points));
//    point2d n = tree.Nearest({ 9, 2 });
    point2d n = tree.Nearest({ 9, 4 });

    std::cout << "Wikipedia example data:\n";
    std::cout << "Nearest Point: " << n << '\n';
    std::cout << "Distance: " << tree.GetDistance() << '\n';
    std::cout << "nodes Visited: " << tree.Visited() << '\n';
}

//typedef Point<double, 3> point3d;
//typedef KDTree<double, 3> tree3d;

struct random_point_generator
{
    random_point_generator(double min, double max)
        : engine_(std::random_device()()), distribution_(min, max)
    {
    }

    Point<double, 3> operator()()
    {
        double x = distribution_(engine_);
        double y = distribution_(engine_);
        double z = distribution_(engine_);
        return Point<double, 3>({ x, y, z });
    }

    std::mt19937 engine_;
    std::uniform_real_distribution<double> distribution_;
};

void TestRandom(size_t count)
{
    random_point_generator rpg(0, 1);
    KDTree<double, 3> tree(rpg, count);     // random points are inserted.
    Point<double, 3> pt(rpg());             // new point generate.
    Point<double, 3> n = tree.Nearest(pt);  // check nearestRecursive point about generated new point.

    std::cout << "Random data (" << count << " points):\n";
    std::cout << "Point: " << pt << '\n';
    std::cout << "Nearest Point: " << n << '\n';
    std::cout << "Distance: " << tree.GetDistance() << '\n';
    std::cout << "nodes Visited: " << tree.Visited() << '\n';
}

int main()
{
    try
    {
        const auto startTime = std::chrono::high_resolution_clock::now();
        TestWikipedia();
//        std::cout << '\n';
//        TestRandom(1000);
//        std::cout << '\n';


//        TestRandom(1000000);

        const auto stopTime = std::chrono::high_resolution_clock::now();
        const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);

        std::cout << '\n';
        std::cout << "process runtime : " << elapsedTime.count() / 1000.0f << " ms. | " << elapsedTime.count() << " us. | " << 1000000 / elapsedTime.count() << " Hz." << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}