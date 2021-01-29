 /*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   drawMachine.cpp
 * Author: paulosga and shahmal5 and rangan39
 * 
 * Created on February 21, 2020, 10:03 PM
 */
//READY TO SUBMIT
#include "drawMachine.h"
#include "StreetsDatabaseAPI.h"
#include "m3.h"
#include <math.h>
#include <string>

#define PI 3.14159265

double max_lon;
double min_lon;
double max_lat;
double min_lat;
double max_lon_x_pos;
double min_lon_x_pos;
double max_lat_y_pos;
double min_lat_y_pos;

const double LARGE_WORLD = 3.67602e-5;
const double MEDIUM_WORD = 4.76412e-7;
//const double SMALL_WORLD = 8.00188e-9;
//const double SMALL_WORLD = 4.76412e-7;
double originalScreenArea;
bool areaFlag = false;

std::set<int> writtenStreetNames;

struct intersection_data {
    LatLon position;
    std::string name;
    bool highlight = false;
};

struct POI_data{
    
    LatLon position;
    std::string name;
    bool highlight = false;
    
};

std::vector<ezgl::point2d> points;
std::vector<intersection_data> intersections;
std::unordered_map<const OSMWay *, std::vector<LatLon>> wayPtrtoLatLon;
std::set<int> highlightedIntersections;
std::set<int> highlightedIntersections1;
std::set<int> highlightedIntersections2;

//Coverting LatLon to Cartesian coordinates
float drawMachine::x_from_lon(float lon){ // Malhar
    float lonRadians = lon * DEGREE_TO_RADIAN;
    float latAverage = ((min_lat + max_lat)/2) * DEGREE_TO_RADIAN;
    float x = lonRadians * (cos(latAverage));
    return x;
}

//Coverting LatLon to Cartesian coordinates
float drawMachine::y_from_lat(float lat){ //Malhar
    float latRadians = lat * DEGREE_TO_RADIAN;
    return latRadians;
}

//Coverting Cartesian to LatLon coordinates
float drawMachine::lon_from_x(float x){ //Malhar
    float latAverage = ((min_lat + max_lat)/2) * DEGREE_TO_RADIAN;
    float lonRadians = x/(cos(latAverage));
    float lon = lonRadians/DEGREE_TO_RADIAN;
    return lon;
}

//Coverting Cartesian to LatLon coordinates
float drawMachine::lat_from_y(float y){ //Malhar
    float lat = y / DEGREE_TO_RADIAN;
    return lat;
}

int drawMachine::closest_POI_ID(LatLon position, std::string type){
    
    //find what POI's are turned on
    double smallDistance = 0.000005;
    
    for(int i = 0; i < getNumPointsOfInterest(); i++){
        double distance = 0;
        if(getPointOfInterestType(i) == type){
            
            std::pair <LatLon, LatLon> positionAndIntersection;
            positionAndIntersection = std::make_pair(position, getPointOfInterestPosition(i));
            distance = find_distance_between_two_points(positionAndIntersection);
            
            if(distance <= smallDistance){
                return i; 
            }
        }   
    }
    
}



//Zoom Level is organized in a way that optimizes load speed and carries out code based on logic statements
int drawMachine::returnZoomLevel(ezgl::renderer* g){ //Malhar + Gabe + Gaurav
    if (areaFlag == false){
        originalScreenArea = g->get_visible_world().area();
        areaFlag = true;
    }
    float zoomConstant = 0.33; //Used to determine zoom level
    if(g->get_visible_world().area() >= originalScreenArea){
        ezgl::rectangle white_rectangle = {g->get_visible_world().m_first,g->get_visible_world().width() ,g->get_visible_world().height()};
        g->set_color(ezgl::WHITE);
        g->fill_rectangle(white_rectangle);
        drawPrimaryFeatures (g);
        writeStreetNames(g, 0);
        return 0;
    }
    else if(g->get_visible_world().area() >= pow(zoomConstant, 1) * originalScreenArea){
        ezgl::rectangle white_rectangle = {g->get_visible_world().m_first,g->get_visible_world().width() ,g->get_visible_world().height()};
        g->set_color(ezgl::WHITE);
        g->fill_rectangle(white_rectangle);
        drawPrimaryFeatures (g);
        writeStreetNames(g, 1);
        return 1;
    }
    else if(g->get_visible_world().area() >= pow(zoomConstant, 2) * originalScreenArea){
        ezgl::rectangle white_rectangle = {g->get_visible_world().m_first,g->get_visible_world().width() ,g->get_visible_world().height()};
        g->set_color(ezgl::WHITE);
        g->fill_rectangle(white_rectangle);
        drawPrimaryFeatures (g);
        drawPrimaryStreets(g);
        writeStreetNames(g, 2);
        return 2;
    }
    else if(g->get_visible_world().area() >= pow(zoomConstant, 3) * originalScreenArea){
        ezgl::rectangle white_rectangle = {g->get_visible_world().m_first,g->get_visible_world().width() ,g->get_visible_world().height()};
        g->set_color(ezgl::WHITE);
        g->fill_rectangle(white_rectangle);
        drawPrimaryFeatures (g);
        drawPrimaryStreets(g);
        writeStreetNames(g, 3);
        return 3;
    }
    else if(g->get_visible_world().area() >= pow(zoomConstant, 4) * originalScreenArea){
        ezgl::rectangle white_rectangle = {g->get_visible_world().m_first,g->get_visible_world().width() ,g->get_visible_world().height()};
        g->set_color(ezgl::WHITE);
        g->fill_rectangle(white_rectangle);
        drawPrimaryFeatures(g);
        drawAllFeatures (g);
        drawPrimaryStreets(g);
        writeStreetNames(g, 4);
        return 4;
    }
    else if(g->get_visible_world().area() >= pow(zoomConstant, 5) * originalScreenArea){
        ezgl::rectangle white_rectangle = {g->get_visible_world().m_first,g->get_visible_world().width() ,g->get_visible_world().height()};
        g->set_color(ezgl::WHITE);
        g->fill_rectangle(white_rectangle);
        drawPrimaryFeatures(g);
        drawAllFeatures (g);
        drawPrimaryStreets(g);
        writeStreetNames(g, 5);
        return 5;
    }
    else if(g->get_visible_world().area() >= pow(zoomConstant, 6) * originalScreenArea){
        ezgl::rectangle white_rectangle = {g->get_visible_world().m_first,g->get_visible_world().width() ,g->get_visible_world().height()};
        g->set_color(ezgl::WHITE);
        g->fill_rectangle(white_rectangle);
        drawPrimaryFeatures(g);
        drawAllFeatures (g);
        drawPrimaryStreets(g);
        drawSecondaryStreets(g);
        writeStreetNames(g, 6);
        return 6;
    }
    else if(g->get_visible_world().area() >= pow(zoomConstant, 7) * originalScreenArea){
      
        drawAllFeatures (g);
        drawPrimaryStreets(g);
        drawSecondaryStreets(g);
        drawTertiaryStreets(g);
        writeStreetNames(g, 7);
        return 7;
    }
    else if(g->get_visible_world().area() >= pow(zoomConstant, 8) * originalScreenArea){
       
        drawAllFeatures (g);
        drawPrimaryStreets(g);
        drawSecondaryStreets(g);
        drawTertiaryStreets(g);
        writeStreetNames(g, 8);
        return 8;
    }
    else if(g->get_visible_world().area() >= pow(zoomConstant, 9) * originalScreenArea){
      
        drawAllFeatures (g);
        drawPrimaryStreets(g);
        drawSecondaryStreets(g);
        drawTertiaryStreets(g);
        writeStreetNames(g, 9);
        return 9;
    }
    
    else if(g->get_visible_world().area() >= pow(zoomConstant, 10) * originalScreenArea){
        g->set_color(ezgl::BACKGROUND);
        g->fill_rectangle(g->get_visible_screen());
        drawAllFeatures (g);
        drawPrimaryStreets(g);
        drawSecondaryStreets(g);
        drawTertiaryStreets(g);
        writeStreetNames(g, 10);
//        drawPOI(g);
        return 10;
    }
    
    //Draw everything if zoomLevel is greater than 11
    else{
        
        drawAllFeatures (g);
        drawPrimaryStreets(g);
        drawSecondaryStreets(g);
        drawTertiaryStreets(g);
        writeStreetNames(g, 11);
//        drawPOI(g);
        return 11;
    }
}
/*
//Highlight the closest intersections and unhighlights all the other
std::string drawMachine::infoClosestIntersection(double x, double y){ //Malhar
    LatLon pos = LatLon(lat_from_y(y), lon_from_x(x)); 
    int id = find_closest_intersection(pos); 
    
    //Clear the previously highlighted intersections
    if(!highlightedIntersections.empty()){
        auto it = highlightedIntersections.begin();
        
        intersections[*it].highlight = false;
        highlightedIntersections.clear();
    }
    
    intersections[id].highlight = true;
    highlightedIntersections.insert(id);
    
    return intersections[id].name;
    
}
*/
//Drawing all the intersections across the city
void drawMachine::drawIntersections (ezgl::renderer *g, int zoomLevel){ //Malhar
    if (zoomLevel <= 3) return;
    for (size_t i = 0; i < intersections.size(); ++i) {
        float x = drawMachine::x_from_lon(intersections[i].position.lon());
        float y = drawMachine::y_from_lat(intersections[i].position.lat());
    
        float width = 0.000001;
        float height = width;
    
        if (intersections[i].highlight) {
            g->set_color(ezgl::RED);
        } 
    
        else {
            g->set_color(ezgl::GREY_55);
        }

        g->fill_rectangle({x, y}, {x + width, y + height});
    }
}

