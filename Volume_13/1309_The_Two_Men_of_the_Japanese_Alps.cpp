#define _USE_MATH_DEFINES
#define INF 0x3f3f3f3f
#include <cstdio>
#include <iostream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <stack>
#include <limits>
#include <map>
#include <string>
#include <cstring>
#include <set>
#include <deque>
#include <bitset>
#include <list>
#include <cctype>
#include <utility>
#include <complex>
 
using namespace std;
 
typedef long long ll;
typedef pair <int,int> P;
typedef pair <int,P > PP;

typedef complex<double> Point;
const static double EPS = 1e-12;

double dot(const Point& p,const Point& q){
  return real(conj(p)*q);
}

double det(const Point& p,const Point& q){
  return imag(conj(p)*q);
}

bool on_seg(const Point& p1,const Point&p2,
	    const Point& q){
  return abs(det(p1-q,p2-q)) <= EPS && dot(p1-q,p2-q) < EPS;
}

Point compute_intersection(const Point& p1,const Point& p2,
			   const Point& q1,const Point& q2) {
  return p1 + (p2-p1) * det(q2-q1,q1-p1) / det(q2-q1,p2-p1);
}

int ccw(Point a,Point b,Point c) {
  b -= a; c -= a;
  if (det(b, c) > 0)   return +1;       // counter clockwise
  if (det(b, c) < 0)   return -1;       // clockwise
  if (dot(b, c) < 0)     return +2;       // c--a--b on line
  if (norm(b) < norm(c)) return -2;       // a--b--c on line
  return 0;
}

bool intersectSS(const Point& p1, const Point& p2,
		 const Point& q1, const Point& q2) {
  return (ccw(p1,p2,q1) *
	  ccw(p1,p2,q2) <= 0 &&
	  ccw(q1,q2,p1) *
	  ccw(q1,q2,p2) <= 0);
}

double compute_distance(const Point& p,const Point &q){
  return abs(q-p);
}

bool is_parallel(const Point& p1, const Point& p2,
		 const Point& q1, const Point& q2){
  double m = compute_distance(p1,p2) * compute_distance(q1,q2);
  if(abs(dot(p1-p2,q1-q2)) - EPS <=  m && m <= abs(dot(p1-p2,q1-q2)) + EPS){
    return true;
  }
  if(abs(dot(p1-p2,q1-q2)) - EPS <=  -m && -m <= abs(dot(p1-p2,q1-q2)) + EPS){
    return true;
  }
  return false;
}


struct State {
  int lhs;
  int rhs;
  double cost;
  State(int l,int r,double c) : lhs(l),rhs(r),cost(c) {}
  bool operator<(const State& s) const {
    return cost < s.cost;
  }
  bool operator>(const State& s) const {
    return cost > s.cost;
  }
};

struct Node {
  Point point;
  bool is_origin;
  Node(Point p, bool o) : point(p), is_origin(o) {}
  bool operator<(const Node& n) const{
    return point.real() < n.point.real();
  }
  bool operator>(const Node& n) const{
    return point.real() > n.point.real();
  }
};

void disp(double next_cost,int lhs_i,int rhs_i){
  printf("lhs:%d rhs:%d cost: %lf\n",lhs_i,rhs_i,next_cost);
}

map<int,bool> dp[1000001];

int main(){
  int total_points;
  while(~scanf("%d",&total_points)){
    if(total_points == 0) break;

    for(int i=0;i<=1000000;i++){
      dp[i].clear();
    }
    set<double> height;
    vector<Node> routes;
    for(int point_i = 0; point_i < total_points; point_i++){
      double x,y;
      scanf("%lf %lf",&x,&y);
      height.insert(y);
      routes.push_back(Node(Point(x,y),true));
    }
    
    vector<Node> external;
    for(set<double>::iterator it = height.begin();
	it != height.end();
	it++){
      for(int route_i = 0; route_i+1 < routes.size(); route_i++){
	if(!intersectSS(routes[route_i].point,
			routes[route_i+1].point,
			Point(routes[route_i].point.real(),(double)*it),
			Point(routes[route_i+1].point.real(),(double)*it))){
	  continue;
	}
	if(is_parallel(routes[route_i].point,
		       routes[route_i+1].point,
		       Point(routes[route_i].point.real(),(double)*it),
		       Point(routes[route_i+1].point.real(),(double)*it))){
	  continue;
	}

	Point p = compute_intersection(routes[route_i].point,
				       routes[route_i+1].point,
				       Point(routes[route_i].point.real(),(double)*it),
				       Point(routes[route_i+1].point.real(),(double)*it));

	if(p.real() - EPS <= routes[route_i].point.real() 
	   && routes[route_i].point.real() <= p.real() + EPS){
	  continue;
	}
	if(p.real() - EPS <= routes[route_i+1].point.real() 
	   && routes[route_i+1].point.real() <= p.real() + EPS){
	  continue;
	}
	external.push_back(Node(p,false));
      }
    }

    for(int ext_i = 0; ext_i < external.size(); ext_i++){
      routes.push_back(external[ext_i]);
    }

    sort(routes.begin(),routes.end());
    priority_queue<State,vector<State>,greater<State> > que;
    que.push(State(0,routes.size() - 1,0.0));
    dp[0][routes.size()-1] = 0.0;

    double res = 0.0;
    while(!que.empty()){
      State s = que.top();
      que.pop();
      if(dp[s.lhs][s.rhs]) continue;
      dp[s.lhs][s.rhs] = true;
      if(s.lhs == s.rhs && routes[s.lhs].is_origin){
	res = s.cost;
	break;
      }

      //target
      for(int lhs_i = s.lhs - 1; lhs_i <= s.lhs + 1; lhs_i++){
    	if(lhs_i < 0 || lhs_i >= routes.size()) continue;
    	for(int rhs_i = s.rhs - 1; rhs_i <= s.rhs + 1; rhs_i++){
    	  if(rhs_i < 0 || rhs_i >= routes.size()) continue;
    	  if(abs(routes[rhs_i].point.imag() - routes[lhs_i].point.imag()) > EPS) continue;

    	  double next_cost = s.cost 
    	    + compute_distance(routes[s.lhs].point,routes[lhs_i].point)
    	    + compute_distance(routes[s.rhs].point,routes[rhs_i].point);
    	  if(!dp[lhs_i][rhs_i]){
    	    que.push(State(lhs_i,rhs_i,next_cost));
    	  }
    	}
      }
    }
    
    printf("%.3lf\n",res);
  }
}
