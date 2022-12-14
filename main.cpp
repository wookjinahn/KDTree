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

void TestWikipedia(bool debug)
{
    typedef Point<int, 2> point2d;
    typedef KDTree<int, 2> tree2d;

    point2d points[] = {{ 3, 1 }, { 13, 3 }, { 2, 6 }, { 10, 2 }, { 8, 7 }};

    tree2d tree(std::begin(points), std::end(points), debug);
    point2d n = tree.Nearest({ 9, 4 });

    std::cout << "Wikipedia example data : " << std::endl;
    std::cout << "Nearest Point: " << n << std::endl;
    std::cout << "Distance: " << tree.GetDistance() << std::endl;
    std::cout << "nodes Visited: " << tree.Visited() << std::endl;
}

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

void TestRandom(size_t count, bool debug)
{
    random_point_generator rpg(0, 1);
    KDTree<double, 3> tree(rpg, count, debug);
    Point<double, 3> pt(rpg());
    Point<double, 3> n = tree.Nearest(pt);

    std::cout << "Random data (" << count << " points) : " << std::endl;
    std::cout << "Point: " << pt << std::endl;
    std::cout << "Nearest Point: " << n << std::endl;
    std::cout << "Distance: " << tree.GetDistance() << std::endl;
    std::cout << "nodes Visited: " << tree.Visited() << std::endl;
}

int main()
{
    bool debug = true;

    try
    {
        const auto TestWikipediastartTime = std::chrono::high_resolution_clock::now();
        TestWikipedia(debug);
        const auto TestWikipediastopTime = std::chrono::high_resolution_clock::now();
        std::cout << '\n';

        const auto TestRandomAstartTime = std::chrono::high_resolution_clock::now();
        TestRandom(1000, debug);
        const auto TestRandomAstopTime = std::chrono::high_resolution_clock::now();
        std::cout << '\n';

        const auto TestRandomBstartTime = std::chrono::high_resolution_clock::now();
        TestRandom(1000000, debug);
        const auto TestRandomBstopTime = std::chrono::high_resolution_clock::now();
        std::cout << '\n';

        if (debug)
        {
            const auto TestWikipediaelapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(TestWikipediastopTime - TestWikipediastartTime);
            const auto TestRandomAelapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(TestRandomAstopTime - TestRandomAstartTime);
            const auto TestRandomBelapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(TestRandomBstopTime - TestRandomBstartTime);
            std::cout << "TestWikipedia process runtime : " << TestWikipediaelapsedTime.count() / 1000.0f << " ms. | " << TestWikipediaelapsedTime.count() << " us. | " << 1000000 / TestWikipediaelapsedTime.count() << " Hz." << std::endl;
            std::cout << "TestRandomA process runtime : " << TestRandomAelapsedTime.count() / 1000.0f << " ms. | " << TestRandomAelapsedTime.count() << " us. | " << 1000000 / TestRandomAelapsedTime.count() << " Hz." << std::endl;
            std::cout << "TestRandomB process runtime : " << TestRandomBelapsedTime.count() / 1000.0f << " ms. | " << TestRandomBelapsedTime.count() << " us. | " << 1000000 / TestRandomBelapsedTime.count() << " Hz." << std::endl;
        }

    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}