/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m2.cpp
 * Author: rangan39 and paulosga and shahmal5
 * 
 * Created on February 14, 2020, 5:48 PM
 */

#include "m1.h"
#include "m2.h"
#include "m3.h"
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

std::string name;
bool rest_bool = false;
bool cafe_bool = false;
bool fastFood_bool = false;
bool bars_bool = false;
bool attractions_bool = false;
bool aquarium_bool = false;
bool hotel_bool = false;
bool museum_bool = false;
bool themePark_bool = false;
bool viewPoint_bool = false;
bool zoo_bool = false;
bool draw_path_bool = false;

struct TurnedOn{
    
    bool rest_bool = false;
    bool cafe_bool = false;
    bool fastFood_bool = false;
    bool bars_bool = false;
    bool attractions_bool = false;
    bool aquarium_bool = false;
    bool hotel_bool = false;
    bool museum_bool = false;
    bool themePark_bool = false;
    bool viewPoint_bool = false;
    bool zoo_bool = false;
   
};

void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data);
void test_button(GtkWidget *widget, ezgl::application *application);
void act_on_mouse_click(ezgl::application* app, GdkEventButton* event, double x, double y);
void infoButton(GtkWidget */*widget*/, ezgl::application *application);
void helpButton(GtkWidget */*widget*/, ezgl::application *application);
void act_on_mouse_move(ezgl::application */*application*/, GdkEventButton */*event*/, double x, double y);
void act_on_key_press(ezgl::application *application, GdkEventKey */*event*/, char *key_name);
void draw_main_canvas (ezgl::renderer *g);
void find_pressed(GtkWidget */*widget*/, ezgl::application *application);
void toggleRestaurant(GtkToggleButton *togglebutton);
void toggleFastFood(GtkToggleButton *togglebutton);
void toggleCafe(GtkToggleButton *togglebutton);
void toggleBars(GtkToggleButton *togglebutton);
void toggleAttractions(GtkToggleButton *togglebutton);
void toggleAquarium(GtkToggleButton *togglebutton);
void toggleHotel(GtkToggleButton *togglebutton);
void toggleMuseum(GtkToggleButton *togglebutton);
void toggleThemePark(GtkToggleButton *togglebutton);
void toggleViewPoint(GtkToggleButton *togglebutton);
void toggleZoo(GtkToggleButton *togglebutton);
void initial_setup(ezgl::application *application, bool /*new_window*/);
void ProceedButton(GtkWidget */*widget*/, ezgl::application *application);
void showPath(GtkButton *button);

drawMachine *n;

/*
//Actions that take place based on user mouse click
void act_on_mouse_click(ezgl::application* app, GdkEventButton* event, double x, double y) {
    std::cout << "Mouse clicked at (" << x << "," << y << ")\n";
    
    //Various statements are printed based on what key is clicked
    if (event->button == 1)
        std::cout << "left ";
    else if (event->button == 2)
        std::cout << "middle ";
    else if (event->button == 3)
        std::cout << "right ";

    std::cout << "mouse button at coordinates (" << x << "," << y << ") ";

    if ((event->state & GDK_CONTROL_MASK) && (event->state & GDK_SHIFT_MASK))
        std::cout << "with control and shift pressed ";
    else if (event->state & GDK_CONTROL_MASK)
        std::cout << "with control pressed ";
    else if (event->state & GDK_SHIFT_MASK)
        std::cout << "with shift pressed ";
    
    //Highlight the closest intersection and print it on the screen
    name = n->infoClosestIntersection(x, y);
    app->refresh_drawing();
    cout << endl << "Closest Intersection: " << name << endl;
}
*/


//Actions that take place based on user mouse click
void act_on_mouse_click(ezgl::application* app, GdkEventButton* event, double x, double y) {
    std::cout << "Mouse clicked at (" << x << "," << y << ")\n";
    
    //Various statements are printed based on what key is clicked
    if (event->button == 1)
        std::cout << "left ";
    else if (event->button == 2)
        std::cout << "middle ";
    else if (event->button == 3)
        std::cout << "right ";

    std::cout << "mouse button at coordinates (" << x << "," << y << ") ";

    if ((event->state & GDK_CONTROL_MASK) && (event->state & GDK_SHIFT_MASK))
        std::cout << "with control and shift pressed ";
    else if (event->state & GDK_CONTROL_MASK)
        std::cout << "with control pressed ";
    else if (event->state & GDK_SHIFT_MASK)
        std::cout << "with shift pressed ";
    
    //Highlight the closest intersection and print it on the screen
    name = n->infoClosestIntersection(x, y);
    app->refresh_drawing();
    cout << endl << "Closest Intersection: " << name << endl;
}


