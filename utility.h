#ifndef utility_h
#define utility_h

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <queue>
#include <utility>
#include <math.h>
#include <unordered_map>

using namespace std;

struct Location{
    char pre_dir;
    char val;
    Location(char in_dir, char in_val) : pre_dir(in_dir), val(in_val){};
};

struct Point{
    int x;
    int y;
    Point() : x(0), y(0){};
    Point(int in_x, int in_y) : x(in_x), y(in_y){};
};


struct Node_State{
    int x;
    int y;
    int health;
    Node_State(int in_x, int in_y, int in_h) : x(in_x), y(in_y), health(in_h){};
    
    bool operator==(const Node_State &other) const{
        return (x == other.x && y == other.y && health == other.health);
    }
};


struct AS_Location{
    char pre_dir;
    int pre_health;
    int pre_cost;
    char val;
    AS_Location() : pre_dir(' '), pre_health(5), pre_cost(0), val(' '){};
    AS_Location(char in_dir, int in_pre_h, int in_cost, char in_val) : pre_dir(in_dir), pre_health(in_pre_h), pre_cost(in_cost), val(in_val){};
};

struct AS_Point{
    int x;
    int y;
    int cost;
    int dis;
    int health;
    char dir;
    AS_Point() : x(0), y(0), cost(0), dis(0), health(5), dir(' '){};
    AS_Point(int in_x, int in_y) : x(in_x), y(in_y), cost(0), dis(0), health(5){};
    AS_Point(int in_x, int in_y, int in_cost, int in_dis, int in_h) : x(in_x), y(in_y),
    cost(in_cost), dis(in_dis), health(in_h), dir(' '){};
    AS_Point(int in_x, int in_y, int in_cost, int in_dis, int in_h, int in_dir) : x(in_x), y(in_y),
    cost(in_cost), dis(in_dis), health(in_h), dir(in_dir){};
};


#endif /* utility_h */
