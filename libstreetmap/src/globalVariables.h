/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   globalVariables.h
 * Author: shahmal5
 *
 * Created on February 25, 2020, 11:52 AM
 */
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <set>
#include <map>
#include <unordered_map>
#include <math.h>
#include <utility>
#include <algorithm>

#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

using namespace std;

//GLOBAL VARIABLES
extern std::vector<std::vector<int>> intersectionStreetSegments;
extern std::vector<std::vector<string>> intersectionStreetNames;
extern std::vector<std::set<int>> adjacentIntersections;
extern std::unordered_map<int, vector<int>> allStreetSegmentsInStreet;
extern std::vector<std::set<int>> streetsIntersections;
extern std::vector<double> speedTravelTime;
extern std::multimap<std::string, int> street_names;
extern std::unordered_map<OSMID, double> wayOSMIDtoWayLength;


#endif /* GLOBALVARIABLES_H */