void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data){
// For demonstration purposes, this will show the int value of the response type
    
    if (user_data == false)
    {
        std::cout << "remove warning" << std::endl;
    }
    
    std::cout << "response is ";
    switch(response_id) {
        case GTK_RESPONSE_ACCEPT:
            std::cout << "GTK_RESPONSE_ACCEPT ";
            break;
        case GTK_RESPONSE_DELETE_EVENT:
            std::cout << "GTK_RESPONSE_DELETE_EVENT (i.e. ’X’ button) ";
            break;
        case GTK_RESPONSE_REJECT:
            std::cout << "GTK_RESPONSE_REJECT ";
            break;
        default:
            std::cout << "UNKNOWN ";
            break;
    }
    std::cout << "(" << response_id << ")\n";
    /*This will cause the dialog to be destroyed*/
    gtk_widget_destroy(GTK_WIDGET (dialog));         
}

TurnedOn POIOn(){
    
    TurnedOn POIs;
    POIs.aquarium_bool = aquarium_bool;
    POIs.attractions_bool = attractions_bool;
    POIs.bars_bool = bars_bool;
    POIs.cafe_bool = cafe_bool;
    POIs.fastFood_bool = fastFood_bool;
    POIs.hotel_bool = hotel_bool;
    POIs.museum_bool = museum_bool;
    POIs.rest_bool = rest_bool;
    POIs.themePark_bool = themePark_bool;
    POIs.viewPoint_bool = viewPoint_bool;
    POIs.zoo_bool = zoo_bool;
    
    return POIs;
}


void infoButton(GtkWidget */*widget*/, ezgl::application *application)
{
    GObject *window;
    GtkWidget *content_area;
    GtkWidget *label;
    GtkWidget *dialog;
    

  // Update the status bar message
  application->update_message("Info Button Pressed");
  
  window = application->get_object(application->get_main_window_id().c_str());
  
  dialog = gtk_dialog_new_with_buttons(
    "Intersection",
    (GtkWindow*) window,
    GTK_DIALOG_MODAL,
    ("OK"),
    GTK_RESPONSE_CLOSE,
    NULL
    );
  
    // Create a label and attach it to the content area of the dialog
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new(name.c_str());
    gtk_container_add(GTK_CONTAINER(content_area), label);
    // The main purpose of this is to show dialog’s child widget, label
    gtk_widget_show_all(dialog);
    
    g_signal_connect(
    GTK_DIALOG(dialog),
    "response",
    G_CALLBACK(on_dialog_response),
    NULL
    );
    
  // Redraw the main canvas
  application->refresh_drawing();
}


void helpButton(GtkWidget */*widget*/, ezgl::application *application)
{
    GObject *window;
    GtkWidget *content_area;
    GtkWidget *label;
    GtkWidget *dialog;
    

  // Update the status bar message
  application->update_message("Info Button Pressed");
  
  window = application->get_object(application->get_main_window_id().c_str());
  
  dialog = gtk_dialog_new_with_buttons(
    "INSTRUCTIONS",
    (GtkWindow*) window,
    GTK_DIALOG_MODAL,
    ("OK"),
    GTK_RESPONSE_ACCEPT,
    ("CANCEL"),
    GTK_RESPONSE_REJECT,
    NULL
    );
  
    // Create a label and attach it to the content area of the dialog
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new("\nHello! Welcome to TourMap. Using our GUI is really as simple as that! Below you'll find all the instructions you'll need to use the variety of features incorporated into our map."
            "\n\nMouse Clicks to Find Shortest Path"
            "\n1. Click anywhere on the map to select your starting point"
            "\n2. Click a second time to select your end point"
            "\n3. Click a third time to clear the path"
            "\n\nShortest Driving Path"
            "\n1. Enter the starting intersection in the first row of text boxes at the bottom of the GUI. To define the intersection enter the first and second street in the text boxes"
            "\n2. Enter the end intersection in the second row of text boxes and define the intersection just like you did with the starting intersection"
            "\n3. Press the Drive Only button"
            "\n\nShortest Walk + Driving Path"
            "\n1. Enter the starting intersection in the first row of text boxes at the bottom of the GUI. To define the intersection enter the first and second street in the text boxes"
            "\n2. Enter the end intersection in the second row of text boxes and define the intersection just like you did with the starting intersection)"
            "\n3. Enter the walking speed and time limit in the indicated text boxes"
            "\n4. Press the Drive + Walk button"
            );
    gtk_container_add(GTK_CONTAINER(content_area), label);
    // The main purpose of this is to show dialog’s child widget, label
    gtk_widget_show_all(dialog);
    
    g_signal_connect(
    GTK_DIALOG(dialog),
    "response",
    G_CALLBACK(on_dialog_response),
    NULL
    );
    
  // Redraw the main canvas
  application->refresh_drawing();
}


