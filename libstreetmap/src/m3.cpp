/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m3.cpp
 * Author: shahmal5
 * 
 * Created on March 13, 2020, 12:28 PM
 */

#include "m3.h"
#define VISUALIZE // Comment out to turn off visualization
#include <chrono>
#include <thread>
#define NO_EDGE -1  // Illegal edge ID  no edge
#define MAX_TIME 999999 // Max best time
#define MAX_SPEED 120 // (km/hr)
#define AVG_WALK_SPEED 1 // (m/s)
#define DIST_WEIGHT 5 // A* Heuristic Weighting
#define DISTANCE_WEIGHT 2 // Another A* Heuristic
#define DIST_WALK_WEIGHT 3 //Another A* heurisitic

#include "m1.h"
#include "m2.h"
#include "drawMachine.h"
#include <unordered_map>
#include "ezgl/point.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "ezgl/canvas.hpp"
#include "ezgl/camera.hpp"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "MapRecord.h"
#include "OSMID.h"

//Completed
class Node {
    public:
        int nodeId;
        std::vector<int> outgoingEdges;// Outgoing edges etc.
        int reachingEdge; // ID of the edge used to reach this node
        double bestTime; // Shortest time found to this node so far}
        bool visited = 0;
        Node() {
            nodeId = 0;
            
        }
        Node(int a, std::vector<StreetSegmentIndex> b, int c, float d) {
            nodeId = a;
            outgoingEdges = b;
            reachingEdge = c;
            bestTime = d;
        }
};   

std::vector<Node> nodeGraph;

//Completed
struct WaveElem {//With A star
    Node *node;
    int edgeID; // ID of edge used to reach this node  
    double travelTime; // Total travel time to reach node 
    double myGoodIdeaOfValueToSortOn;
    
    WaveElem (Node *n, int id, double time, double heuristic) {
        node = n;
        edgeID = id;
        travelTime = time;
        myGoodIdeaOfValueToSortOn = heuristic;
    }
};

struct Compare{
    bool operator()(const WaveElem& e1, const WaveElem& e2) {
        return (e1.myGoodIdeaOfValueToSortOn) > (e2.myGoodIdeaOfValueToSortOn);
    }
};
//for myGoodIdeaOfValueToSortOn try this heuristic:
 
 // distance = abs(x(currentNode) - x(destination)) + abs(y(currentNode - y(destination))
 // speedCity = 40
 // myGoodIdeaOfValueToSortOn = distance / speedCity
 //  

//Completed
void nodeMap() {
    //load all the nodes with constructor
    int intersectionCount = getNumIntersections();
    for (int i = 0; i < intersectionCount; i++){
        Node node(i, find_street_segments_of_intersection(i), NO_EDGE, MAX_TIME);
        nodeGraph.push_back(node);
    }
}

//Completed
Node* getNodeByID(int intersection){
    if(nodeGraph.empty()){
        nodeMap();
    }
    Node* newNode = &(nodeGraph[intersection]);
    return newNode;
}

void clearNodeMap(){
    for(StreetSegmentIndex it = 0; it < nodeGraph.size(); it++){
        if(nodeGraph[it].reachingEdge != NO_EDGE){
            nodeGraph[it].reachingEdge = NO_EDGE;
        }
        if(nodeGraph[it].bestTime != MAX_TIME){
            nodeGraph[it].bestTime = MAX_TIME;
        }
    }
}

//Completed
std::vector<StreetSegmentIndex> bfsTraceBack (int destID) {
    std::vector<StreetSegmentIndex> path; 
    Node *currNode = getNodeByID(destID);  
    int prevEdge = currNode->reachingEdge;  
    
    while (prevEdge != NO_EDGE) {      
        path.push_back(prevEdge);
        InfoStreetSegment streetSeg = getInfoStreetSegment(prevEdge);
        if(streetSeg.to == currNode->nodeId)
            currNode = getNodeByID(streetSeg.from);
        else
            currNode = getNodeByID(streetSeg.to);     
        prevEdge = currNode->reachingEdge;   
    }
    std::reverse(path.begin(), path.end());
    return path;
}    


