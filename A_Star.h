#ifndef A_STAR_H
#define A_STAR_H

#include "utility.h"
using namespace std;


class A_Star{
    // comparator
    struct comp{
        bool operator()(AS_Point& p1, AS_Point& p2){
            if (p1.cost + p1.dis == p2.cost + p2.dis){
                if (p1.x == p2.x){
                    if (p1.y == p2.y){
                        return p1.health < p2.health;
                    }
                    return p1.y > p2.y;
                }
                return p1.x > p2.x;
            }
            return p1.cost + p1.dis > p2.cost + p2.dis;
        }
    };
    
    // variables:
    /*
     * flag specifies which heuristic function to be used:
     *     0: Manhattan Distance
     *     1: Zero Heuristic
     *     2: Move Horizontally
     *     3: Wall
     * verbose specifies if print out 2b result
     */
    int flag;
    int verbose;
    vector<vector<vector<AS_Location>>> map_v;
    AS_Point start, end;
    int rows, columns;
    string path;
    int path_cost;
    int node_examine;
    int final_health;
    string file_name;
    priority_queue<AS_Point, vector<AS_Point>, comp> s;
    vector<vector<vector<int>>> explored;
    
    // member functions
    double getDis(AS_Point& p){
        double distance = 0;
        switch (flag){
            case 0: // Manhattan Distance
                distance = abs(p.x - end.x) + abs(p.y - end.y);
                break;
            case 1: // Zero Heuristic
                distance = 0;
                break;
            case 2: // Move Horizontally
                distance = abs(p.x - end.x) / (4.0) + abs(p.y - end.y);
                break;
            case 3: // Wall
                if ((p.y + 1 < columns && map_v[p.x][p.y + 1][0].val != 'X'
                    && map_v[p.x][p.y + 1][0].val != 'O') || p.y == columns - 1){
                    distance = abs(p.x - end.x) + abs(p.y - end.y);
                }
                else{
                    // go up and find first open tile
                    int north_open = p.x, north_manh = 0, up = p.x - 1;
                    for (; up >= 0; up--){
                        if (p.y + 1 < columns && map_v[up][p.y + 1][0].val != 'X'
                            && map_v[up][p.y + 1][0].val != 'O'){
                            north_open = up;
                            break;
                        }
                    }
                    north_manh = (up == -1)?
                        INT_MAX : abs(p.x - up) + abs(end.y - p.y) + abs(end.x - up);
                    
                    // go down and find first open tile
                    int south_open = p.x, south_manh = 0, down = p.x + 1;
                    for (; down < rows; down++){
                        if (p.y + 1 < columns && map_v[down][p.y + 1][0].val != 'X'
                            && map_v[down][p.y + 1][0].val != 'O'){
                            south_open = down;
                            break;
                        }
                    }
                    south_manh = (down == rows)?
                        INT_MAX : abs(down - p.x) + abs(end.y - p.y) + abs(down - end.x);
                    distance = min(north_manh, south_manh);
                    if (distance == INT_MAX) distance = abs(p.x - end.x) + abs(p.y - end.y);
                }
                break;
            default: // ERROR
                distance = 0;
                break;
        }
        return distance;
    }
    
