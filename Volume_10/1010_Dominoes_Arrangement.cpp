#define _USE_MATH_DEFINES
#define INF 0x3f3f3f3f
#include <iostream>
#include <sstream>
#include <cmath>
#include <cstdio>
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
 
using namespace std;
 
typedef long long ll;
typedef pair <int,int> P;
 
static const double EPS = 1e-8;

static const int tx[] = {0,1,0,-1};
static const int ty[] = {-1,0,1,0};

class Domino {
private:
  int _head;
  int _tail;
  bool _can_connect_head;
  bool _can_connect_tail;

public:
  Domino(int head, int tail) {
    _head = head;
    _tail = tail;
    _can_connect_head = true;
    _can_connect_tail = true;
  }

  int detect_connect_pattern(Domino d) {
    int res = 0;
    if(d.get_head() == _head
       && d.can_connect_head() && can_connect_head()){
      return (res | (1<<0));
    }
    if(d.get_head() == _tail
       && d.can_connect_head() && can_connect_tail()){
      return (res | (1<<1));
    }
    if(d.get_tail() == _head
       && d.can_connect_tail() && can_connect_head()){
      return (res | (1<<2));
    }
    if(d.get_tail() == _tail
       && d.can_connect_tail() && can_connect_tail()){
      return (res | (1<<3));
    }
    return res;
  }

  Domino connect(Domino d,int pattern){
    if(pattern == (1<<0)){
      return Domino(d.get_tail(),_tail);
    }
    if(pattern == (1<<1)){
      return Domino(d.get_tail(),_head);
    }
    if(pattern == (1<<2)){
      return Domino(d.get_head(),_tail);
    }
    if(pattern == (1<<3)){
      return Domino(d.get_head(),_head);
    }
    return *this;
  }

  int get_head() const{
    return _head;
  }

  int get_tail() const{
    return _tail;
  }

  bool can_connect_head() const{
    return _can_connect_head;
  }

  bool can_connect_tail() const{
    return _can_connect_tail;
  }

};

int main(){
  int n;
  while(~scanf("%d",&n)){
    int degree[8];
    vector<Domino> dominos;

    int same_head_tail = 0;
    for(int i = 0; i < n; i++){
      char domino[8];
      scanf("%s",domino);
      if(domino[0] - '0' == domino[1] - '0'){
	same_head_tail |= (1 << (domino[1] - '0'));
	continue;
      }
      dominos.push_back(Domino(domino[0] - '0',domino[1] - '0'));
    }

    bool res = true;
    for(int num = 0; num <= 6; num++){
      if(same_head_tail & (1 << num)){
	bool isok = false;
	for(int i = 0; i < dominos.size(); i++){
	  if(dominos[i].get_tail() == num
	     || dominos[i].get_head() == num){
	    isok = true;
	  }
	}
	if(!isok){
	  res = false;
	  break;
	}
      }
    }

    for(int S = 0; S <= (1<<dominos.size()); S++){
      for(int from_i = 0; from_i < dominos.size(); from_i++){
	for(int to_i = 0; to_i < dominos.size(); to_i++){
	  
	}
      }
    }
  }
}
