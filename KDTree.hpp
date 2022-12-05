//
// Created by canine on 22. 12. 3.
//

#ifndef KDTREE_ROSETTA_KDTREE_HPP
#define KDTREE_ROSETTA_KDTREE_HPP

#include "Point.hpp"


/**
 * C++ k-d tree implementation, based on the C version at rosettacode.org.
 */
template<typename coordinate_type, size_t dimensions>
class KDTree
{
public:
    typedef Point<coordinate_type, dimensions> point_type;

public:
//    KDTree(const KDTree&) = delete;
//    KDTree& operator=(const KDTree&) = delete;

    /**
     * Constructor taking a pair of iterators. Adds each
     * Point in the range [begin, end) to the tree.
     *
     * @param begin start of range
     * @param end end of range
     */
    template<typename iterator>
    KDTree(iterator begin, iterator end)
        : mNodes(begin, end)
    {
        mRootNode = makeTreeRecursive(0, mNodes.size(), 0);
    }

    /**
     * Constructor taking a function object that generates
     * points. The function object will be called n times
     * to populate the tree.
     *
     * @param f function that returns a Point
     * @param n number of points to add
     */
    template<typename func>
    KDTree(func&& f, size_t n)
    {
        mNodes.reserve(n);
        for (size_t i = 0; i < n; ++i)
        {
            mNodes.push_back(f());          // generate(at Point<double, 3> random_point_generator::operator()()) and pushback
        }
        mRootNode = makeTreeRecursive(0, mNodes.size(), 0);
    }

    /**
     * Returns true if the tree is Empty, false otherwise.
     */
    bool Empty() const
    {
        return mNodes.empty();
    }

    /**
     * Returns the number of nodes Visited by the last call
     * to Nearest().
     */
    size_t Visited() const
    {
        return mVisited;
    }

    /**
     * Returns the Distance between the input Point and return value
     * from the last call to Nearest().
     */
    double GetDistance() const
    {
        return std::sqrt(mBestDist);
    }

    /**
     * Finds the Nearest Point in the tree to the given Point.
     * It is not valid to call this function if the tree is Empty.
     *
     * @param pt a Point
     * @return the Nearest Point in the tree to the given Point
     */
    const point_type& Nearest(const point_type& pt)
    {
        if (mRootNode == nullptr)
        {
            throw std::logic_error("tree is Empty");
        }

        mBestNode = nullptr;
        mVisited = 0;
        mBestDist = 0;
        nearestRecursive(mRootNode, pt, 0);  // start index = 0 : x

        return mBestNode->mPoint;
    }

private:
    // std::vector<Node> mNodes;
    struct Node
    {
        Node(const point_type& pt)
            : mPoint(pt)
            , mLeft(nullptr)
            , mRight(nullptr)
        {
        }

        coordinate_type get(size_t index) const
        {
            return mPoint.get(index);
        }

        double distance(const point_type& pt) const
        {
            return mPoint.getDistanceSquared(pt);
        }

        point_type mPoint;
        Node* mLeft;
        Node* mRight;
    };

    // for std::nth_element
    struct node_cmp
    {
        node_cmp(size_t index)
            : mIndex(index)
        {
        }

        bool operator()(const Node& n1, const Node& n2) const
        {
            return n1.mPoint.get(mIndex) < n2.mPoint.get(mIndex);
        }

        size_t mIndex;
    };

    //todo
    //at constructor
    //mRootNode = makeTreeRecursive(0, mNodes.size(), 0);
    Node* makeTreeRecursive(size_t begin, size_t end, size_t index)
    {
        // for break recursive.
        if (end <= begin)
        {
            return nullptr;
        }

        size_t n = begin + (end - begin) / 2;   // -> ( begin <-> n ), ( n+1 <-> end) : middle value.
        auto i = mNodes.begin();                // first index of mNodes vector.
        std::nth_element(i + begin, i + n, i + end, node_cmp(index));   // index coordinate
        index = (index + 1) % dimensions;

        mNodes[n].mLeft = makeTreeRecursive(begin, n, index);       // generate left, right tree at n node(middle point).
        mNodes[n].mRight = makeTreeRecursive(n + 1, end, index);

        return &mNodes[n];
    }

    //todo
    //const point_type& Nearest(const point_type& pt)
    //{  nearestRecursive(mRootNode, pt, 0); }
    void nearestRecursive(Node* root, const point_type& point, size_t index)
    {
        if (root == nullptr)
        {
            return;
        }
        // start at root Node
        ++mVisited;
        double distance = root->distance(point);

        if (mBestNode == nullptr || distance < mBestDist)
        {
            mBestDist = distance;
            mBestNode = root;
        }

        if (mBestDist == 0)
        {
            return;
        }

        double dx = root->get(index) - point.get(index);    // difference between current coordinate
        index = (index + 1) % dimensions;                   // next coordinate

        // search left first.
        nearestRecursive(dx > 0 ? root->mLeft : root->mRight, point, index);

        if (dx * dx >= mBestDist)
        {
            return;
        }

        nearestRecursive(dx > 0 ? root->mRight : root->mLeft, point, index);
    }

private:
    std::vector<Node> mNodes;
    Node* mRootNode = nullptr;
    Node* mBestNode = nullptr;
    double mBestDist = 0;
    size_t mVisited = 0;
};


#endif //KDTREE_ROSETTA_KDTREE_HPP