// Creates map of OSMWay*'s to vectors of LatLon Coordinates
void drawMachine::setWayOSMIDs(){
    std::unordered_map<const OSMWay *, const std::vector<OSMID>> wayPtrtoOSMIDVectors;//Gaurav
    std::unordered_map<OSMID, const OSMNode *> OSMIDtoOSMNodePtr; // unordered map of OSMID's to OMDNode *'s
    // unordered map of OSMWay *'s to vectors of LatLons

    const std::vector<OSMID> OSMIDNodes; 

    const OSMWay * wayPtr;
    const OSMNode * nodePtr;

    std::vector<LatLon> latLonVector;

    LatLon nodeCoord1;
    LatLon nodeCoord2;

    std::vector<OSMID> OSMIDVector;

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
}

void drawMachine::clearOSMPtr(){
    
    wayPtrtoLatLon.clear();  
}

//Draw the primary streets in the map
void drawMachine::drawPrimaryStreets(ezgl::renderer *g){ // Gaurav

    for(int i = 0; i < getNumberOfWays(); i++){ // loops through ways in map
       
        const OSMWay* w = getWayByIndex(i);
       
        for(int j = 0; j < getTagCount(w); j++){ // uses tag count from lower level API
           
            std::string key, value;
            std::tie(key,value) = getTagPair(w,j);
           
           
            if(key == "highway" && (value == "motorway" || value == "trunk" || value == "primary")){ // only draws if the way is a classified as a primary road
                if(value == "motorway"){
                   
                    g->set_color(ezgl::MOTORWAY); 
                    g->set_line_width(3);
 
                }
               
                else if(value == "trunk"){

                   g->set_color(ezgl:: MOTORWAY);
                   g->set_line_width(2);

                }

                else if(value == "primary"){

                   g->set_color(ezgl:: MOTORWAY);
                   g->set_line_width(2);

                }
               
            
                for (int k = 0; k+1 < wayPtrtoLatLon.at(w).size(); k++){ // only draws if the coordinates of road are contained in screen coordinates

                   double x1 = x_from_lon(wayPtrtoLatLon.at(w)[k].lon());
                   double y1 = y_from_lat(wayPtrtoLatLon.at(w)[k].lat());

                   double x2 = x_from_lon(wayPtrtoLatLon.at(w)[k+1].lon());
                   double y2 = y_from_lat(wayPtrtoLatLon.at(w)[k+1].lat());
                   
                   if(g->get_visible_world().contains(x1,y1) || g->get_visible_world().contains(x2,y2)){
                       g->draw_line({x1, y1}, {x2, y2});
                   }

                }    
 
            }
   
        }
    }
}