    void update_health(AS_Point& cur){
        int damage = ((map_v[cur.x][cur.y][0]).val == ' ') ? 1 : 3;
        if (cur.x - 1 >= 0 && (map_v[cur.x - 1][cur.y][0]).val == 'O') cur.health -= damage;
        if (cur.y - 1 >= 0 && (map_v[cur.x][cur.y - 1][0]).val == 'O') cur.health -= damage;
        if (cur.x + 1 < rows && (map_v[cur.x + 1][cur.y][0]).val == 'O') cur.health -= damage;
        if (cur.y + 1 < columns && (map_v[cur.x][cur.y + 1][0]).val == 'O') cur.health -= damage;
    }
public:
    A_Star(vector<vector<Location>>& in_map_v, int in_s_x, int in_s_y,
               int in_e_x, int in_e_y, string& in_f_name, int in_flag, int in_verbose){
        rows = (int)in_map_v.size();
        columns = (int)in_map_v[0].size();
        
        map_v = vector<vector<vector<AS_Location>>>(rows, vector<vector<AS_Location>>(columns, vector<AS_Location>()));
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < columns; j++){
                map_v[i][j] = vector<AS_Location>(6, AS_Location((in_map_v[i][j]).pre_dir, 5, 0, (in_map_v[i][j]).val));
            }
        }
        explored = vector<vector<vector<int>>>(rows, vector<vector<int>>(columns, vector<int>(6, INT_MAX)));
        
        start = AS_Point(in_s_x, in_s_y);
        end = AS_Point(in_e_x, in_e_y);
        start.dis = getDis(start);
        path_cost = 1;
        node_examine = 0;
        file_name = in_f_name;
        flag = in_flag;
        verbose = in_verbose;
    }
    
    void search_path(){
        map_v[start.x][start.y][5].pre_dir = 'X';
        s.push(start);
        explored[start.x][start.y][start.health] = start.cost + start.dis;
        
        while (!s.empty()){
            AS_Point cur = s.top();
            s.pop();
            
            if (explored[cur.x][cur.y][cur.health] != cur.cost + cur.dis) continue;
            explored[cur.x][cur.y][cur.health] = INT_MIN;
            
            int cur_cost = 1;
            if ((map_v[cur.x][cur.y][cur.health]).val == '*') cur_cost = 3;
            
            
            // update health
            int old_health = cur.health;
            update_health(cur);
            
            //save state of current point
            if (cur.health <= 0) continue;
            node_examine++;
            if (cur.x == end.x && cur.y == end.y){
                final_health = cur.health;
                break;
            }

            if (cur.x - 1 >= 0){
                AS_Point tmp(cur.x - 1, cur.y);
                if (explored[cur.x - 1][cur.y][cur.health] > cur.cost + cur_cost + getDis(tmp)
                    && (map_v[cur.x - 1][cur.y][cur.health]).val != 'X' && (map_v[cur.x - 1][cur.y][cur.health]).val != 'O'){
                    
                    explored[cur.x - 1][cur.y][cur.health] = cur.cost + cur_cost + getDis(tmp);
                    s.push(AS_Point(cur.x - 1, cur.y, cur.cost + cur_cost, getDis(tmp), cur.health, 'N'));
                    (map_v[cur.x - 1][cur.y][cur.health]).pre_dir = 'N';
                    (map_v[cur.x - 1][cur.y][cur.health]).pre_health = old_health;
                    
                }
            }
            if (cur.y - 1 >= 0){
                AS_Point tmp(cur.x, cur.y - 1);
                if (explored[cur.x][cur.y - 1][cur.health] > cur.cost + cur_cost + getDis(tmp)
                    && (map_v[cur.x][cur.y - 1][cur.health]).val != 'X' && (map_v[cur.x][cur.y - 1][cur.health]).val != 'O'){
                    
                    explored[cur.x][cur.y - 1][cur.health] = cur.cost + cur_cost + getDis(tmp);
                    s.push(AS_Point(cur.x, cur.y - 1, cur.cost + cur_cost, getDis(tmp), cur.health, 'W'));
                    (map_v[cur.x][cur.y - 1][cur.health]).pre_dir = 'W';
                    (map_v[cur.x][cur.y - 1][cur.health]).pre_health = old_health;
                   
                }
            }
            if (cur.y + 1 < columns){
                AS_Point tmp(cur.x, cur.y + 1);
                if (explored[cur.x][cur.y + 1][cur.health] > cur.cost + cur_cost + getDis(tmp)
                    && (map_v[cur.x][cur.y + 1][cur.health]).val != 'X' && (map_v[cur.x][cur.y + 1][cur.health]).val != 'O'){
                    
                    explored[cur.x][cur.y + 1][cur.health] = cur.cost + cur_cost + getDis(tmp);
                    s.push(AS_Point(cur.x, cur.y + 1, cur.cost + cur_cost, getDis(tmp), cur.health, 'E'));
                    (map_v[cur.x][cur.y + 1][cur.health]).pre_dir = 'E';
                    (map_v[cur.x][cur.y + 1][cur.health]).pre_health = old_health;
                    
                }
            }
            if (cur.x + 1 < rows){
                AS_Point tmp(cur.x + 1, cur.y);
                if (explored[cur.x + 1][cur.y][cur.health] > cur.cost + cur_cost + getDis(tmp)
                    && (map_v[cur.x + 1][cur.y][cur.health]).val != 'X' && (map_v[cur.x + 1][cur.y][cur.health]).val != 'O'){
                    
                    explored[cur.x + 1][cur.y][cur.health] = cur.cost + cur_cost + getDis(tmp);
                    s.push(AS_Point(cur.x + 1, cur.y, cur.cost + cur_cost, getDis(tmp), cur.health, 'S'));
                    (map_v[cur.x + 1][cur.y][cur.health]).pre_dir = 'S';
                    (map_v[cur.x + 1][cur.y][cur.health]).pre_health = old_health;
                    
                }
            }
            
        }
    }
    
    void print_result(){
        int x = end.x, y = end.y, h = final_health;
        
        while (map_v[x][y][h].pre_dir != 'X'){
            if (x == start.x && y == start.y) break;
            
            path += map_v[x][y][h].pre_dir;
            if (map_v[x][y][h].val == ' ') path_cost++;
            else if (map_v[x][y][h].val == '*') path_cost += 3;
            
            if (map_v[x][y][h].pre_dir == 'N'){
                h = map_v[x][y][h].pre_health;
                x++;
            }
            else if (map_v[x][y][h].pre_dir == 'W'){
                h = map_v[x][y][h].pre_health;
                y++;
            }
            else if (map_v[x][y][h].pre_dir == 'E'){
                h = map_v[x][y][h].pre_health;
                y--;
            }
            else{
                h = map_v[x][y][h].pre_health;
                x--;
            }
        }
        
        reverse(path.begin(), path.end());
        if (flag == 0 && verbose){
            cout << "**************** A* on "<< file_name << " with Manhattan Distance heuristic ****************" << endl;
            cout << "Path: " << path << endl;
            cout << "Path Length: " << path.length() << endl;
            cout << "Path Cost: " << path_cost << endl;
            cout << "# Nodes Examined: " << node_examine << endl;
            cout << endl;
        }
        else{
            switch (flag){
                case 0:
                    cout << "2 Manh: " << node_examine << endl;
                    break;
                case 1:
                    cout << "1 Zero: " << node_examine << endl;
                    break;
                case 2:
                    cout << "3 East: " << node_examine << endl;
                    break;
                case 3:
                    cout << "4 Wall: " << node_examine << endl;
                    break;
                default:
                    cout << "error" << endl;
                    break;
            }
        }
    }
    
};


#endif