void act_on_mouse_move(ezgl::application */*application*/, GdkEventButton */*event*/, double x, double y){
    //std::cout << "Mouse move at coordinates (" << x << "," << y << ") "<< std::endl;
}

/**
 * Function to handle keyboard press event
 * The name of the key pressed is returned (0-9, a-z, A-Z, Up, Down, Left, Right, Shift_R, Control_L, space, Tab, ...)
 * A pointer to the application and the entire GDK event are also returned
 */

//Extra Feature: Moving the screen based on arrow keys from Keyboard
void act_on_key_press(ezgl::application *application, GdkEventKey */*event*/, char *key_name){ //Malhar
    double translateFactor = 8.0;
    
    std::string canvasID = application->get_main_canvas_id();
    ezgl::canvas* canvasPointer = application->get_canvas(canvasID);
    
    //Used to move around the map using arrow keys on keyboard
    if(strcmp(key_name, "Left") == 0){
        ezgl::translate_left(canvasPointer, translateFactor);
    } 
    else if(strcmp(key_name, "Up") == 0){
        ezgl::translate_up(canvasPointer, translateFactor);
    }
    else if(strcmp(key_name, "Down") == 0){
        ezgl::translate_down(canvasPointer, translateFactor);
    }
    else if(strcmp(key_name, "Right") == 0){
        ezgl::translate_right(canvasPointer, translateFactor); 
    }
}

//Drawing the main_canvas based on Zoom Level and extra Features centered towards Toursim Focus
void draw_main_canvas (ezgl::renderer *g) {
    
    std::cout << "Current Screen Area " << g->get_visible_world().area() << std::endl;
    
    int zoomLevel = n->returnZoomLevel(g);
    
    n->drawIntersections(g, zoomLevel);
    
    n->highlightPath(g);
    n->highlightPathPool(g);
    n->highlightPathClick(g);
    
    if(rest_bool == true && zoomLevel >= 8) n->drawRestaurant(g);
    
    if(cafe_bool == true && zoomLevel >= 8) n->drawCafe(g);
        
    if(fastFood_bool == true && zoomLevel >= 8)  n->drawFastFood(g);
    
    if(bars_bool == true && zoomLevel >= 8) n->drawBars(g);
    
    if(attractions_bool == true && zoomLevel >= 8) n->drawAttractions(g);  
    
    if(aquarium_bool == true && zoomLevel >= 8) n->drawAquarium(g);
        
    if(hotel_bool == true && zoomLevel >= 8) n->drawHotel(g);
    
    if(museum_bool == true && zoomLevel >= 8) n->drawMuseum(g);
    
    if(themePark_bool == true && zoomLevel >= 8) n->drawThemePark(g);
    
    if(viewPoint_bool == true && zoomLevel >= 8) n->drawViewPoint(g);
        
    if(zoo_bool == true && zoomLevel >= 8) n->drawZoo(g);
       
}

//Implemented for the find intersections of the 2 street segments
void find_pressed(GtkWidget */*widget*/, ezgl::application *application){
    
    GtkEntry* street1 = (GtkEntry *) application->get_object("street1");
    GtkEntry* street2 = (GtkEntry *) application->get_object("street2");
    
    std::string street1_text = gtk_entry_get_text (street1);
    std::string street2_text = gtk_entry_get_text (street2);
    
    std::cout <<  street1_text  << std::endl;
    std::cout <<  street2_text  << std::endl;
    
    n->textParser(street1_text, street2_text);
    application->refresh_drawing();
    
}

/*d

All toggle functions work on same logic
if the toggle singal is detected it will set a bool that calls a function 

 */


void toggleRestaurant(GtkToggleButton *togglebutton, ezgl::application *application,
        ezgl::renderer *g){
    std::cout << gtk_toggle_button_get_active (togglebutton) << std::endl;
    
    //application->get_main_canvas_id();
    
    if(gtk_toggle_button_get_active (togglebutton)){
        
        rest_bool = true;
    }
    else{
        
        rest_bool = false;
        
    }
    
}

void toggleFastFood(GtkToggleButton *togglebutton){
    std::cout << gtk_toggle_button_get_active (togglebutton) << std::endl;
    
    if(gtk_toggle_button_get_active (togglebutton)){
       
        fastFood_bool = true;

    }
    else{
        fastFood_bool = false;
    }
}

