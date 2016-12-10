#include "A_Star.h"

using namespace std;


struct comp{
	bool operator()(UCS_Point& p1, UCS_Point& p2){
		if (p1.cost == p2.cost){
			if (p1.x == p2.x){
				return p1.y > p2.y;
			}
			return p1.x > p2.x;
		}
		return p1.cost > p2.cost;
	}
};

void handle_infile(string& file_name, vector<vector<Location>>& map_v,
                   vector<Point>& way_points, vector<Point>& fully_way_points, 
				   int& start_x, int& start_y,int& end_x, int& end_y){
    ifstream map_file(file_name);
    string line;
    int rows = 0, columns = 0;
    if (map_file.is_open()){
        map_file >> rows >> columns;
        getline(map_file, line);
        int line_num = 0;
        while (getline(map_file, line)){
            map_v.push_back({});
            for (int i = 0; i < (int)line.length(); i++){
                map_v.back().push_back(Location(' ', line[i]));
				// add way points
				if (line_num % 2 == 0 && i % 2 == 0)
					way_points.push_back(Point(line_num, i));
				fully_way_points.push_back(Point(line_num, i));
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



// For waypoints
void handle_waypoints(vector<vector<Location>> in_map_v, vector<Point> way_points, unordered_map<Point, int>& way_points_map, int end_x, int end_y){
	int rows = (int)in_map_v.size();
	int columns = (int)in_map_v[0].size();
	
	omp_lock_t way_point_lock, way_point_map_lock;
	omp_init_lock(&way_point_lock);
	omp_init_lock(&way_point_map_lock);

	// Way point timer
	double waypoint_start = omp_get_wtime();

#pragma omp parallel
	{
		while (1){
			omp_set_lock(&way_point_lock);
			if (way_points.empty()){
				omp_unset_lock(&way_point_lock);
				break;
			}
			Point cur_point = way_points.back();
			way_points.pop_back();
			omp_unset_lock(&way_point_lock);
			vector<vector<Location>> map_v = in_map_v;

			int path_cost = 1;
			// priority_queue 
			priority_queue<UCS_Point, vector<UCS_Point>, comp> pq;
			// explored set
			vector<vector<int>> explored(rows, vector<int>(columns, INT_MAX));
			// Initialize first point
			map_v[cur_point.x][cur_point.y].pre_dir = 'X';
			map_v[cur_point.x][cur_point.y].val = ' ';
			pq.push(UCS_Point(cur_point.x, cur_point.y, 0));
			explored[cur_point.x][cur_point.y] = 0;
			
			while (!pq.empty()){
				UCS_Point cur = pq.top();
				pq.pop();

				if (explored[cur.x][cur.y] != cur.cost) continue;
				if (cur.x == end_x && cur.y == end_y) break;

				int cur_cost = 1;
				if ((map_v[cur.x][cur.y]).val == '*') cur_cost = 3;
				
				
				if (cur.x + 1 < rows && explored[cur.x + 1][cur.y] > cur.cost + cur_cost
					&& (map_v[cur.x + 1][cur.y]).val != 'X'){
					explored[cur.x + 1][cur.y] = cur.cost + cur_cost;
					pq.push(UCS_Point(cur.x + 1, cur.y, cur.cost + cur_cost));
					(map_v[cur.x + 1][cur.y]).pre_dir = 'S';
				}
				if (cur.y + 1 < columns && explored[cur.x][cur.y + 1] > cur.cost + cur_cost
					&& (map_v[cur.x][cur.y + 1]).val != 'X'){
					explored[cur.x][cur.y + 1] = cur.cost + cur_cost;
					pq.push(UCS_Point(cur.x, cur.y + 1, cur.cost + cur_cost));
					
					(map_v[cur.x][cur.y + 1]).pre_dir = 'E';
				}
				if (cur.y - 1 >= 0 && explored[cur.x][cur.y - 1] > cur.cost + cur_cost
					&& (map_v[cur.x][cur.y - 1]).val != 'X'){
					explored[cur.x][cur.y - 1] = cur.cost + cur_cost;
					pq.push(UCS_Point(cur.x, cur.y - 1, cur.cost + cur_cost));
					
					(map_v[cur.x][cur.y - 1]).pre_dir = 'W';
				}
				if (cur.x - 1 >= 0 && explored[cur.x - 1][cur.y] > cur.cost + cur_cost
					&& (map_v[cur.x - 1][cur.y]).val != 'X'){
					explored[cur.x - 1][cur.y] = cur.cost + cur_cost;
					pq.push(UCS_Point(cur.x - 1, cur.y, cur.cost + cur_cost));
					
					(map_v[cur.x - 1][cur.y]).pre_dir = 'N';
				}
			} // end while

			
			int x = end_x, y = end_y;
			while (x >= 0 && y >= 0 && map_v[x][y].pre_dir != 'X'){
				if (map_v[x][y].val == ' ') path_cost++;
				else if (map_v[x][y].val == '*') path_cost += 3;
				
				if (map_v[x][y].pre_dir == 'N') x++;
				else if (map_v[x][y].pre_dir == 'W') y++;
				else if (map_v[x][y].pre_dir == 'E') y--;
				else x--;
			}

			// insert path_cost into map
			omp_set_lock(&way_point_map_lock);
			way_points_map[cur_point] = path_cost;
			omp_unset_lock(&way_point_map_lock);
		} // end of idle loop	
	} // end of parallel part
	double waypoint_end = omp_get_wtime();
	cout << "Finish generating waypoints: " << 1000 * (waypoint_end - waypoint_start) << "ms" << endl;
}




int main(int argc, const char * argv[]) {
	if (argc < 2){
		cerr << "Usage: <program> <#cores>" <<endl;
		exit(1);
	}
	int num_threads = atoi(argv[1]);
	omp_set_num_threads(num_threads);

    vector<vector<Location>> map_v;

	// Waypoints
	vector<Point> way_points;
	vector<Point> fully_way_points;
	unordered_map<Point, int> way_points_map;
	unordered_map<Point, int> fully_way_points_map;
    
    // parse input files and save results into map_v

	cout << "Program start: core number: " << omp_get_num_threads() << endl;
    // Change the filename here if you want to run Part1 on a different map file
    string file_name = "large_map2.txt";
    int start_x, start_y, end_x, end_y;
    handle_infile(file_name, map_v, way_points, fully_way_points, start_x, start_y, end_x, end_y);

	// Handle waypoints
	handle_waypoints(map_v, way_points, way_points_map, end_x, end_y);
	handle_waypoints(map_v, fully_way_points, fully_way_points_map, end_x, end_y);
    
    A_Star astar(map_v, start_x, start_y, end_x, end_y, file_name, way_points_map, 0);
    astar.search_path();
    astar.print_result();
	
    A_Star zero(map_v, start_x, start_y, end_x, end_y, file_name, way_points_map, 1);
    zero.search_path();
    zero.print_result();
    
    A_Star manh(map_v, start_x, start_y, end_x, end_y, file_name, way_points_map, 0);
    manh.search_path();
    manh.print_result();
    
    A_Star east(map_v, start_x, start_y, end_x, end_y, file_name, way_points_map, 2);
    east.search_path();
    east.print_result();
    
    A_Star wall(map_v, start_x, start_y, end_x, end_y, file_name, way_points_map, 3);
    wall.search_path();
    wall.print_result();

    A_Star waypoints(map_v, start_x, start_y, end_x, end_y, file_name, way_points_map, 4);
    waypoints.search_path();
    waypoints.print_result();
    
    A_Star fully_waypoints(map_v, start_x, start_y, end_x, end_y, file_name, fully_way_points_map, 5);
    fully_waypoints.search_path();
    fully_waypoints.print_result();
    return 0;
}