//Completed
bool bfsPath (Node* sourceNode, int destID, double turn_penalty) {
    LatLon initial = getIntersectionPosition(sourceNode->nodeId);
    LatLon destination = getIntersectionPosition(destID);
    
    double maxDistance = find_distance_between_two_points
    (std::make_pair(destination, initial));
    
    priority_queue <WaveElem, std::vector<WaveElem>, Compare> waveFront;
    waveFront.push(WaveElem(sourceNode, NO_EDGE, 0.0, 0.0));
    
    while(waveFront.size() != 0) {
        WaveElem wave = waveFront.top(); // Get next element 
        waveFront.pop(); // Remove from waveFront 
        
        Node *currNode = wave.node;
        
        if (wave.travelTime < currNode->bestTime) {
            currNode->reachingEdge = wave.edgeID;
            currNode->bestTime = wave.travelTime;
            
            std::vector<int> outgoingStreetSegs = currNode->outgoingEdges; 
            
            if (currNode->nodeId == destID) // Found destination? 
                return true;
            
            for (int i = 0; i < outgoingStreetSegs.size(); i++) {            
                Node *toNode = getNodeByID((currNode->outgoingEdges)[i]);
                int edgeStreetSegID = outgoingStreetSegs[i];
                int nextIntersection;
                InfoStreetSegment streetSegInfo = 
                        getInfoStreetSegment(edgeStreetSegID);
                
                if(streetSegInfo.to == currNode->nodeId){
                    nextIntersection = streetSegInfo.from;
                }
                
                else {
                    nextIntersection = streetSegInfo.to;
                }
                
                Node *nextNode = getNodeByID(nextIntersection);
                
                double time = nextNode->bestTime + currNode->bestTime;
                
                double turnPenalty = 0.0;
                
                if(currNode->nodeId != sourceNode->nodeId){
                    if(getInfoStreetSegment(wave.edgeID).streetID != 
                            getInfoStreetSegment(edgeStreetSegID).streetID){
                        turnPenalty = turn_penalty;
                    }
                }
                
                double travelTime = wave.travelTime + turnPenalty + 
                            find_street_segment_travel_time(edgeStreetSegID);
                
                LatLon current = getIntersectionPosition(nextNode->nodeId);
                
                double heuristic = 0.0;
                
                if (maxDistance < 30000){
                    heuristic = travelTime + find_distance_between_two_points
                    (std::make_pair(destination, current))/MAX_SPEED * DIST_WEIGHT;
                }
                
                else{
                    heuristic = travelTime + find_distance_between_two_points
                    (std::make_pair(destination, current))/MAX_SPEED * DISTANCE_WEIGHT;
                }
                
                if((streetSegInfo.oneWay && nextIntersection == 
                        streetSegInfo.to) || !(streetSegInfo.oneWay)){
                    waveFront.push(WaveElem(nextNode, edgeStreetSegID,  
                                    travelTime, heuristic));
                }
            }
        }  // End if best path to this node
    } // End while waveFront not empty (more to search)
    return false;  // No path exists! 
}

bool bfsWalkingPath(Node* sourceNode, int destID, double turn_penalty, double travel_speed){
    LatLon destination = getIntersectionPosition(destID);
    priority_queue <WaveElem, std::vector<WaveElem>, Compare> waveFront;
    waveFront.push(WaveElem(sourceNode, NO_EDGE, 0.0, 0.0));
    
    while(waveFront.size() != 0) {
        WaveElem wave = waveFront.top(); // Get next element 
        waveFront.pop(); // Remove from waveFront 
        
        Node *currNode = wave.node;
        
        if (wave.travelTime < currNode->bestTime) {
            currNode->reachingEdge = wave.edgeID;
            currNode->bestTime = wave.travelTime;
            
            std::vector<int> outgoingStreetSegs = currNode->outgoingEdges; 
            
            if (currNode->nodeId == destID) // Found destination? 
                return true;
            
            for (int i = 0; i < outgoingStreetSegs.size(); i++) {            
                //Node *toNode = getNodeByID((currNode->outgoingEdges)[i]);
                int edgeStreetSegID = outgoingStreetSegs[i];
                int nextIntersection;
                InfoStreetSegment streetSegInfo = 
                        getInfoStreetSegment(edgeStreetSegID);
                
                if(streetSegInfo.to == currNode->nodeId){
                    nextIntersection = streetSegInfo.from;
                }
                
                else {
                    nextIntersection = streetSegInfo.to;
                }
                
                Node *nextNode = getNodeByID(nextIntersection);
                
                //double time = nextNode->bestTime + currNode->bestTime;
                
                double turnPenalty = 0.0;
                
                if(currNode->nodeId != sourceNode->nodeId){
                    if(getInfoStreetSegment(wave.edgeID).streetID != 
                            getInfoStreetSegment(edgeStreetSegID).streetID){
                        turnPenalty = turn_penalty;
                    }
                }
                
                double travelTime = wave.travelTime + turnPenalty + 
                            travel_speed*find_street_segment_length(edgeStreetSegID);
                
                LatLon current = getIntersectionPosition(nextNode->nodeId);
                
                double heuristic = travelTime + find_distance_between_two_points
                (std::make_pair(destination, current))/AVG_WALK_SPEED * DIST_WALK_WEIGHT; 
                
                waveFront.push(WaveElem(nextNode, edgeStreetSegID,  
                                    travelTime, heuristic));
            }
        }  // End if best path to this node
    } // End while waveFront not empty (more to search)
    return false;  // No path exists! 
}
    
