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
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "OSMID.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"

//Program exit codes
constexpr int SUCCESS_EXIT_CODE = 0;        //Everyting went OK
constexpr int ERROR_EXIT_CODE = 1;          //An error occured
constexpr int BAD_ARGUMENTS_EXIT_CODE = 2;  //Invalid command-line usage

using namespace std;

//The default map to load if none is specified
std::string default_map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
std::string default_map_path_osm = "/cad2/ece297s/public/maps/toronto_canada.osm.bin";

int main(int argc, char** argv) {
    
    std::string map_path;
    if(argc == 1) {
        //Use a default map
        map_path = default_map_path;
    } else if (argc == 2) {
        //Get the map from the command line
        map_path = argv[1];
    } else {
        //Invalid arguments
        std::cerr << "Usage: " << argv[0] << " [map_file_path]\n";
        std::cerr << "  If no map_file_path is provided a default map is loaded.\n";
        return BAD_ARGUMENTS_EXIT_CODE;
    }
    
    // Unordered map to pair cities with paths
    std::unordered_map<string, string> mapsAndPathPair;
    
    // Vector of city names
    std::vector<std::string> cityMapsList{
        "beijing", "cairo", "cape-town", "golden-horseshoe",
        "hamilton", "hong-kong", "iceland", "london", "moscow", "new-delhi",
        "new-york", "rio-de-janeiro", "saint-helena", "singapore", "sydney",
        "tehran", "tokyo", "toronto"
    };
    
    // Vector of city paths
    std::vector<std::string> cityMapsPath{
        "/cad2/ece297s/public/maps/beijing_china.streets.bin",
        "/cad2/ece297s/public/maps/cairo_egypt.streets.bin",
        "/cad2/ece297s/public/maps/cape-town_south-africa.streets.bin",
        "/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin",
        "/cad2/ece297s/public/maps/hamilton_canada.streets.bin",
        "/cad2/ece297s/public/maps/hong-kong_china.streets.bin",
        "/cad2/ece297s/public/maps/iceland.streets.bin",
        "/cad2/ece297s/public/maps/london_england.streets.bin",
        "/cad2/ece297s/public/maps/moscow_russia.streets.bin",
        "/cad2/ece297s/public/maps/new-delhi_india.streets.bin",
        "/cad2/ece297s/public/maps/new-york_usa.streets.bin",
        "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin",
        "/cad2/ece297s/public/maps/saint-helena.streets.bin",
        "/cad2/ece297s/public/maps/singapore.streets.bin",
        "/cad2/ece297s/public/maps/sydney_australia.streets.bin",
        "/cad2/ece297s/public/maps/tehran_iran.streets.bin",
        "/cad2/ece297s/public/maps/tokyo_japan.streets.bin",
        "/cad2/ece297s/public/maps/toronto_canada.streets.bin"
    };
    
    // Pair cities and paths
    for (int i = 0; i < cityMapsList.size(); i++) {
        mapsAndPathPair.insert(std::make_pair(cityMapsList[i], cityMapsPath[i]));
    }
    int k = 0;
    string response = "Yes";
    
    //Text for choosing map
        cout << "Choose a city map that you wish to load: " << endl;
        cout << "Available city options: " <<endl;
        
        //Print out city names for viewing/choosing purposes on Command Prompt
        for (int i = 0; i < cityMapsList.size(); i+=6) {
            cout << "    " << cityMapsList[i] << "    " << cityMapsList[i+1] 
                 << "    " << cityMapsList[i+2] << "    " << cityMapsList[i+3]
                 << "    " << cityMapsList[i+4] << "    " << cityMapsList[i+5]
                 << endl;
        }
        
        cout << "Enter map name or specific map path: " << endl;
        string mapName = "";

        cin >> mapName;
        cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n'); 
        
        std::for_each(mapName.begin(), mapName.end(), [](char & c){c = ::tolower(c);});

        cout << mapName << " map chosen" << endl;

        auto it = mapsAndPathPair.find(mapName);

         //City Name entered correctly
        if (it != mapsAndPathPair.end()){
            map_path = it->second;
        }

        else{
            map_path = mapName;
        }

        //Load the map and related data structures
        bool load_success = load_map(map_path);
        if(!load_success) {
            std::cerr << "Failed to load map '" << map_path << "'\n";
            return ERROR_EXIT_CODE;
        }

        std::cout << "Successfully loaded map '" << map_path << "'\n";
    
    k = draw_map(mapsAndPathPair);
   
    
    return k;
}
