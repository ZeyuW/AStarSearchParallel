#include "A_Star.h"

using namespace std;

void handle_infile(string& file_name, vector<vector<Location>>& map_v,
                   int& start_x, int& start_y, int& end_x, int& end_y){
    ifstream map_file(file_name);
    string line;
    int rows = 0, columns = 0;
    if (map_file.is_open()){
        map_file >> rows >> columns;
        getline(map_file, line);
        int line_num = 0;
        while (getline(map_file, line)){
            map_v.push_back({});
            for (int i = 0; i < line.length(); i++){
                map_v.back().push_back(Location(' ', line[i]));
                if (line[i] == 'S'){
                    start_x = line_num;
                    start_y = i;
                }
                else if (line[i] == 'G'){
                    end_x = line_num;
                    end_y = i;
                }
            }
            line_num++;
        }
    }
    else{
        cerr << "ERROR: Failed to open the map file" << endl;
        exit(1);
    }
}

int main(int argc, const char * argv[]) {
    vector<vector<Location>> map_v;
    
    // parse input files and save results into map_v
    /**************** Part 1 ****************/

    // Change the filename here if you want to run Part1 on a different map file
    string file_name = "large_map2.txt";
    int start_x, start_y, end_x, end_y;
    handle_infile(file_name, map_v, start_x, start_y, end_x, end_y);
    
    // Part 2B: A_Star Search
    A_Star astar(map_v, start_x, start_y, end_x, end_y, file_name, 0, 1);
    astar.search_path();
    astar.print_result();
    
    // Part 2C: Different Heuristics
    // Change the filename here if you want to run Part2C on a different map file
    file_name = "large_map2.txt";
    map_v.clear();
    handle_infile(file_name, map_v, start_x, start_y, end_x, end_y);
    
    cout << "**************** Different Heuristics of A* on " << file_name << " ****************" << endl;
    cout << "# Nodes Examined: " << endl;
    A_Star zero(map_v, start_x, start_y, end_x, end_y, file_name, 1, 0);
    zero.search_path();
    zero.print_result();
    
    A_Star manh(map_v, start_x, start_y, end_x, end_y, file_name, 0, 0);
    manh.search_path();
    manh.print_result();
    
    A_Star east(map_v, start_x, start_y, end_x, end_y, file_name, 2, 0);
    east.search_path();
    east.print_result();
    
    A_Star wall(map_v, start_x, start_y, end_x, end_y, file_name, 3, 0);
    wall.search_path();
    wall.print_result();
    
    return 0;
}