//Write all street names based on the zoom level of the map
void drawMachine::writeStreetNames(ezgl::renderer* g, int zoomLevel){ //Malhar
    int numStreetSegments = getNumStreetSegments();
    writtenStreetNames.clear();
    
    for(int i = 0; i < numStreetSegments; i++) {
        if (zoomLevel <= 3) continue;
       
        float streetSpeedLimit = getInfoStreetSegment(i).speedLimit;
        LatLon streetSegmentStart = getIntersectionPosition(getInfoStreetSegment(i).from);
        LatLon streetSegmentEnd = getIntersectionPosition(getInfoStreetSegment(i).to);
        bool oneWayStreet = getInfoStreetSegment(i).oneWay;
        int streetID = getInfoStreetSegment(i).streetID;
        
        //Convert to x, y coordinates
        double streetStartX = x_from_lon(streetSegmentStart.lon());
        double streetStartY = y_from_lat(streetSegmentStart.lat());
        double streetEndX = x_from_lon(streetSegmentEnd.lon());
        double streetEndY = y_from_lat(streetSegmentEnd.lat());
        
        if(!(g->get_visible_world().contains(streetStartX, streetStartY) || 
             g->get_visible_world().contains(streetEndX, streetEndY)))
            continue;
        
        bool exists = (writtenStreetNames.find(streetID) != writtenStreetNames.end());
        if(exists) continue;

        if (zoomLevel >= 4 && streetSpeedLimit >= 95){
            writtenStreetNames.insert(streetID);
            std::string streetName = getStreetName(streetID);
            
            if (streetName == "<unknown>")
                continue;            
            
            double deltaXValue = streetEndX - streetStartX;
            double deltaYValue = streetEndY - streetStartY;
            
            double textRotationAngle = 0;
            
            if (deltaXValue != 0){
                textRotationAngle = atan(deltaYValue/deltaXValue) / DEGREE_TO_RADIAN;
            }
            else textRotationAngle = 90;
            
            double textXPosition = (streetStartX + streetEndX)/2;
            double textYPosition = (streetStartY + streetEndY)/2;
            
            g->set_text_rotation(textRotationAngle);
            g->set_color(ezgl::BLACK);
            
            ezgl::point2d textPosition(textXPosition, textYPosition);
            
            if(!oneWayStreet){
                g->draw_text(textPosition, streetName);
            }
            else if(deltaXValue > 0){
                g->draw_text(textPosition, streetName + " →");
            }
            else{
                g->draw_text(textPosition, streetName + " ←");
            }
        }
        
        else if (zoomLevel >= 6 && streetSpeedLimit >= 80){
            writtenStreetNames.insert(streetID);
            std::string streetName = getStreetName(streetID);
            
            if (streetName == "<unknown>")
                continue;
            
            double deltaXValue = streetEndX - streetStartX;
            double deltaYValue = streetEndY - streetStartY;
            
            double textRotationAngle = 0;
            
            if (deltaXValue != 0){
                textRotationAngle = atan(deltaYValue/deltaXValue) / DEGREE_TO_RADIAN;
            }
            else textRotationAngle = 90;
            
            double textXPosition = (streetStartX + streetEndX)/2;
            double textYPosition = (streetStartY + streetEndY)/2;
            
            g->set_text_rotation(textRotationAngle);
            g->set_color(ezgl::BLACK);
            
            ezgl::point2d textPosition(textXPosition, textYPosition);
            
            if(!oneWayStreet){
                g->draw_text(textPosition, streetName);
            }
            else if(deltaXValue > 0){
                g->draw_text(textPosition, streetName + " →");
            }
            else{
                g->draw_text(textPosition, streetName + " ←");
            }
        }
        
        else if (zoomLevel >= 7 && streetSpeedLimit >= 50){
            writtenStreetNames.insert(streetID);
            std::string streetName = getStreetName(streetID);
            
            if (streetName == "<unknown>")
                continue;
            
            double deltaXValue = streetEndX - streetStartX;
            double deltaYValue = streetEndY - streetStartY;
            
            double textRotationAngle = 0;
            
            if (deltaXValue != 0){
                textRotationAngle = atan(deltaYValue/deltaXValue) / DEGREE_TO_RADIAN;
            }
            else textRotationAngle = 90;
            
            double textXPosition = (streetStartX + streetEndX)/2;
            double textYPosition = (streetStartY + streetEndY)/2;
            
            g->set_text_rotation(textRotationAngle);
            g->set_color(ezgl::BLACK);
            
            ezgl::point2d textPosition(textXPosition, textYPosition);
            
            if(!oneWayStreet){
                g->draw_text(textPosition, streetName);
            }
            else if(deltaXValue > 0){
                g->draw_text(textPosition, streetName + " →");
            }
            else{
                g->draw_text(textPosition, streetName + " ←");
            }
        }
        
        else if (zoomLevel >= 9 && streetSpeedLimit >= 30){
            writtenStreetNames.insert(streetID);
            std::string streetName = getStreetName(streetID);
            
            if (streetName == "<unknown>")
                continue;
            
            double deltaXValue = streetEndX - streetStartX;
            double deltaYValue = streetEndY - streetStartY;
            
            double textRotationAngle = 0;
            
            if (deltaXValue != 0){
                textRotationAngle = atan(deltaYValue/deltaXValue) / DEGREE_TO_RADIAN;
            }
            else textRotationAngle = 90;
            
            double textXPosition = (streetStartX + streetEndX)/2;
            double textYPosition = (streetStartY + streetEndY)/2;
            
            g->set_text_rotation(textRotationAngle);
            g->set_color(ezgl::BLACK);
            
            ezgl::point2d textPosition(textXPosition, textYPosition);
            
            if(!oneWayStreet){
                g->draw_text(textPosition, streetName);
            }
            else if(deltaXValue > 0){
                g->draw_text(textPosition, streetName + " →");
            }
            else{
                g->draw_text(textPosition, streetName + " ←");
            }
        }
    }
}

//Draw all the secondary streets in the map
void drawMachine::drawSecondaryStreets(ezgl::renderer *g){ // Gaurav
    for(int i = 0; i < getNumberOfWays(); i++){ // loops though all ways
       
       const OSMWay* w = getWayByIndex(i);
       
       for(int j = 0; j < getTagCount(w); j++){ // gets key and value from lower level API
           
           std::string key, value;
           std::tie(key,value) = getTagPair(w,j);
           
           
           if(key == "highway"){ // only draws if way is classified as a road and a secondary street

               
               if(value == "secondary"){
                    g->set_color(ezgl::WHITE);  
                    g->set_line_width(3);
               }

               for (int k = 0; k+1 < wayPtrtoLatLon.at(w).size(); k++){ // only draws if street coordinates are in screen
               
                   double x1 = x_from_lon(wayPtrtoLatLon.at(w)[k].lon());
                   double y1 = y_from_lat(wayPtrtoLatLon.at(w)[k].lat());

                   double x2 = x_from_lon(wayPtrtoLatLon.at(w)[k+1].lon());
                   double y2 = y_from_lat(wayPtrtoLatLon.at(w)[k+1].lat());

                   if(g->get_visible_world().contains(x1,y1) || g->get_visible_world().contains(x2,y2)){
                       g->draw_line({x1, y1}, {x2, y2});
                   }
               
               }    
            }
        }
    }
}

