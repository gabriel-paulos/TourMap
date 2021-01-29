/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m4.cpp
 * Author: shahmal5
 * 
 * Created on April 3, 2020, 5:01 AM
 */

#include "m4.h"
#define MAX 99999

std::vector<CourierSubpath> traveling_courier(
                                    const std::vector<DeliveryInfo>& deliveries,
                                    const std::vector<int>& depots,
                                    const float turn_penalty,
                                    const float truck_capacity){
        
    int lastIntersection;
    int distance = MAX;
    
    std::vector<CourierSubpath> path;
    CourierSubpath courierInfo;
    std::vector<StreetSegmentIndex> pickUpToDropOff;
    std::vector<StreetSegmentIndex> dropOffToPickUp;
    std::vector<StreetSegmentIndex> depotToPickUpPath;
    std::vector<StreetSegmentIndex> dropOffToDepot;
    

    
    LatLon initial = getIntersectionPosition(deliveries[0].pickUp);
    LatLon ending = getIntersectionPosition(deliveries[deliveries.size()].dropOff);
    
    int startInt = 0;
    int endInt = 0;
    
    for(int i = 0; i < depots.size(); i++){
        LatLon start = getIntersectionPosition(depots[i]);
        
        int dist = find_distance_between_two_points(make_pair(start, initial));
        if(dist < distance){
            startInt = depots[i];
            distance = dist;
        }
    }
        
    depotToPickUpPath = find_path_between_intersections(startInt, deliveries[0].pickUp, turn_penalty);

    courierInfo.start_intersection = startInt;
    courierInfo.subpath = depotToPickUpPath; 
    courierInfo.end_intersection = deliveries[0].pickUp;
    
    path.push_back(courierInfo);  
    
    for(int i = 0; i < deliveries.size(); i++){
        
        pickUpToDropOff = find_path_between_intersections(deliveries[i].pickUp, deliveries[i].dropOff, turn_penalty);
        
        courierInfo.pickUp_indices.push_back(i);
        courierInfo.start_intersection = deliveries[i].pickUp;
        courierInfo.subpath = pickUpToDropOff;
        courierInfo.end_intersection = deliveries[i].dropOff;
        
        path.push_back(courierInfo);
        courierInfo.pickUp_indices.clear();
        
        if(i < deliveries.size()-1){ 
            dropOffToPickUp = find_path_between_intersections(deliveries[i].dropOff, deliveries[i+1].pickUp, turn_penalty);
            
            courierInfo.start_intersection = courierInfo.end_intersection;
            courierInfo.subpath = dropOffToPickUp;
            courierInfo.end_intersection = deliveries[i+1].pickUp;
            path.push_back(courierInfo);
            
        }
                
    }
    distance = MAX;
    
    for(int i = 0; i < depots.size(); i++){
        LatLon end = getIntersectionPosition(depots[i]);
        
        int dist = find_distance_between_two_points(make_pair(end, ending));
        if(dist < distance){
            endInt = depots[i];
            distance = dist;
        }
    }
    
    
    dropOffToDepot = find_path_between_intersections(courierInfo.end_intersection, endInt, turn_penalty);
        
    courierInfo.start_intersection = courierInfo.end_intersection;    
    courierInfo.subpath = dropOffToDepot;
    courierInfo.end_intersection = endInt;
        
    path.push_back(courierInfo);
    pickUpToDropOff.clear();
    dropOffToPickUp.clear();
    depotToPickUpPath.clear();
    dropOffToDepot.clear();
    
    return path;
    
}