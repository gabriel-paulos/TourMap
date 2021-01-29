/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MapRecord.cpp
 * Author: shahmal5
 * 
 * Created on January 30, 2020, 8:42 PM
 */

#include "MapRecord.h"
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "MapRecord.h"
#include "OSMID.h"
#include <vector>
#include <set>
#include <map>
#include <iostream> 
#include <math.h>
#include <utility>
#include <set>
#include <algorithm>

/*
MapRecord::MapRecord(){
        
        setIntersectionsOfStreet();
        setStreetSegmentsOfIntersection();
        setAdjacencyList();
        setStreetSegmentsOfStreet();
        setMultimap();
        setStreetSegmentTravelTime();
    }

void MapRecord::setIntersectionsOfStreet(){
       
    if (streetsIntersections.empty()){
        for(int id = 0; id < getNumStreets(); id++){
            std::set<int> intersectionsForThisStreet;
            for(int streetSeg = 0; streetSeg < getNumStreetSegments(); streetSeg++){
                InfoStreetSegment streetSegInfo = getInfoStreetSegment(streetSeg);
                if(streetSegInfo.streetID == id){
                    intersectionsForThisStreet.insert(streetSegInfo.from);
                    intersectionsForThisStreet.insert(streetSegInfo.to);
                }
            }
            streetsIntersections.push_back(intersectionsForThisStreet);
        }
    }
}
   
std::vector<std::set<int>> MapRecord::getIntersectionsOfStreet(){
   
    return streetsIntersections;       
}

void MapRecord::setStreetSegmentsOfIntersection(){

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
        
}

std::vector<std::vector<int>> MapRecord::getStreetSegmentsOfIntersection(){
    
    return intersectionStreetSegments;

}


void MapRecord:: setAdjacencyList(){
    
    if (adjacentIntersections.empty()) {
        for(int id = 0; id < getNumIntersections(); id++) {
            std::set<int> intersectionsConnected;
            for(int i = 0; i < getIntersectionStreetSegmentCount(id); i++) {
                IntersectionIndex toFrom = -1;
                int streetSegmentId = getIntersectionStreetSegment(id, i);
                InfoStreetSegment streetSegment = getInfoStreetSegment(streetSegmentId);
                
                if ((streetSegment.from) == id){
                    toFrom = streetSegment.to; 
                }
        
                // If Street is a one way, can't go to -> from
                else if ((streetSegment.to) == id && (!streetSegment.oneWay)){
                    toFrom = streetSegment.from; // Gets the from intersection while checking for a 1-way street. 
                }
        
                if (toFrom != -1){ // Using set to eliminate duplicates as project specs did not allow for duplicates
                    intersectionsConnected.insert(toFrom);
                }
                
            }
            adjacentIntersections.push_back(intersectionsConnected);
        }
    }  
}

std::vector<std::set<int>> MapRecord::getAdjacentIntersections(){
    
    return adjacentIntersections;

}

void MapRecord::setStreetNamesOfIntersection(){
    
   if (intersectionStreetNames.empty()) {
        for(int id = 0; id < getNumIntersections(); id++) {
            std::vector<std::string> streetSegmentNames;
            for(int i = 0; i < getIntersectionStreetSegmentCount(id); i++) {
                int streetSegmentId = getIntersectionStreetSegment(id, i);
                InfoStreetSegment streetSegment = getInfoStreetSegment(streetSegmentId);
                std::string streetSegmentName = getStreetName(streetSegment.streetID);
                streetSegmentNames.push_back(streetSegmentName);
            }
            intersectionStreetNames.push_back(streetSegmentNames);
        }
    }
}

std::vector<std::vector<std::string>> MapRecord:: getStreetNamesOfIntersection(){
    
    return intersectionStreetNames;
}

void MapRecord:: setStreetSegmentsOfStreet(){
    
    if (allStreetSegmentsInStreet.empty()) {
        for(int id = 0; id < getNumStreets(); id++) {
            std::set<int> streetSegments;
            for(int streetSeg = 0; streetSeg < getNumStreetSegments(); streetSeg++) {
                InfoStreetSegment streetSegInfo = getInfoStreetSegment(streetSeg);;
                if (streetSegInfo.streetID == id) {
                    streetSegments.insert(streetSeg);
                }
            }
            allStreetSegmentsInStreet.push_back(streetSegments);
        }
    }  
}

std::vector<std::set<int>> MapRecord:: getStreetSegmentsOfStreet(){
    
    return allStreetSegmentsInStreet;
    
}

void MapRecord:: setMultimap(){
    
     for(int i = 0; i < getNumStreets(); i++){
        
        std::string StreetName = getStreetName(i);
        
        std::for_each(StreetName.begin(), StreetName.end(), [](char & c){
	c = ::tolower(c);});
        
       StreetName.erase(std::remove(StreetName.begin(), StreetName.end(), ' '), StreetName.end());
        
        street_names.insert(std::make_pair(StreetName, i));    
    }
}

std::multimap<std::string, int> MapRecord:: getStreetNames(){
    
    return street_names;
    
}

void MapRecord:: setStreetSegmentTravelTime(){
    
     for (int i = 0; i<getNumStreetSegments(); i++){
        double distance = find_street_segment_length(i) * 0.001;
        double speed = double (getInfoStreetSegment(i).speedLimit);
        double travel_time = (distance/speed) * 3600;
        
        speedTravelTime.push_back(travel_time);
    }
     
}

std::vector<double> MapRecord:: getStreetSegmentTravelTime(){
    
    return speedTravelTime;
}

*/






//void MapRecord::setIntersectoStreets(){
    
    //dsfsdfs
//}

   /*
    //Populates intersectionStreetSegments with all intersections and their connected street segments
    //Populates intersectionStreetNames with all intersections and their connected street names
    //Populates adjacentIntersections with all intersections and their adjacent intersections;
    for(int intersection_id = 0; intersection_id < getNumIntersections(); intersection_id++) {
        
        std::vector<int> streetSegmentIds;
        std::vector<string> streetSegmentNames;
        std::vector<int> intersectionsConnected;
        
        
        for(int i = 0; i < getIntersectionStreetSegmentCount(intersection_id); i++) {
            int streetSegmentId = getIntersectionStreetSegment(intersection_id, i);
            streetSegmentIds.push_back(streetSegmentId);
            
            InfoStreetSegment streetSegment = getInfoStreetSegment(streetSegmentId);
            string streetSegmentName = getStreetName(streetSegment.streetID);
            streetSegmentNames.push_back(streetSegmentName);
            
            int adjacentIntersection = streetSegment.to;
            intersectionsConnected.push_back(adjacentIntersection);
        }
        
        intersectionStreetSegments.push_back(streetSegmentIds);
        intersectionStreetNames.push_back(streetSegmentNames);
        adjacentIntersections.push_back(intersectionsConnected);
    }
    */