//Draw the smallest streets in the map
void drawMachine::drawTertiaryStreets(ezgl::renderer *g){ 
    for(int i = 0; i < getNumberOfWays(); i++){ // loops through all ways
       
        const OSMWay* w = getWayByIndex(i);
        
        for(int j = 0; j < getTagCount(w); j++){ // gets key and value from lower level API
           
           std::string key, value;
           std::tie(key,value) = getTagPair(w,j);
           
           
           if(key == "highway"){
               
               if(value == "tertiary"){
                   
                    g->set_color(ezgl::BLACK);
                    g->set_line_width(3);
               }
               
               else if(value == "unclassified"){
                   
                    g->set_color(ezgl::BLACK);
               }
               
               else if(value == "residential"){
                   
                    g->set_color(ezgl::BLACK);
                    g->set_line_width(2);
               }
               for (int k = 0; k+1 < wayPtrtoLatLon.at(w).size(); k++){ // only draws if coordinates are contained in screen
               
                double x1 = x_from_lon(wayPtrtoLatLon.at(w)[k].lon());
                double y1 = y_from_lat(wayPtrtoLatLon.at(w)[k].lat());
               
                double x2 = x_from_lon(wayPtrtoLatLon.at(w)[k+1].lon());
                double y2 = y_from_lat(wayPtrtoLatLon.at(w)[k+1].lat());
                
                if(g->get_visible_world().contains(x1,y1) || g->get_visible_world().contains(x2,y2)){
                    g->draw_line({x1, y1}, {x2, y2});
                }
                
             }
        
            }  
        }
    }
}

//Drawing only the main features in the map
void drawMachine::drawPrimaryFeatures(ezgl::renderer *g){ 
    for (int i = 0; i < getNumFeatures(); i++){ // loops through all features
            
        if (getFeatureType(i) == Beach || getFeatureType(i) == Lake || getFeatureType(i) == River || getFeatureType(i) == Island || getFeatureType(i) == Greenspace){ // classifies all primary features

            points.clear();

            bool open = false;

            if (getFeaturePointCount(i) == 0){ // if no feature points do nothing

            }

            else if (getFeaturePoint(0, i).lat() != getFeaturePoint(getFeaturePointCount(i) - 1, i).lat() && getFeaturePoint(0, i).lon() != getFeaturePoint(getFeaturePointCount(i) - 1, i).lon()){ // if it is not a closed area return 0
                open = true;
            }

            else {
                for (int j = 0; j < getFeaturePointCount(i) - 1; j++){ // create vector of points

                    double x = x_from_lon(getFeaturePoint(j,i).lon());
                    double y = y_from_lat(getFeaturePoint(j,i).lat());

                    points.push_back({x,y});              
                }
            }

            if (getFeatureType(i) == 0){ // set colors of features
                    g->set_color(ezgl::GREY_55);
            }

            else if (getFeatureType(i) == Park){
                    g->set_color(ezgl::LIGHT_GREENERY);
            }

            else if (getFeatureType(i) == Beach){
                    g->set_color(ezgl::SAND);
            }

            else if (getFeatureType(i) == Lake){
                    g->set_color(ezgl::WATER);
            }

            else if (getFeatureType(i) == River){
                    g->set_color(ezgl::WATER);
            }

            else if (getFeatureType(i) == Island){
                    g->set_color(ezgl::WHITE);
            }

            else if (getFeatureType(i) == Building){
                    g->set_color(ezgl::BUILDINGS);
            }

            else if (getFeatureType(i) == Greenspace){
                g->set_color(ezgl::LIGHT_GREENERY);
            }

            else if (getFeatureType(i) == Golfcourse){
                    g->set_color(ezgl::LIGHT_GREENERY);
            }

            else if (getFeatureType(i) == Stream){
                    g->set_color(ezgl::WATER);
            }

            if (open == true){ // draw a line if feature is open
                for (int j = 0; j + 1 < getFeaturePointCount(i); j++){
                    g->draw_line({x_from_lon(getFeaturePoint(j,i).lon()), y_from_lat(getFeaturePoint(j,i).lat())},{x_from_lon(getFeaturePoint(j+1,i).lon()), y_from_lat(getFeaturePoint(j+1,i).lat())});
                }
            }

            else if (open == false){ // draw a polygon if feature is closed
                if (points.size() > 1){
                    g->fill_poly(points);   
                }
            }
        }
    }
}

//Draw different restaurant locations across the city
void drawMachine::drawRestaurant(ezgl::renderer *g){
    
    for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "restaurant"){
            g->set_color(ezgl::RED);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y}, name, 150.0, DBL_MAX);
        }  
    }
}

/*
 
 All draw POIS have the same logic in different functions.
 If the toggle signal is detected it will cycle through all POIs and only print indicated type
 
 */


//Draw the different fast food places around the city
void drawMachine::drawFastFood(ezgl::renderer *g){
    
    for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "fast_food"){
            g->set_color(ezgl::ORANGE);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y}, name, 150.0, DBL_MAX);
        }  
    }
    
}

//Drawing the locations of all the cafes across the city
void drawMachine::drawCafe(ezgl::renderer *g){
    
    for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "cafe"){
            g->set_color(ezgl::YELLOW);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y}, name, 150.0, DBL_MAX);
        }  
    }
}

//Draw bar locations around the city
void drawMachine::drawBars(ezgl::renderer *g){
    
    for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "bar"){
            g->set_color(ezgl::LIME_GREEN);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y}, name, 150.0, DBL_MAX);
        }  
    }
}

//Drawing the text for Attractions
void drawMachine::drawAttractions(ezgl::renderer *g){
    
    for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "attraction"){
            g->set_color(ezgl::LIGHT_SKY_BLUE);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y}, name, 150.0, DBL_MAX);
        }  
    }
    
}

//Drawing Aquariums across the city
void drawMachine::drawAquarium(ezgl::renderer *g){
    
    for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "aquarium"){
            g->set_color(ezgl::KHAKI);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y}, name, 150.0, DBL_MAX);
        }  
    }
}

