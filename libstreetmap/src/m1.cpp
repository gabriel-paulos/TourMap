/* 
 * Copyright 2020 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "m1.h"
#include "globalVariables.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "MapRecord.h"
#include "OSMID.h"
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <iostream> 
#include <math.h>
#include <utility>
#include <algorithm>

using namespace std;
std::vector<std::vector<int>> intersectionStreetSegments;
std::vector<std::vector<string>> intersectionStreetNames;
std::vector<std::set<int>> adjacentIntersections;
std::unordered_map<int, vector<int>> allStreetSegmentsInStreet;
std::vector<std::set<int>> streetsIntersections;
std::vector<double> speedTravelTime;
std::multimap<std::string, int> street_names;
std::unordered_map<OSMID, double> wayOSMIDtoWayLength;

double polygonArea(std::vector<double> X, std::vector<double> Y, int n);

    bool load_map(std::string map_name) {
    bool load_successful = false;
    bool loadOSMsuccessful = false;

    //Going through all possible map files

    size_t endPoint = map_name.find(".streets.bin");
    string exercises = map_name.substr(0, endPoint);
    string mapOSMpath = exercises + ".osm.bin";
    
    //Checks if the map API and OSM database open correctly
    
    load_successful = loadStreetsDatabaseBIN(map_name); 
    loadOSMsuccessful = loadOSMDatabaseBIN(mapOSMpath);
    
    for (int i = 0; i<getNumStreetSegments(); i++){
        double distance = find_street_segment_length(i) * 0.001; // finds distance and divides by time converting to desired units
        double speed = double (getInfoStreetSegment(i).speedLimit);
        double travel_time = (distance/speed) * 3600;
        
        speedTravelTime.push_back(travel_time);
    }
    
    for(int i = 0; i < getNumStreets(); i++){
        std::string StreetName = getStreetName(i);
        std::for_each(StreetName.begin(), StreetName.end(), [](char & c){c = ::tolower(c);});
        StreetName.erase(std::remove(StreetName.begin(), StreetName.end(), ' '), StreetName.end());
        street_names.insert(std::make_pair(StreetName, i));    
    }
    
    unordered_map<const OSMWay *, const vector<OSMID>> wayPtrtoOSMIDVectors;//Gaurav
        unordered_map<OSMID, const OSMNode *> OSMIDtoOSMNodePtr; // unordered map of OSMID's to OMDNode *'s
        unordered_map<const OSMWay *, vector<LatLon>> wayPtrtoLatLon;// unordered map of OSMWay *'s to vectors of LatLons
        
        
        const vector<OSMID> OSMIDNodes; 
        
        const OSMWay * wayPtr;
        const OSMNode * nodePtr;
        
        vector<LatLon> latLonVector;
        
        LatLon nodeCoord1;
        LatLon nodeCoord2;
        
        double way_length;
        
        vector<OSMID> OSMIDVector;
        
        for (int i = 0; i<getNumberOfNodes(); i++){ // creates map of all OSMID's of nodes to OSMNode *
            nodePtr = getNodeByIndex(i);
            OSMIDtoOSMNodePtr.insert({nodePtr->id(), nodePtr});      
        }
    
        for (int i = 0; i<getNumberOfWays(); i++){ // creates map of OSMWay *'s to vectors full of LatLon's for way
        
            wayPtr = getWayByIndex(i);                
            OSMIDVector = getWayMembers(wayPtr);
                    
            for (int j = 0; j<OSMIDVector.size(); j++){
                latLonVector.push_back(getNodeCoords(OSMIDtoOSMNodePtr.at(OSMIDVector[j])));
            }
            
            wayPtrtoLatLon.insert({wayPtr, latLonVector});
            
            latLonVector.clear(); 
        }
        
        for (auto i : wayPtrtoLatLon){ // iterates and calculates every way length 
            
            way_length = 0;
            
            if(isClosedWay(i.first)){ // if way is closed...
                for(int j = 0; j+1 < i.second.size() ; j++){
                    way_length += find_distance_between_two_points(make_pair(i.second[j], i.second[j+1]));
                }
                
            }
            
            else if(!isClosedWay(i.first)){ // if way is open...
                for(int j = 0; j+1 < i.second.size() ; j++){
                    way_length += find_distance_between_two_points(make_pair(i.second[j], i.second[j+1]));
                }
            }
            
            wayOSMIDtoWayLength.insert({i.first->id(), way_length}); // populates map
        }
        
        std::cout << getIntersectionName(73) << std::endl;
        std::cout << getIntersectionName(222) << std::endl;
        
    //Returns true only if it's successful
    return (load_successful && loadOSMsuccessful);
}

void close_map() {
    //Clean-up your map related data structures here
    closeStreetDatabase();
    closeOSMDatabase();
    intersectionStreetSegments.clear();
    intersectionStreetNames.clear();
    adjacentIntersections.clear();
    allStreetSegmentsInStreet.clear();
    speedTravelTime.clear();
    streetsIntersections.clear();
    street_names.clear();
    wayOSMIDtoWayLength.clear();
}


//Returns the distance between two coordinates in meters
double find_distance_between_two_points(std::pair<LatLon, LatLon> points){//Gaurav
    
    double y1 = points.first.lat() * DEGREE_TO_RADIAN; // converts points to radians
    double y2 = points.second.lat() * DEGREE_TO_RADIAN;
    
    double cosLatAvg = DEGREE_TO_RADIAN * std::cos((y1 + y2)/2); // finds cosine of average of latitudes (intermediate step)
 
    double x1 = points.first.lon() * cosLatAvg; // final step to convert to rectangular coordinates
    double x2 = points.second.lon() * cosLatAvg;
    
    double distance_between_two_points = EARTH_RADIUS_METERS * std::sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)); // scales to radius of earth and calculates distance
    
    return distance_between_two_points;
    
}

//Returns the length of the given street segment in meters
double find_street_segment_length(int street_segment_id){//Gaurav
    std::pair <LatLon, LatLon> positionAndIntersection; // pair to hold coordinates

    int curvePointCount = getInfoStreetSegment(street_segment_id).curvePointCount; // gets number of curve points

    double street_segment_length_double = 0; // accumulator for street segment lengths
    

    if (curvePointCount == 0){ // if no curve points 
        
        LatLon from = getIntersectionPosition(getInfoStreetSegment(street_segment_id).from); // gets first LatLon pair of street
        LatLon to = getIntersectionPosition(getInfoStreetSegment(street_segment_id).to); // gets last LatLon pair of street
        
        positionAndIntersection = std::make_pair(from, to); // makes pair of first and last coordinates
        
        street_segment_length_double = find_distance_between_two_points(positionAndIntersection); // finds distance between the first and last points
                
        return street_segment_length_double;
    }
    
    else if (curvePointCount > 0) {
        
        LatLon from = getIntersectionPosition(getInfoStreetSegment(street_segment_id).from); // gets first LatLon pair of street
        LatLon start = getStreetSegmentCurvePoint(0, street_segment_id);
        positionAndIntersection = std::make_pair(from, start); // makes pair of first to last coordinate
        
        street_segment_length_double += find_distance_between_two_points(positionAndIntersection); // accumulator for street segment length
        
        LatLon end = getStreetSegmentCurvePoint(curvePointCount - 1, street_segment_id); // very end of street 
        LatLon to = getIntersectionPosition(getInfoStreetSegment(street_segment_id).to); // very beginning of street
        
        positionAndIntersection = std::make_pair(end, to); 
        
        street_segment_length_double += find_distance_between_two_points(positionAndIntersection);
        
        for (int i = 0; i < curvePointCount-1; i++){ // finds distance in between 0 and n-1 curve points
            LatLon point1 = getStreetSegmentCurvePoint(i, street_segment_id);
            LatLon point2 = getStreetSegmentCurvePoint(i+1, street_segment_id);
            positionAndIntersection = std::make_pair(point1, point2);
        
            street_segment_length_double += find_distance_between_two_points(positionAndIntersection);
        }
                
        return street_segment_length_double;
    }
    
    else{
        return (-1);
    }
    
}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)  
double find_street_segment_travel_time(StreetSegmentIndex streetSegIndex){ // Gaurav
    return speedTravelTime[streetSegIndex]; // picks travel time which has been previously loaded
}

//Returns the nearest intersection to the given position
int find_closest_intersection(LatLon my_position){ //Malhar
    double smallestDistance = std::numeric_limits<int>::max();
    int closestIntersection = 0;
    //For loop goes through all intersections and finds the distance between the point and the intersection
    for (int intersection_id = 0; intersection_id < getNumIntersections(); intersection_id++) {
        std::pair <LatLon, LatLon> positionAndIntersection;
        positionAndIntersection = std::make_pair(my_position, getIntersectionPosition(intersection_id));
        double distance = find_distance_between_two_points(positionAndIntersection);
        if (distance < smallestDistance){// if a distance is intersection distance is smaller, it will be replaced
            smallestDistance = distance; //smallest distance remains
            closestIntersection = intersection_id; //smallest id remains
        }
    }
    return closestIntersection; // the closest intersection id is returned
}

//Returns the street segments for the given intersection 
std::vector<int> find_street_segments_of_intersection(int intersection_id){ //Malhar
    if (intersectionStreetSegments.empty()) {  
        for(int id = 0; id < getNumIntersections(); id++) {
            std::vector<int> streetSegmentIds;
            for(int i = 0; i < getIntersectionStreetSegmentCount(id); i++) {
                int streetSegmentId = getIntersectionStreetSegment(id, i);
                streetSegmentIds.push_back(streetSegmentId);
            }
            intersectionStreetSegments.push_back(streetSegmentIds);
        }
    }
    return intersectionStreetSegments[intersection_id];
}

//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)
 std::vector<std::string> find_street_names_of_intersection(int intersection_id){ //Malhar
    //if the global variable is empty, the following computations will take place
    if (intersectionStreetNames.empty()) {
        //Loops through all intersections
        for(int id = 0; id < getNumIntersections(); id++) {
            std::vector<string> streetSegmentNames;
            //Loops through all street segments for the intersection and adds the name to the intersection id
            for(int i = 0; i < getIntersectionStreetSegmentCount(id); i++) {
                int streetSegmentId = getIntersectionStreetSegment(id, i);
                InfoStreetSegment streetSegment = getInfoStreetSegment(streetSegmentId);
                string streetSegmentName = getStreetName(streetSegment.streetID);
                streetSegmentNames.push_back(streetSegmentName);
            }
            intersectionStreetNames.push_back(streetSegmentNames);
        }
    }
    //returns the street names for the given intersection id
    return intersectionStreetNames[intersection_id];
}
 
//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.

 std::vector<int> find_intersections_of_two_streets(std::pair<int, int> street_ids){//Gabe
    //Find intersections of both streets using their ids and compare their intersection ids with each other, until you find the equal one
    std::vector<int> streetIntersections1 = find_intersections_of_street(street_ids.first);
    std::vector<int> streetIntersections2 = find_intersections_of_street(street_ids.second);
    std::vector<int> common;
    
    for(auto const& first : streetIntersections1){
        for(auto const& second : streetIntersections2){
            
            if(first == second){
                
                common.push_back(first);
            }
        }
    }
    return common;
}


//Returns all intersections along the a given street
std::vector<int> find_intersections_of_street(int street_id){//Gabe
    //Loop through street segments and and store their intersections into a set to remove duplicates, 
    //then access the intersections of the street id
    if (streetsIntersections.empty()) {
        for(int id = 0; id < getNumStreets(); id++) {
            std::set<int> intersectionsForThisStreet;
            for(int streetSeg = 0; streetSeg < getNumStreetSegments(); streetSeg++) {
                InfoStreetSegment streetSegInfo = getInfoStreetSegment(streetSeg);
                if (streetSegInfo.streetID == id) {
                    intersectionsForThisStreet.insert(streetSegInfo.from);
                    intersectionsForThisStreet.insert(streetSegInfo.to);
                }
            }
            streetsIntersections.push_back(intersectionsForThisStreet);
        }
    }
    
    std::vector<int> vectorOfIntersections(streetsIntersections[street_id].begin(), streetsIntersections[street_id].end());
    return vectorOfIntersections;
}

//Returns true if you can get from intersection_ids.first to intersection_ids.second using a single 
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself
bool are_directly_connected(std::pair<int, int> intersection_ids){//Malhar
    bool connected = false;
    if(intersection_ids.first == intersection_ids.second) {
        return true;
    }
    if (intersectionStreetSegments.empty()) {  
        for(int id = 0; id < getNumIntersections(); id++) {
            std::vector<int> streetSegmentIds;
            for(int i = 0; i < getIntersectionStreetSegmentCount(id); i++) {
                int streetSegmentId = getIntersectionStreetSegment(id, i);
                streetSegmentIds.push_back(streetSegmentId);
            }
            intersectionStreetSegments.push_back(streetSegmentIds);
        }
    }
    
    for (int streetSeg = 0; streetSeg < (intersectionStreetSegments[intersection_ids.first].size()); streetSeg++){
        
        int streetSegmentId = getIntersectionStreetSegment(intersection_ids.first, streetSeg);
        InfoStreetSegment streetSegment = getInfoStreetSegment(streetSegmentId);
        
        if(streetSegment.oneWay){
            if((streetSegment.to) == intersection_ids.second) {
                return true;
            }
        }
        
        else if((streetSegment.from) == (intersection_ids.second) ||(streetSegment.to) == (intersection_ids.second)) {
            return true;
        }
    }
    return connected;
}

//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
std::vector<int> find_adjacent_intersections(int intersection_id){//Malhar
    if (adjacentIntersections.empty()) {
        for(int id = 0; id < getNumIntersections(); id++) {
            std::set<int> intersectionsConnected;
            for(int i = 0; i < getIntersectionStreetSegmentCount(id); i++) {
                IntersectionIndex toFrom = 0;
                int streetSegmentId = getIntersectionStreetSegment(id, i);
                InfoStreetSegment streetSegment = getInfoStreetSegment(streetSegmentId);
                
                if ((streetSegment.from) == id){
                    toFrom = streetSegment.to; 
                }
                // If Street is a one way, can't go to -> from
                else if ((streetSegment.to) == id && (!streetSegment.oneWay)){
                    toFrom = streetSegment.from; // Gets the from intersection while checking for a 1-way street. 
                }
        
                if (toFrom != 0){ // Using set to eliminate duplicates as project specs did not allow for duplicates
                    intersectionsConnected.insert(toFrom);
                }
            }
            adjacentIntersections.push_back(intersectionsConnected);
        }
    }
    std::vector<int> vectorOfAdjacentIntersections(adjacentIntersections[intersection_id].begin(), adjacentIntersections[intersection_id].end());
    return vectorOfAdjacentIntersections;
}

//Returns all street segments for the given street
std::vector<int> find_street_segments_of_street(int street_id){//Malhar
    // Entering street segments for all streets
    if (allStreetSegmentsInStreet.empty()) {
        for(int id = 0; id < getNumStreets(); id++) {
            std::vector<int> streetSegments;
            for(int streetSeg = 0; streetSeg < getNumStreetSegments(); streetSeg++) {
                InfoStreetSegment streetSegInfo = getInfoStreetSegment(streetSeg);
                if (streetSegInfo.streetID == id) {
                    streetSegments.push_back(streetSeg);
                }
            }
            allStreetSegmentsInStreet.insert({id, streetSegments});
        }
    }
    return allStreetSegmentsInStreet.at(street_id);
}

//Returns all street ids corresponding to street names that start with the given prefix
//The function should be case-insensitive to the street prefix. You should ignore spaces.
//For example, both "bloor " and "BloOrst" are prefixes to "Bloor Street East".
//If no street names match the given prefix, this routine returns an empty (length 0) 
//vector.
//You can choose what to return if the street prefix passed in is an empty (length 0) 
//string, but your program must not crash if street_prefix is a length 0 string.
std::vector<int> find_street_ids_from_partial_street_name(std::string street_prefix){ // Gabriel
    //Take the street_prefix and make all of its characters lower-case and remove all white spaces.
    //Loop through a multimap and store the matching street names into a vector
    //Setting up the multi-map with all the street names of each street
  
  
    std::for_each(street_prefix.begin(), street_prefix.end(), [](char & c){c = ::tolower(c);});
    street_prefix.erase(std::remove(street_prefix.begin(), street_prefix.end(), ' '), street_prefix.end());
    auto upper = street_names.upper_bound(street_prefix + "\255");
    auto lower = street_names.lower_bound(street_prefix);
    vector<int> streetIDs;
    for(auto it = lower; it != upper; it++){
        if(it != street_names.end())
        streetIDs.push_back(it->second);
    }
    return streetIDs;
}

//Returns the area of the given closed feature in square meters
//Assume a non self-intersecting polygon (i.e. no holes)
//Return 0 if this feature is not a closed polygon.
double find_feature_area(int feature_id){//Gaurav
    int featurePointCount = getFeaturePointCount(feature_id); // gets number of points from feature
    std::vector<double> x;
    std::vector<double> y;
    double latAvg=0;
    double result;
    
    if (getFeaturePoint(0, feature_id).lat() != getFeaturePoint(featurePointCount - 1, feature_id).lat() && getFeaturePoint(0, feature_id).lon() != getFeaturePoint(featurePointCount - 1, feature_id).lon()){ // if it is not a closed area return 0
        return 0;
    }
    
    for (int i = 0; i<featurePointCount; i++){ // finds average of all latitudes
        latAvg += getFeaturePoint(i, feature_id).lat() * DEGREE_TO_RADIAN;
    }
    
    latAvg = latAvg/featurePointCount;
        
    for (int i = 0; i<featurePointCount; i++){ // creates vector of x and y rectangular coordinates
        x.push_back(getFeaturePoint(i, feature_id).lon() * DEGREE_TO_RADIAN * cos(latAvg));
        y.push_back(getFeaturePoint(i, feature_id).lat() * DEGREE_TO_RADIAN);
    }
    
    result = polygonArea(x, y, x.size()); // uses shoe lace formula
        
    return result;
}

double polygonArea(std::vector<double> X, std::vector<double> Y, int n){ //Gaurav
    // Initialize area 
    double area = 0.0; 
  
    // Calculate value of shoelace formula 
    int j = n - 1; 
    for (int i = 0; i < n; i++) 
    { 
        area += EARTH_RADIUS_METERS * EARTH_RADIUS_METERS * (X[j] + X[i]) * (Y[j] - Y[i]); 
        j = i;  // j is previous vertex to i 
    } 
  
    // Return absolute value 
    return abs(area / 2.0); 
} 

//Returns the length of the OSMWay that has the given OSMID, in meters.
//To implement this function you will have to  access the OSMDatabaseAPI.h 
//functions.
double find_way_length(OSMID way_id){//Gaurav 
    return wayOSMIDtoWayLength.at(way_id); // picks out way length from previously generated unordered map
}
