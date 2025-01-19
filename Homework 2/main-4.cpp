//CS300 - Data Structures - HW2 - Quadtree Implementation
// Ayşe Efdal Erdem

/*This program implements a QuadTree data structure to efficiently find cities within
a specified radius. It reads city locations from a file, builds a QuadTree,
and processes search queries. For each query, it returns both the cities
found within the radius and keeps track of all nodes visited during the search process. */

#include <iostream>
#include <fstream>
#include <sstream>
#include "Quadtree.h"
#include <algorithm>
using namespace std;
// joins strings  with commas
string joinWithComma(const vector<string>& items) {
    //if it is empty give <None>
    if (items.empty()) return "<None>";

    stringstream ss;
    for (size_t i = 0; i < items.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << items[i];
    }
    return ss.str();
}
//line input coordinates
bool parseCoordinatesAndRadius(const string& line, double& x, double& y, double& radius) {
    string temp = line;
    replace(temp.begin(), temp.end(), ',', ' ');

    istringstream iss(temp);
    if (!(iss >> x >> y >> radius)) {
        return false;
    }
    return true;
}

int main() {
    //open cities file
    ifstream citiesFile("cities.txt");
    if (!citiesFile.is_open()) {
      cerr << "Error opening cities.txt" <<endl;
        return 1;
    }
//first line is max x and max y
    double maxX, maxY;
    citiesFile >> maxX >> maxY;

    Quadtree quadtree(maxX, maxY);

    string cityName;
    double x, y;
    while (citiesFile >> cityName >> x >> y) {
        //insert cities to quadtree
        quadtree.insert(cityName, x, y);
    }
    citiesFile.close();

    quadtree.prettyPrint();
    cout << endl;
//open queries file
    ifstream queriesFile("queries0.txt");
    if (!queriesFile.is_open()) {
        cerr << "Error opening queries.txt" << endl;
        return 1;
    }
//process each query line
    string line;
    while (getline(queriesFile, line)) {
        //skip empty lines
        if (line.empty()) continue;

        double queryX, queryY, radius;
        if (parseCoordinatesAndRadius(line, queryX, queryY, radius)) {
            //find cities
            vector<string> citiesInRadius = quadtree.findWithinRadius(queryX, queryY, radius);
            //get list of the nodes
            const vector<string>& visitedCities = quadtree.getVisitedNodes();
            //print results
            cout << joinWithComma(citiesInRadius) << endl;
            cout << joinWithComma(visitedCities) << endl;
            cout << endl;
        }
    }
    queriesFile.close();

    return 0;
}