//Drawing all the hotels in the city
void drawMachine::drawHotel(ezgl::renderer *g){
    
    for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "hotel"){
            g->set_color(ezgl::SADDLE_BROWN);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y}, name, 150.0, DBL_MAX);
        }  
    }
}

//drawing all the museums in the city
void drawMachine::drawMuseum(ezgl::renderer *g){
    
    for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "museum"){
            g->set_color(ezgl::SAND);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y}, name, 150.0, DBL_MAX);
        }  
    }
    
}

//Drawing the theme parks in the city
void drawMachine::drawThemePark(ezgl::renderer *g){
    
     for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "theme_park"){
            g->set_color(ezgl::ORANGE);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y}, name, 150.0, DBL_MAX);
        }  
    }
    
}

//Drawing the drawViewPoint
void drawMachine::drawViewPoint(ezgl::renderer *g){
    
     for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "viewPoint"){
            g->set_color(ezgl::SAND);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y}, name, 150.0, DBL_MAX);
        }  
    }
    
}

//Drawing the Zoo
void drawMachine::drawZoo(ezgl::renderer *g){
     for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "zoo"){
            g->set_color(ezgl::GREEN);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y}, name, 150.0, DBL_MAX);
        }  
    }    
}

//Drawing all the Point of Interests on the map
void drawMachine::drawPOI(ezgl::renderer *g){
    const int OFFSET = 0.1;
    
    for(int i = 0; i < getNumPointsOfInterest(); i++){
        
        LatLon position = getPointOfInterestPosition(i);
        std::string name = getPointOfInterestName(i);
        std::string type = getPointOfInterestType(i);
        
        double x = x_from_lon(position.lon());
        double y = y_from_lat(position.lat());
        
        if(type == "bar" || type == "cafe" || type == "fast_food" || type == "restaurant"){
            g->set_color(ezgl::PINK);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y-OFFSET}, name, 150.0, DBL_MAX);
        }  
        
        else if(type == "college" || type == "library" || type == "university" || type == "school" ){
            g->set_color(ezgl::TURQUOISE);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y-OFFSET}, name, 150.0, DBL_MAX);
        }  
        
        else if(type == "fuel" || type == "parking"){
            g->set_color(ezgl::ORANGE);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y-OFFSET}, name, 150.0, DBL_MAX);
        }  
        
        else if(type == "courthouse" || type == "firestation" || type == "police" || type == "townhall"){
            g->set_color(ezgl::PLUM);
            g->fill_arc({x,y}, 0.0000005 , 0 , 360);
            g->set_color(ezgl::BLACK);
            g->draw_text({x,y-OFFSET}, name, 150.0, DBL_MAX);
        }      
    }                  
}

//Draw all features incorporates all the features in drawPrimaryFeatures + all the other ones
void drawMachine::drawAllFeatures(ezgl::renderer *g){
    
     int count = 0;
    
    for (int i = 0; i < getNumFeatures(); i++){

        bool flag = true;
       
        
        points.clear();

        bool open = false;

        if (getFeaturePointCount(i) == 0){
            return;
        }

        else if (getFeaturePoint(0, i).lat() != getFeaturePoint(getFeaturePointCount(i) - 1, i).lat() && getFeaturePoint(0, i).lon() != getFeaturePoint(getFeaturePointCount(i) - 1, i).lon()){ // if it is not a closed area return 0
            open = true;
        }

        else {
            for (int j = 0; j < getFeaturePointCount(i) - 1; j++){

                double x = x_from_lon(getFeaturePoint(j,i).lon());
                double y = y_from_lat(getFeaturePoint(j,i).lat());
                
                if (!g->get_visible_world().contains(x,y)){
                    flag = false;
                    
                }
                else{
                    count++;
                }
                
                points.push_back({x,y});              
            }
        }
        
        if (flag && count){

        if (getFeatureType(i) == 0){
                g->set_color(ezgl::GREY_55);
        }

        else if (getFeatureType(i) == Park){
                g->set_color(ezgl::LIGHT_GREENERY);
        }

        else if (getFeatureType(i) == Beach){
                g->set_color(ezgl::SAND);
        }

        else if (getFeatureType(i) == Lake){
                g->set_color(ezgl::WATER);
        }

        else if (getFeatureType(i) == River){
                g->set_color(ezgl::WATER);
        }

        else if (getFeatureType(i) == Island){
                g->set_color(ezgl::WHITE);
        }

        else if (getFeatureType(i) == Building){
                g->set_color(ezgl::BUILDINGS);
        }

        else if (getFeatureType(i) == Greenspace){
            g->set_color(ezgl::LIGHT_GREENERY);
        }

        else if (getFeatureType(i) == Golfcourse){
                g->set_color(ezgl::LIGHT_GREENERY);
        }

        else if (getFeatureType(i) == Stream){
                g->set_color(ezgl::WATER);
        }

        if (open == true){
            for (int j = 0; j + 1 < getFeaturePointCount(i); j++){
                g->draw_line({x_from_lon(getFeaturePoint(j,i).lon()), y_from_lat(getFeaturePoint(j,i).lat())},{x_from_lon(getFeaturePoint(j+1,i).lon()), y_from_lat(getFeaturePoint(j+1,i).lat())});
            }
        }

        else if (open == false){
            if (points.size() > 1){
                g->fill_poly(points);   
            }
        }   
    }
       count=0; 
    }
}

//Parse through the two street names and compute the intersections of two streets and highlight them
void drawMachine::textParser(std::string userStringEntry1, std::string userStringEntry2){ //Malhar
    std::vector<int> resultingMatches1 = find_street_ids_from_partial_street_name(userStringEntry1);
    std::vector<int> resultingMatches2 = find_street_ids_from_partial_street_name(userStringEntry2);
    
    //Clear the previously highlighted intersections
    if(!highlightedIntersections.empty()){
        for (auto it = highlightedIntersections.begin(); it != highlightedIntersections.end(); it++){
            intersections[*it].highlight = false;
        }
        highlightedIntersections.clear();
    }
    
    if(resultingMatches1.size() == 0 || resultingMatches2.size() == 0) {
        cout << "One of the street inputs does not exist. Please check spelling and try again!" << endl;
    }
    
    else{
        cout << "The following intersections are between " 
             << "these two streets: " << endl;
        for (auto it1 = resultingMatches1.begin(); it1 != resultingMatches1.end(); it1++){
            for (auto it2 = resultingMatches2.begin(); it2 != resultingMatches2.end(); it2++){
                std::vector<int> resultIntersections = find_intersections_of_two_streets(std::make_pair(*it1, *it2));
                for (auto it3 = resultIntersections.begin(); it3 != resultIntersections.end(); ++it3) {
                   intersections[*it3].highlight = true;
                   highlightedIntersections.insert(*it3);
                   cout << endl << intersections[*it3].name << endl;
                }
            }
        }
    }
}