//Completed
double compute_path_travel_time(const std::vector<StreetSegmentIndex>& path,
                                const double turn_penalty){
    double totalTime = 0.0;
    StreetIndex prevStreetID = 0;
    
    //In case of an empty path
    if(!path.size()){
        return 0;
    }
    
    for(int i = 0; i < path.size(); i++){
        StreetSegmentIndex streetSegID = path[i];
        InfoStreetSegment info = getInfoStreetSegment(streetSegID);
         
        double segmentTime = find_street_segment_travel_time(streetSegID);
        totalTime += segmentTime;
        
        if(i > 0 && info.streetID != prevStreetID){
            totalTime+= turn_penalty;
        }
        
        prevStreetID = info.streetID;
    }
    return totalTime;
}

//Completed
std::vector<StreetSegmentIndex> find_path_between_intersections(
                                    const IntersectionIndex intersect_id_start,
                                    const IntersectionIndex intersect_id_end,
                                    const double turn_penalty){
    Node* sourceNode = getNodeByID(intersect_id_start);
    std::vector<StreetSegmentIndex> path;
    bool foundPath = bfsPath(sourceNode, intersect_id_end, turn_penalty);
    
    if (foundPath){
        path = bfsTraceBack(intersect_id_end);
    }
    clearNodeMap();
    return path;
}

double compute_path_walking_time(const std::vector<StreetSegmentIndex>& path,
                                 const double walking_speed,
                                 const double turn_penalty)
{
   double totalTime = 0.0;
   StreetIndex prevStreetID;
    
    //In case of an empty path
    if(!path.size()){
        return 0;
    }
    
    for(int i = 0 ; i < path.size(); i++){
        StreetSegmentIndex streetSegID = path[i];
        InfoStreetSegment info = getInfoStreetSegment(streetSegID);
         
        double segmentTime = find_street_segment_length(streetSegID)/walking_speed;
        totalTime += segmentTime;
        
        if(i > 0 && info.streetID != prevStreetID){
            totalTime += turn_penalty;
        }
        
        prevStreetID = info.streetID;
    }
    return totalTime;
}