void toggleCafe(GtkToggleButton *togglebutton){
    std::cout << gtk_toggle_button_get_active (togglebutton) << std::endl;
    
    if(gtk_toggle_button_get_active (togglebutton)){
       
        cafe_bool = true;
    }
    else{
        cafe_bool = false;
    }
}

void toggleBars(GtkToggleButton *togglebutton){
    std::cout << gtk_toggle_button_get_active (togglebutton) << std::endl;
    
    if(gtk_toggle_button_get_active (togglebutton)){
       
        bars_bool = true;
    }
    else{
        bars_bool = false;
    }
}

void toggleAttractions(GtkToggleButton *togglebutton){
    std::cout << gtk_toggle_button_get_active (togglebutton) << std::endl;
    
    if(gtk_toggle_button_get_active (togglebutton)){
       
        attractions_bool = true;
    }
    else{
        attractions_bool = false;
    }
}

void toggleAquarium(GtkToggleButton *togglebutton){
    std::cout << gtk_toggle_button_get_active (togglebutton) << std::endl;
    
    if(gtk_toggle_button_get_active (togglebutton)){
       
        aquarium_bool = true;
    }
    else{
        aquarium_bool = false;
    }
}

void toggleHotel(GtkToggleButton *togglebutton){
    std::cout << gtk_toggle_button_get_active (togglebutton) << std::endl;
    
    if(gtk_toggle_button_get_active (togglebutton)){
       
        hotel_bool = true;
    }
    else{
        hotel_bool = false;
    }
}

void toggleMuseum(GtkToggleButton *togglebutton){
    std::cout << gtk_toggle_button_get_active (togglebutton) << std::endl;
    
    if(gtk_toggle_button_get_active (togglebutton)){
       
        museum_bool = true;
    }
    else{
        museum_bool = false;
    }
}

void toggleThemePark(GtkToggleButton *togglebutton){
    std::cout << gtk_toggle_button_get_active (togglebutton) << std::endl;
    
    if(gtk_toggle_button_get_active (togglebutton)){
       
        themePark_bool = true;
    }
    else{
        themePark_bool = false;
    }
}

void toggleViewPoint(GtkToggleButton *togglebutton){
    std::cout << gtk_toggle_button_get_active (togglebutton) << std::endl;
    
    if(gtk_toggle_button_get_active (togglebutton)){
       
        viewPoint_bool = true;
    }
    else{
        viewPoint_bool = false;
    }
}


void showPath(GtkButton *button, gpointer user_data){
    
    
    //GtkEntry* street1 = (GtkEntry *) user_data->get_object("int1street1");
    //GtkEntry* street2 = (GtkEntry *) user_data->get_object("int1street2");
    
    //std::string street1_text = gtk_entry_get_text (street1);
    //std::string street2_text = gtk_entry_get_text (street2);
    
    //std::cout <<  street1_text  << std::endl;
    //std::cout <<  street2_text  << std::endl;
    
}


//Main code for the initial set-up for the map includings callbacks
void initial_setup(ezgl::application *application, bool /*new_window*/){
  //application->create_button_find("Info", 0, 6, 1, 1, infoButton); // creates buttons for callbacks
  application->create_button_find("Help", -2, 0, 1, 1, helpButton); // creates buttons for callbacks
  application->create_button_find("Info", -1, 0, 1, 1, infoButton); // creates buttons for callbacks
  application->create_button_find("Find", 3, 0, 1, 1, find_pressed);
  
  GtkCheckButton* restaurant = (GtkCheckButton *) application->get_object("restaurant"); // GTK button declaration to get ID from main.ui
  GtkCheckButton* fastFood = (GtkCheckButton *) application->get_object("fast food");
  GtkCheckButton* cafe = (GtkCheckButton *) application->get_object("cafe");
  GtkCheckButton* bars = (GtkCheckButton *) application->get_object("bars");
  GtkCheckButton* attractions = (GtkCheckButton *) application->get_object("attractions");
  GtkCheckButton* aquarium = (GtkCheckButton *) application->get_object("aquarium");
  GtkCheckButton* hotel = (GtkCheckButton *) application->get_object("hotel");
  GtkCheckButton* museum = (GtkCheckButton *) application->get_object("museum");
  GtkCheckButton* themePark = (GtkCheckButton *) application->get_object("theme park");
  GtkCheckButton* viewPoint = (GtkCheckButton *) application->get_object("view point");
  GtkCheckButton* zoo = (GtkCheckButton *) application->get_object("zoo");
  
  // All toggle signals using g_signal_connect
  
  //drive_walk
  //drive_only
  
  g_signal_connect(
        G_OBJECT(restaurant),
        "toggled",
        G_CALLBACK(toggleRestaurant),
        NULL
   );
  
   g_signal_connect(
        G_OBJECT(fastFood),
        "toggled",
        G_CALLBACK(toggleFastFood),
        NULL
   );
   
   g_signal_connect(
        G_OBJECT(cafe),
        "toggled",
        G_CALLBACK(toggleCafe),
        NULL
   ); 
  
   g_signal_connect(
        G_OBJECT(bars),
        "toggled",
        G_CALLBACK(toggleBars),
        NULL
   ); 
   
    g_signal_connect(
        G_OBJECT(attractions),
        "toggled",
        G_CALLBACK(toggleAttractions),
        NULL
   ); 
    
  g_signal_connect(
    G_OBJECT(aquarium),
    "toggled",
    G_CALLBACK(toggleAquarium),
    NULL
  ); 
     
  g_signal_connect(
    G_OBJECT(hotel),
    "toggled",
    G_CALLBACK(toggleHotel),
    NULL
  ); 
  
  g_signal_connect(
    G_OBJECT(museum),
    "toggled",
    G_CALLBACK(toggleMuseum),
    NULL
  ); 

  g_signal_connect(
    G_OBJECT(themePark),
    "toggled",
    G_CALLBACK(toggleThemePark),
    NULL
  ); 
  
  g_signal_connect(
    G_OBJECT(viewPoint),
    "toggled",
    G_CALLBACK(toggleViewPoint),
    NULL
  ); 

}