//Draw the initial world
ezgl::rectangle drawMachine::initial_world (){
    intersections.resize(getNumIntersections());
    
    intersections[0].position = getIntersectionPosition(0);   
    intersections[0].name = getIntersectionName(0);
    
    max_lat = getIntersectionPosition(0).lat(); 
    min_lat = max_lat;
    max_lon = getIntersectionPosition(0).lon();
    min_lon = max_lon;
    
    intersections.resize(getNumIntersections());

    for (int id = 0; id < getNumIntersections(); ++id) {  
        intersections[id].position = getIntersectionPosition(id);
        intersections[id].name = getIntersectionName(id);

        max_lat = std::max(max_lat, intersections[id].position.lat());
        min_lat = std::min(min_lat, intersections[id].position.lat());
        max_lon = std::max(max_lon, intersections[id].position.lon());
        min_lon = std::min(min_lon, intersections[id].position.lon());
    }
    
    min_lon_x_pos = x_from_lon(min_lon);
    min_lat_y_pos = y_from_lat(min_lat);
    max_lon_x_pos = x_from_lon(max_lon);
    max_lat_y_pos = y_from_lat(max_lat);
    
    ezgl::rectangle initial_world({min_lon_x_pos, min_lat_y_pos}, {max_lon_x_pos, max_lat_y_pos});
    
    return initial_world;
}

int intersect_id_start1;
int intersect_id_end1;


void drawMachine::pathTextParser(std::string userStringEntry1, std::string userStringEntry2, std::string userStringEntry3, std::string userStringEntry4){

    std::vector<int> resultingMatches1 = find_street_ids_from_partial_street_name(userStringEntry1);
    std::vector<int> resultingMatches2 = find_street_ids_from_partial_street_name(userStringEntry2);
    
    //Clear the previously highlighted intersections
    if(!highlightedIntersections1.empty()){
        for (auto it = highlightedIntersections1.begin(); it != highlightedIntersections1.end(); it++){
            intersections[*it].highlight = false;
        }
        highlightedIntersections1.clear();
    }
    
    if(resultingMatches1.size() == 0 || resultingMatches2.size() == 0) {
        cout << "One of the street inputs does not exist. Please check spelling and try again!" << endl;
    }
    
    else{
        cout << "The following intersections are between " 
             << "these two streets: " << endl;
        for (auto it1 = resultingMatches1.begin(); it1 != resultingMatches1.end(); it1++){
            for (auto it2 = resultingMatches2.begin(); it2 != resultingMatches2.end(); it2++){
                std::vector<int> resultIntersections = find_intersections_of_two_streets(std::make_pair(*it1, *it2));
                for (auto it3 = resultIntersections.begin(); it3 != resultIntersections.end(); ++it3) {
                   intersect_id_start1 = resultIntersections.at(0);
                   intersections[*it3].highlight = true;
                   highlightedIntersections1.insert(*it3);
                   cout << endl << intersections[*it3].name << endl;
                }
            }
        }
    }
    
    
    
    std::vector<int> resultingMatches3 = find_street_ids_from_partial_street_name(userStringEntry3);
    std::vector<int> resultingMatches4 = find_street_ids_from_partial_street_name(userStringEntry4);
    
    //Clear the previously highlighted intersections
    if(!highlightedIntersections2.empty()){
        for (auto it = highlightedIntersections2.begin(); it != highlightedIntersections2.end(); it++){
            intersections[*it].highlight = false;
        }
        highlightedIntersections2.clear();
    }
    
    if(resultingMatches3.size() == 0 || resultingMatches4.size() == 0) {
        cout << "One of the street inputs does not exist. Please check spelling and try again!" << endl;
    }
    
    else{
        cout << "The following intersections are between " 
             << "these two streets: " << endl;
        for (auto it1 = resultingMatches3.begin(); it1 != resultingMatches3.end(); it1++){
            for (auto it2 = resultingMatches4.begin(); it2 != resultingMatches4.end(); it2++){
                std::vector<int> resultIntersections = find_intersections_of_two_streets(std::make_pair(*it1, *it2));
                for (auto it3 = resultIntersections.begin(); it3 != resultIntersections.end(); ++it3) {
                   intersect_id_end1 = resultIntersections.at(0);
                   intersections[*it3].highlight = true;
                   highlightedIntersections2.insert(*it3);
                   cout << endl << intersections[*it3].name << endl;
                }
            }
        }
    }
            
}


