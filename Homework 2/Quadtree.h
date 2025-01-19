#ifndef QUADTREE_H
#define QUADTREE_H
#include <string>
#include <vector>
using namespace std;
// Quadtreenode is a node in the quadtree structure
struct QuadtreeNode {
    string cityName;
    double x;
    double y;
    QuadtreeNode* NW; // Pointer to northwest child
    QuadtreeNode* NE; // Pointer to northeast child
    QuadtreeNode* SW; // Pointer to southwest child
    QuadtreeNode* SE; // Pointer to southeast child
//constructor
    QuadtreeNode(const string& name, double xCoord, double yCoord)
            : cityName(name), x(xCoord), y(yCoord), NW(nullptr), NE(nullptr), SW(nullptr), SE(nullptr) {}
};

class Quadtree {
private:
    QuadtreeNode* root; //root node
    double maxX;
    double maxY;
    vector<string> visitedNodes; //stores node visited
    vector<string> treeOrder; // stores the order of the tree

    QuadtreeNode* addnoderecursive(QuadtreeNode* node, const string& cityName, double x, double y);
    void findradiuslocation(QuadtreeNode* node, double x, double y, double radius,
                                     vector<string>& result);
    void displayrecursive(QuadtreeNode* node);
    void deleteTree(QuadtreeNode* node);
    void generatetreeorder(QuadtreeNode* node); // arrange order of the tree
public:
    //constructor
    Quadtree(double maxX, double maxY);
    // destructor
    ~Quadtree();
//insert new city
    void insert(const string& cityName, double x, double y);
    //find all cities in the radius
    vector<string> findWithinRadius(double x, double y, double radius);
    //get the list of nodes visited while searching the specified radius nodes
    const vector<string>& getVisitedNodes() const { return visitedNodes; }
    //prints the tree structure in readable format
    void prettyPrint();
    const vector<string>& getTreeOrder() const { return treeOrder; }
};

#endif // QUADTREE_H