//Main function to draw the map based on inputs from API calls
int draw_map(unordered_map<std::string, std::string> map) {
    
    ezgl::application::settings settings;
    
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";
    
    ezgl::application application(settings);

    n->setWayOSMIDs();
    std::string response;
    
    
    application.add_canvas("MainCanvas", draw_main_canvas, n->initial_world());
    
    application.run(initial_setup, act_on_mouse_click, act_on_mouse_move, act_on_key_press);
    
    //Clean-up the map data and related data structures
        std::cout << "Closing map\n";
        close_map();
    
    std::string map_path;
    do{
        
        //Load another map via command prompt if the user wants to]
        std::cout << "Do you wish to look at another city map?" << endl;
        std::cout << "Type: 'Yes' or 'No': ";
        
        std::cin >> response;
        std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout <<response << endl;
        
        if(response== "no" || response == "No"){ return 0;}
        
        //Text for choosing map
        std::cout << "Choose a city map that you wish to load: " << std::endl;
        std::cout << "Available city options: " << std::endl;
        
        int k = 0;
        //Print out city names for viewing/choosing purposes on Command Prompt
        for (auto& x: map) {
            k++;
            std::cout << x.first << "    " ;
                    
            if(k % 6 == 0){ std::cout << std::endl;}
        }
        
        std::cout << "Enter map name or specific map path: " << std::endl;
        string mapName = "";

        std::cin >> mapName;
        std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n'); 
        
        std::for_each(mapName.begin(), mapName.end(), [](char & c){c = ::tolower(c);});

        cout << mapName << " map chosen" << endl;

        auto it = map.find(mapName);

        // City Name entered correctly
        if (it != map.end()){
            map_path = it->second;
        }

        else{
            map_path = mapName;
        }

        //Load the map and related data structures
        bool load_success = load_map(map_path);
        if(!load_success) {
            std::cerr << "Failed to load map '" << map_path << "'\n";
            return 1;
        }
        
         std::cout << "Successfully loaded map '" << map_path << "'\n";
         n->setWayOSMIDs();
      application.change_canvas_world_coordinates("MainCanvas", n->initial_world());
      application.refresh_drawing();
      application.run(initial_setup, act_on_mouse_click, act_on_mouse_move, act_on_key_press);
       
      //Clean-up the map data and related data structures
        std::cout << "Closing map\n";
        close_map();
        n->clearOSMPtr();
      
    }while(response == "Yes" || response == "yes");   
    
    return 0;
}

/*
#include <chrono>
#include <thread>

#define VISUALIZE // Comment out to turn off

#ifdef VISUALIZE
    // Draw edge (streetSegment) we’re about to explore
    ezgl::renderer*g = application->get_renderer();    
    highlightStreetSegment (g, streetSegID, ezgl:BLUE); 
    application->flush_drawing();  // Draw right now!   
    delay (50);  // wait for 50 ms so we can see progress
#endif      

...

void delay (int milliseconds) {// Pause for millisecond
    std::chrono::milliseconds duration (milliseconds);
    std::this_thread::sleep_for (duration);
}

 */