std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> 
        find_path_with_walk_to_pick_up(const IntersectionIndex start_intersection,
                                       const IntersectionIndex end_intersection, 
                                       const double turn_penalty,
                                       const double walking_speed, 
                                       const double walking_time_limit)
{
    Node* sourceNode = getNodeByID(start_intersection);
    std::vector<StreetSegmentIndex> drivePath;
    std::vector<StreetSegmentIndex> walkPath;
    
    double distance_max = walking_time_limit * walking_speed; 
    LatLon start = getIntersectionPosition(start_intersection);
    Node* begin = getNodeByID(start_intersection);
    std::pair<LatLon, LatLon> distance;
    distance.first = start;
    double bestTime;
    LatLon endInt = getIntersectionPosition(end_intersection);
    distance.second = endInt;
    double difference = find_distance_between_two_points(distance);
    std::vector<int> candidates;
    
    if(difference <= distance_max){

        bool foundWalkingPath = bfsWalkingPath(begin, end_intersection, turn_penalty, walking_speed);
        std::vector<StreetSegmentIndex> walkPathTemp = bfsTraceBack(end_intersection);
        double walkTime = compute_path_walking_time(walkPathTemp, walking_speed, turn_penalty);

        clearNodeMap();
        
        if(foundWalkingPath && walkTime <= walking_time_limit){
            walkPath = walkPathTemp;
            drivePath.clear();
        } 
        else{
        bool foundPath = bfsPath(begin, end_intersection, turn_penalty);
        walkPath.clear();
        std::vector<StreetSegmentIndex> pathTemp = bfsTraceBack(end_intersection);
        drivePath = pathTemp;
        
        clearNodeMap();
        }
    }
    
    else if(walking_time_limit == 0){
        drivePath = find_path_between_intersections(start_intersection, end_intersection, turn_penalty);
        walkPath.clear();
    }

    else{
        for(int i = 0; i < getNumIntersections(); i++){
            LatLon end = getIntersectionPosition(i); 
            distance.second = end;
            //Node* c = getNodeByID(i);
            double diff = find_distance_between_two_points(distance);
            
            if(diff < distance_max){
                candidates.push_back(i);
            }
        }
        
        double minTime = MAX_TIME;
      
        for(int i = 0; i < candidates.size(); i++){
            Node* newNode = getNodeByID(candidates[i]); 
            bool foundPath = bfsPath(newNode, end_intersection, turn_penalty);
            
            if (foundPath){
                std::vector<StreetSegmentIndex> newPath = bfsTraceBack(end_intersection);
                double newTime = compute_path_travel_time(newPath, turn_penalty);
                
                clearNodeMap();
            
                if(minTime > newTime){
                    
                    bool foundWalkingPath = bfsWalkingPath(begin, candidates[i], turn_penalty, walking_speed);
                    if(foundWalkingPath){
                        std::vector<StreetSegmentIndex> temporaryWalkPath = bfsTraceBack(candidates[i]);
                        
                        double temporaryTime = compute_path_walking_time(temporaryWalkPath, walking_speed, turn_penalty);
                       
                        if(temporaryTime <= walking_time_limit){
                            //cout << temporaryTime << endl;
                            drivePath = newPath;
                            walkPath = temporaryWalkPath;
                            minTime = newTime;
                        }
                        
                        clearNodeMap();
                    }
                    else{
                        clearNodeMap();
                    }
                }
            }          
        }      
    } 
    
    std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> finalPath;
    finalPath.first = walkPath;
    finalPath.second = drivePath;
    return finalPath;
}

//Don't delete this

// if(diff < distance_max){
//                
//                candidates.push_back(i);
//                
//                
//                //std::cout << diff << " " << distance_max << " " << "passed" <<endl;
//                bool foundWalkingPath = bfsWalkingPath(sourceNode, c->nodeId, turn_penalty, walking_speed);
//                
//                std::vector<StreetSegmentIndex> walkPathTemp = bfsTraceBack(c->nodeId);
//                
//                /*
//                for(auto i : walkPathTemp){
//                    cout << i << endl;
//                    
//                }
//                */
//               
//                double walkTime = compute_path_walking_time(walkPathTemp, walking_speed, turn_penalty);
//                //cout << foundWalkingPath << " walk path " << walkTime << " walkTIme" << endl;
//                if(foundWalkingPath == true && walkTime <= walking_time_limit){
//                    bool foundPath = bfsPath(c, end_intersection, turn_penalty);
//                    //cout << foundPath << " f" << endl;
//                    if(foundPath){
//                        std::vector<StreetSegmentIndex>pathTemp = bfsTraceBack(end_intersection);
//                        double pathTime = compute_path_travel_time(pathTemp, turn_penalty);
//                        
//                        if(k == 0){
////                            for(auto h : pathTemp)
////                            cout << h << endl;
//                            bestTime = pathTime;
//                            for(int j = 0; j < pathTemp.size(); j++)
//                            drivePath;
//                            
//                            for(int j = 0; j < pathTemp.size(); j++)
//                            walkPath.push_back(walkPathTemp[j]);
//                        }
//
//                        if(pathTime < bestTime){
//                           bestTime = pathTime;
//                           for(int j = 0; j < pathTemp.size(); j++)
//                            drivePath.push_back(pathTemp[j]);
//                            
//                            for(int j = 0; i < pathTemp.size(); j++)
//                            walkPath.push_back(walkPathTemp[j]);
//                        }
//                        k++;
//                    }
//                }
//                else{
//                    drivePath.clear();
//                    walkPath.clear();
//                }
//                for(StreetSegmentIndex it = 0; it < nodeGraph.size(); it++){
//                    nodeGraph[it].reachingEdge = NO_EDGE;
//                    nodeGraph[it].bestTime = MAX_TIME;
//                }
//            }
//        }
//    } 
//    if(!drivePath.size()) cout << "not "<< endl;
//    cout << " " << endl;
//    if(!walkPath.size()) cout << " do"<< endl;
//    
//    std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> finalPath;
//    finalPath.first = walkPath;
//    finalPath.second = drivePath;
//    return finalPath;
//
//}
