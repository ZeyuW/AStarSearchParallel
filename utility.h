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
#include <algorithm>
#include <limits.h>
#include <omp.h>
#include <unistd.h>
#include <functional>

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

	bool operator==(const Point& p) const{
		return (x == p.x && y == p.y);
	}
};

namespace std{
	template<>
	struct hash<Point>{
		std::size_t operator()(const Point& p) const{
			return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
		}
	};
}


struct UCS_Point{
	int x;
	int y;
	int cost;
	UCS_Point() : x(0), y(0){};
	UCS_Point(int in_x, int in_y, int in_cost) : x(in_x), y(in_y), cost(in_cost){};
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
