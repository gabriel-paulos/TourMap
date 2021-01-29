/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   drawMachine.h
 * Author: paulosga & shahmal5
 *
 * Created on February 21, 2020, 10:03 PM
 */

#ifndef DRAWMACHINE_H
#define DRAWMACHINE_H
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "m1.h"
#include "globalVariables.h"
#include "m2.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "MapRecord.h"
#include "OSMID.h"
#include "ezgl/point.hpp"
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <math.h>

class drawMachine {
public:
    drawMachine();
    drawMachine(const drawMachine& orig);
    virtual ~drawMachine();
    
    ezgl::rectangle initial_world ();
    
    
    //Converting to cartesian from LatLon
    float x_from_lon(float lon);
    float y_from_lat(float lat);
    float lon_from_x(float x);
    float lat_from_y(float y);
    
    int closest_POI_ID(LatLon position);
    void drawIntersections();
    void drawIntersections (ezgl::renderer *g, int);
    void drawStreets (ezgl::renderer *g);
    void drawFeatures (ezgl::renderer *g);
    
    void setWayOSMIDs();
    void clearOSMPtr();
    std::string infoClosestIntersection(double x, double y);
    
    void drawPrimaryStreets(ezgl::renderer *g);
    void drawSecondaryStreets(ezgl::renderer *g); 
    void drawTertiaryStreets(ezgl::renderer *g);
    
    void drawPrimaryFeatures(ezgl::renderer *g);
    void drawSecondaryFeatures(ezgl::renderer * g);
    void drawRestaurant(ezgl::renderer *g);
    void drawFastFood(ezgl::renderer *g);
    void drawCafe(ezgl::renderer *g);
    void drawBars(ezgl::renderer *g);
    void drawAttractions(ezgl::renderer *g);
    void drawAquarium(ezgl::renderer *g);
    void drawHotel(ezgl::renderer *g);
    void drawMuseum(ezgl::renderer *g);
    void drawThemePark(ezgl::renderer *g);
    void drawViewPoint(ezgl::renderer *g);
    void drawZoo(ezgl::renderer *g);
    void drawPOI(ezgl::renderer *g);
    void drawAllFeatures(ezgl::renderer *g);
    
    int returnZoomLevel(ezgl::renderer* g);
    
    void textParser(std::string, std::string);
    void pathTextParser(std::string userStringEntry1, std::string userStringEntry2, std::string userStringEntry3, std::string userStringEntry4);
    void pathTextParserPool(std::string intersection_1_street_1, std::string intersection_1_street_2, std::string intersection_2_street_1, std::string intersection_2_street_2, double walkingSpeed, double walkingTimeLimit);
    void writeStreetNames(ezgl::renderer* g, int);
    
    void printDirections (double x, double y);
    
    void highlightPath (ezgl::renderer *g);
    void highlightPathClick (ezgl::renderer *g);
    void highlightPathPool(ezgl::renderer *g);
    void setIntersectionPath(int intersect_id_start1, int intersect_id_end1);
    
private:

};
#endif /* DRAWMACHINE_H */

