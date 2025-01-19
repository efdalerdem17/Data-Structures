#include "Quadtree.h"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

// Constructor
Quadtree::Quadtree(double maxX, double maxY) : root(nullptr), maxX(maxX), maxY(maxY) {}

// Destructor
Quadtree::~Quadtree() {
    deleteTree(root);
}

// delete class to delete all nodes in the tree
void Quadtree::deleteTree(QuadtreeNode* node) {
    if (node == nullptr) return;
    deleteTree(node->NW);
    deleteTree(node->NE);
    deleteTree(node->SW);
    deleteTree(node->SE);
    delete node;
}

// insert function
void Quadtree::insert(const string& cityName, double x, double y) {
    // Check if coordinates are within the defined boundaries
    if (x < 0 || x > maxX || y < 0 || y > maxY) {
        cerr << "Coordinates out of bounds: " << x << "," << y << endl;
        return;
    }
    root = addnoderecursive(root, cityName, x, y);
}

// recursive  function to insert a city into the quadtree
QuadtreeNode* Quadtree::addnoderecursive(QuadtreeNode* node, const string& cityName, double x, double y) {
    if (node == nullptr) {
        return new QuadtreeNode(cityName, x, y);
    }

    if (x == node->x && y == node->y) {
        return node;
    }

    if (x < node->x) {
        if (y >= node->y) {  // if its equal it will go north
            node->NW = addnoderecursive(node->NW, cityName, x, y);
        } else {
            node->SW = addnoderecursive(node->SW, cityName, x, y);
        }
    } else {
        if (y >= node->y) {
            node->NE = addnoderecursive(node->NE, cityName, x, y);
        } else {
            node->SE = addnoderecursive(node->SE, cityName, x, y);
        }
    }
    return node;
}

//  tree traversal order
void Quadtree::generatetreeorder(QuadtreeNode* node) {
    if (node == nullptr) return;
    treeOrder.push_back(node->cityName);
    // traverse in SE-SW-NE-NW
    generatetreeorder(node->SE);
    generatetreeorder(node->SW);
    generatetreeorder(node->NE);
    generatetreeorder(node->NW);
}

// print the tree in the specified order
void Quadtree::prettyPrint() {
    treeOrder.clear();
    generatetreeorder(root);
    displayrecursive(root);
}

// recursive function for pretty printing
void Quadtree::displayrecursive(QuadtreeNode* node) {
    if (node == nullptr) return;
    cout << node->cityName << endl;
    // print in SE-SW-NE-NW order
    displayrecursive(node->SE);
    displayrecursive(node->SW);
    displayrecursive(node->NE);
    displayrecursive(node->NW);
}

// recursive function to find cities within given radius
void Quadtree::findradiuslocation(QuadtreeNode* node, double x, double y, double radius,
                                           vector<string>& result) {
    if (node == nullptr) return;

    // adding  to visited list
    visitedNodes.push_back(node->cityName);

    // calculating distance
    double distance = sqrt(pow(node->x - x, 2) + pow(node->y - y, 2));

    // if within radius --> add to result
    if (distance <= radius) {
        result.push_back(node->cityName);
    }

    // search order logic
    if (x < node->x) {
        if (y < node->y) {
            // Point is in SW quadrant
            findradiuslocation(node->SW, x, y, radius, result);
            if (abs(x - node->x) <= radius) // Check if radius crosses vertical line
                findradiuslocation(node->SE, x, y, radius, result);
            if (abs(y - node->y) <= radius) // Check if radius crosses horizontal line
                findradiuslocation(node->NW, x, y, radius, result);
            if (sqrt(pow(x - node->x, 2) + pow(y - node->y, 2)) <= radius) // Check diagonal
                findradiuslocation(node->NE, x, y, radius, result);
        } else {
            // Point is in NW quadrant
            findradiuslocation(node->NW, x, y, radius, result);
            if (abs(x - node->x) <= radius)
                findradiuslocation(node->NE, x, y, radius, result);
            if (abs(y - node->y) <= radius)
                findradiuslocation(node->SW, x, y, radius, result);
            if (sqrt(pow(x - node->x, 2) + pow(y - node->y, 2)) <= radius)
                findradiuslocation(node->SE, x, y, radius, result);
        }
    } else {
        if (y < node->y) {
            // Point is in SE quadrant
            findradiuslocation(node->SE, x, y, radius, result);
            if (abs(x - node->x) <= radius)
                findradiuslocation(node->SW, x, y, radius, result);
            if (abs(y - node->y) <= radius)
                findradiuslocation(node->NE, x, y, radius, result);
            if (sqrt(pow(x - node->x, 2) + pow(y - node->y, 2)) <= radius)
                findradiuslocation(node->NW, x, y, radius, result);
        } else {
            // Point is in NE quadrant
            findradiuslocation(node->NE, x, y, radius, result);
            if (abs(x - node->x) <= radius)
                findradiuslocation(node->NW, x, y, radius, result);
            if (abs(y - node->y) <= radius)
                findradiuslocation(node->SE, x, y, radius, result);
            if (sqrt(pow(x - node->x, 2) + pow(y - node->y, 2)) <= radius)
                findradiuslocation(node->SW, x, y, radius, result);
        }
    }
}
// to find cities within radius and order results
vector<string> Quadtree::findWithinRadius(double x, double y, double radius) {
    vector<string> result;
    visitedNodes.clear();
    findradiuslocation(root, x, y, radius, result);

    // ordering results
    vector<string> orderedResult;
    for (size_t i = 0; i < treeOrder.size(); i++) {
        const string& city = treeOrder[i];
        if (find(result.begin(), result.end(), city) != result.end()) {
            orderedResult.push_back(city);
        }
    }

// ordering visited cities
    vector<string> orderedVisited;
    for (size_t i = 0; i < treeOrder.size(); i++) {
        const string& city = treeOrder[i];
        if (find(visitedNodes.begin(), visitedNodes.end(), city) != visitedNodes.end()) {
            orderedVisited.push_back(city);
        }
    }
    visitedNodes = orderedVisited;

    return orderedResult;
}