void drawMachine::highlightPath (ezgl::renderer *g){
    std::vector<StreetSegmentIndex> path = find_path_between_intersections(intersect_id_start1, intersect_id_end1, 1);
    std::vector<StreetSegmentIndex> path2 = find_path_between_intersections(intersect_id_start1, intersect_id_end1, 1);
    
    for (int i = 0; i < path.size(); i++){
        
        LatLon from = getIntersectionPosition(getInfoStreetSegment(path.at(i)).from);
        LatLon to = getIntersectionPosition(getInfoStreetSegment(path.at(i)).to);
        
        double x1 = x_from_lon(from.lon());
        double y1 = y_from_lat(from.lat());

        double x2 = x_from_lon(to.lon());
        double y2 = y_from_lat(to.lat());
        
        g->set_color(ezgl:: RED);
        g->set_line_width(3);
        g->draw_line({x1, y1}, {x2, y2});
        
        double xAngle = x2 - x1;
        double yAngle = y2 - y1;
        double angleDeg = atan2(yAngle, xAngle) * (180 / PI);
        
    }
    

//    if (path2.size()!=0){
//        std::cout << "Testing Directions" << std::endl;
//        for (int j = 0; j < (path2.size()-1); j++){
//         
//        LatLon node1 = getIntersectionPosition(getInfoStreetSegment(path2.at(j)).from);
//        LatLon node2 = getIntersectionPosition(getInfoStreetSegment(path2.at(j)).to);
//        LatLon node3 = getIntersectionPosition(getInfoStreetSegment(path2.at(j+1)).from);
//        LatLon node4 = getIntersectionPosition(getInfoStreetSegment(path2.at(j+1)).to);
//        
//        double x1 = x_from_lon(node1.lon());
//        double y1 = y_from_lat(node1.lat());
//
//        double x2 = x_from_lon(node2.lon());
//        double y2 = y_from_lat(node2.lat());
//        
//        double x3 = x_from_lon(node3.lon());
//        double y3 = y_from_lat(node3.lat());
//        
//        double x4 = x_from_lon(node4.lon());
//        double y4 = y_from_lat(node4.lat());
//        
//        int inter1 = getInfoStreetSegment(path2.at(j)).from;
//        int inter2 = getInfoStreetSegment(path2.at(j)).to;
//        int inter3 = getInfoStreetSegment(path2.at(j+1)).from;
//        int inter4 = getInfoStreetSegment(path2.at(j+1)).to;
//        
//        double xAngle;
//        double yAngle;
//        double angleDeg1;
//        double angleDeg2;
//        
//        if (inter1 == inter3){
//            xAngle = x1 - x2;
//            yAngle = y1 - y2;
//            angleDeg1 = atan2(yAngle, xAngle) * (180 / PI);
//            
//            xAngle = x4 - x1;
//            yAngle = y4 - y1;
//            angleDeg2 = atan2(yAngle, xAngle) * (180 / PI);
//        }
//        
//        else if (inter1 == inter4){
//            xAngle = x1 - x2;
//            yAngle = y1 - y2;
//            angleDeg1 = atan2(yAngle, xAngle) * (180 / PI);
//            
//            xAngle = x3 - x1;
//            yAngle = y3 - y1;
//            angleDeg2 = atan2(yAngle, xAngle) * (180 / PI);
//        }
//        
//        else if (inter2 == inter3){
//            xAngle = x2 - x1;
//            yAngle = y2 - y1;
//            angleDeg1 = atan2(yAngle, xAngle) * (180 / PI);
//            
//            xAngle = x4 - x3;
//            yAngle = y4 - y3;
//            angleDeg2 = atan2(yAngle, xAngle) * (180 / PI);
//        }
//        
//        else if (inter2 == inter4){
//            xAngle = x2 - x1;
//            yAngle = y2 - y1;
//            angleDeg1 = atan2(yAngle, xAngle) * (180 / PI);
//            
//            xAngle = x3 - x4;
//            yAngle = y3 - y4;
//            angleDeg2 = atan2(yAngle, xAngle) * (180 / PI);
//        }
//        
//        
//        if (angleDeg1<0){
//            angleDeg1 + 360;
//        }
//        
//        if (angleDeg2<0){
//            angleDeg2 + 360;
//        }
//        
//        
//        if(angleDeg1 > 0 && angleDeg1 < 90){
//                std::cout << "Testing Directions" << std::endl;
//                if(angleDeg2 >= 0 && angleDeg2 < 90){
//                    
//                    if(angleDeg2 > angleDeg1) {
//                        std::cout << "left" << std::endl;
//                    }
//                    else if(angleDeg2 < angleDeg1){
//                        std::cout << "right" << std::endl;
//                    }
//                    else if(angleDeg2 == angleDeg1) {
//                        std::cout << "straight" << std::endl;
//                    }
//                    
//                }
//                if(angleDeg2 >= 90 && angleDeg2 <= 180){
//                    std::cout << "left" << std::endl;
//                }
//                if(angleDeg2 > 180 && angleDeg2 < 270){
//                    
//                    if(angleDeg2-180 > angleDeg1) {
//                        std::cout << "right" << std::endl;
//                    }
//                    else if(angleDeg2-180 < angleDeg1){
//                        std::cout << "left" << std::endl;
//                    }
//                    
//                }
//                if(angleDeg2 >= 270 && angleDeg2 < 360){
//                    std::cout << "right" << std::endl;
//                }
//                    
//            }
//            
//        
//        
//        /*
//        double x1 = x_from_lon(from.lon());
//        double y1 = y_from_lat(from.lat());
//        double next_to_x = y_from_lat(from.lat());    
//       
//        double x2 = x_from_lon(to.lon());
//        double y2 = y_from_lat(to.lat());
//        double next_to_y = y_from_lat(to.lat());
//        
//        double xAngle = x2 - x1;
//        double yAngle = y2 - y1;
//        double next_Angle_x = next_to_x - x2;
//        double next_Angle_y = next_to_y - y2;
//        
//        double angleDeg1 = atan2(yAngle, xAngle) * (180 / PI);
//        double angleDeg2 = atan2(next_Angle_x, next_Angle_y) * (180 / PI);
//        
//        std::cout << "Angle between street 1: " << angleDeg1 << std::endl;
//        std::cout << "Angle between street 2: " << angleDeg2 << std::endl;
//         */
//        
//        }
//    }
//    
//    path.clear();
//    path2.clear();
    
}


std::set<int> highlightedClickedIntersections;

int intersect_id_start_click;
int intersect_id_end_click;

int countClick = 0;

int id1;
int id2;

std::string drawMachine::infoClosestIntersection(double x, double y){ //Malhar
    countClick++;    
    LatLon pos = LatLon(lat_from_y(y), lon_from_x(x)); 
    
    if (countClick == 1){
        id1 = find_closest_intersection(pos); 
        intersections[id1].highlight = true;
        intersect_id_start_click = id1;
        return intersections[id1].name;
    }
    
    if(countClick == 2){
        id2 = find_closest_intersection(pos); 
        intersections[id2].highlight = true;
        intersect_id_end_click = id2;
        return intersections[id2].name;
    }
    
    if(countClick == 3){
        intersections[id1].highlight = false;
        intersections[id2].highlight = false;
        countClick = 0;
        return ("Intersections Cleared");
    }
     
}


void drawMachine::highlightPathClick (ezgl::renderer *g){
    
    std::vector<StreetSegmentIndex> path = find_path_between_intersections(intersect_id_start_click, intersect_id_end_click, 1);
    
    if (countClick == 2){
        
        for (int i = 0; i < path.size(); i++){
        
        LatLon from = getIntersectionPosition(getInfoStreetSegment(path.at(i)).from);
        LatLon to = getIntersectionPosition(getInfoStreetSegment(path.at(i)).to);
        
        double x1 = x_from_lon(from.lon());
        double y1 = y_from_lat(from.lat());

        double x2 = x_from_lon(to.lon());
        double y2 = y_from_lat(to.lat());

        
        g->set_color(ezgl:: BLUE);
        g->set_line_width(3);
        g->draw_line({x1, y1}, {x2, y2});
        
        }
        
    }
    
    else if (countClick == 3){
        for (int i = 0; i < path.size(); i++){
        
        LatLon from = getIntersectionPosition(getInfoStreetSegment(path.at(i)).from);
        LatLon to = getIntersectionPosition(getInfoStreetSegment(path.at(i)).to);
        
        double x1 = x_from_lon(from.lon());
        double y1 = y_from_lat(from.lat());

        double x2 = x_from_lon(to.lon());
        double y2 = y_from_lat(to.lat());

        g->set_color(ezgl:: WHITE);
        g->set_line_width(3);
        g->draw_line({x1, y1}, {x2, y2});
        
        }
    }
    
    
    path.clear();
    
}

int intersect_id_start2;
int intersect_id_end2;
double walking_speed_global;
double walking_time_limit_global;

void drawMachine::pathTextParserPool(std::string userStringEntry1, std::string userStringEntry2, std::string userStringEntry3, std::string userStringEntry4, double walking_speed, double walking_time_limit){

    std::vector<int> resultingMatches1 = find_street_ids_from_partial_street_name(userStringEntry1);
    std::vector<int> resultingMatches2 = find_street_ids_from_partial_street_name(userStringEntry2);
    
    walking_speed_global = walking_speed;
    walking_time_limit_global = walking_time_limit;
    
    //Clear the previously highlighted intersections
    if(!highlightedIntersections1.empty()){
        for (auto it = highlightedIntersections1.begin(); it != highlightedIntersections1.end(); it++){
            intersections[*it].highlight = false;
        }
        highlightedIntersections1.clear();
    }
    
    if(resultingMatches1.size() == 0 || resultingMatches2.size() == 0) {
        cout << "One of the street inputs does not exist. Please check spelling and try again!" << endl;
    }
    
    else{
        cout << "The following intersections are between " 
             << "these two streets: " << endl;
        for (auto it1 = resultingMatches1.begin(); it1 != resultingMatches1.end(); it1++){
            for (auto it2 = resultingMatches2.begin(); it2 != resultingMatches2.end(); it2++){
                std::vector<int> resultIntersections = find_intersections_of_two_streets(std::make_pair(*it1, *it2));
                for (auto it3 = resultIntersections.begin(); it3 != resultIntersections.end(); ++it3) {
                   intersect_id_start2 = resultIntersections.at(0);
                   intersections[*it3].highlight = true;
                   highlightedIntersections1.insert(*it3);
                   cout << endl << intersections[*it3].name << endl;
                }
            }
        }
    }
    
    
    std::vector<int> resultingMatches3 = find_street_ids_from_partial_street_name(userStringEntry3);
    std::vector<int> resultingMatches4 = find_street_ids_from_partial_street_name(userStringEntry4);
    
    //Clear the previously highlighted intersections
    if(!highlightedIntersections2.empty()){
        for (auto it = highlightedIntersections2.begin(); it != highlightedIntersections2.end(); it++){
            intersections[*it].highlight = false;
        }
        highlightedIntersections2.clear();
    }
    
    if(resultingMatches3.size() == 0 || resultingMatches4.size() == 0) {
        cout << "One of the street inputs does not exist. Please check spelling and try again!" << endl;
    }
    
    else{
        cout << "The following intersections are between " 
             << "these two streets: " << endl;
        for (auto it1 = resultingMatches3.begin(); it1 != resultingMatches3.end(); it1++){
            for (auto it2 = resultingMatches4.begin(); it2 != resultingMatches4.end(); it2++){
                std::vector<int> resultIntersections = find_intersections_of_two_streets(std::make_pair(*it1, *it2));
                for (auto it3 = resultIntersections.begin(); it3 != resultIntersections.end(); ++it3) {
                   intersect_id_end2 = resultIntersections.at(0);
                   intersections[*it3].highlight = true;
                   highlightedIntersections2.insert(*it3);
                   cout << endl << intersections[*it3].name << endl;
                }
            }
        }
    }           
}


void drawMachine::highlightPathPool (ezgl::renderer *g){
    std::vector<StreetSegmentIndex> walking_path = find_path_with_walk_to_pick_up(intersect_id_start2, intersect_id_end2, 1, walking_speed_global, walking_time_limit_global).first;
    std::vector<StreetSegmentIndex> driving_path = find_path_with_walk_to_pick_up(intersect_id_start2, intersect_id_end2, 1, walking_speed_global, walking_time_limit_global).second;
    
    for (int i = 0; i < walking_path.size(); i++){
        
        LatLon from = getIntersectionPosition(getInfoStreetSegment(walking_path.at(i)).from);
        LatLon to = getIntersectionPosition(getInfoStreetSegment(walking_path.at(i)).to);
        
        double x1 = x_from_lon(from.lon());
        double y1 = y_from_lat(from.lat());

        double x2 = x_from_lon(to.lon());
        double y2 = y_from_lat(to.lat());
        
        g->set_color(ezgl::GREEN);
        g->set_line_width(3);
        g->draw_line({x1, y1}, {x2, y2});
        
        double xAngle = x2 - x1;
        double yAngle = y2 - y1;
        double angleDeg = atan2(yAngle, xAngle) * (180 / PI);
        
        
        std::cout << "Angle between street" << angleDeg << std::endl;
        
    }
    
    for (int i = 0; i < driving_path.size(); i++){
        
        LatLon from = getIntersectionPosition(getInfoStreetSegment(driving_path.at(i)).from);
        LatLon to = getIntersectionPosition(getInfoStreetSegment(driving_path.at(i)).to);
        
        double x1 = x_from_lon(from.lon());
        double y1 = y_from_lat(from.lat());

        double x2 = x_from_lon(to.lon());
        double y2 = y_from_lat(to.lat());
        
        g->set_color(ezgl::ORANGE);
        g->set_line_width(3);
        g->draw_line({x1, y1}, {x2, y2});
        
        double xAngle = x2 - x1;
        double yAngle = y2 - y1;
        double angleDeg = atan2(yAngle, xAngle) * (180 / PI);
        
        std::cout << "Coordinates for example: " << xAngle << " " << yAngle << std::endl;
        
        std::cout << "Angle between street" << angleDeg << std::endl;
        
    }
    
    walking_path.clear();
    driving_path.clear();
    
}


