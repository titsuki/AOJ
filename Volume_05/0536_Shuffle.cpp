#define _USE_MATH_DEFINES
#define INF 0x3f3f3f3f

#include <iostream>
#include <cstdio>
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
  
using namespace std;
  
typedef long long ll;
typedef pair <int,int> P;
  
static const double EPS = 1e-8;
  
static const int tx[] = {0,1,0,-1};
static const int ty[] = {-1,0,1,0};

int main(){
  int total_cards;
  while(~scanf("%d",&total_cards)){
    if(total_cards == 0) break;
    int total_shuffles;
    int first,last,upper;

    scanf("%d",&total_shuffles);
    scanf("%d %d %d",&first,&last,&upper);
    
    vector<P> cards;
    cards.push_back(P(1,total_cards));

    for(int shuffle_idx=0;shuffle_idx<total_shuffles;shuffle_idx++){
      int x,y;
      scanf("%d %d",&x,&y);
      vector<P> A;
      vector<P> B;
      vector<P> C;
      vector<P> next;

      int sum = 0;
      int prev_sum = 0;
      for(int j=0;j<cards.size();j++){
	prev_sum = sum;
	sum += cards[j].second - cards[j].first + 1;
	if(prev_sum < x){
	  if(sum <= x){
	    A.push_back(cards[j]);
	  }
	  else if(sum > x && sum <= y){
	    A.push_back(P(cards[j].first,cards[j].first + (x - prev_sum - 1)));
	    B.push_back(P(cards[j].first + (x - prev_sum),
			  cards[j].second));
	  }
	  else if(sum > y){
	    A.push_back(P(cards[j].first,cards[j].first + (x - prev_sum - 1)));
	    B.push_back(P(cards[j].first + (x - prev_sum),
			  cards[j].first + (x - prev_sum) + (y - x - 1)));
	    C.push_back(P(cards[j].first + (x - prev_sum) + (y - x),
			  cards[j].second));
	  }
	}
	else if(prev_sum >= x){
	  if(prev_sum < y){
	    if(sum <= y){
	      B.push_back(P(cards[j].first,cards[j].second));
	    }
	    else if(sum > y){
	      B.push_back(P(cards[j].first,cards[j].first + (y - prev_sum - 1)));
	      C.push_back(P(cards[j].first + (y - prev_sum),cards[j].second));
	    }
	  }
	  else if(prev_sum >= y){
	    C.push_back(P(cards[j].first,cards[j].second));
	  }
	}
      }
      for(int i=0;i<C.size();i++){
	next.push_back(C[i]);
      }
      for(int i=0;i<B.size();i++){
	next.push_back(B[i]);
      }
      for(int i=0;i<A.size();i++){
	next.push_back(A[i]);
      }
      cards = next;
    }

    int offset = 0;
    int res = 0;
    for(int i=0;i<cards.size();i++){
      int current = cards[i].second - cards[i].first + 1;
      if(offset + current < first){
	offset += current;
	continue;
      }
      if(offset > last){
	offset += current;
	continue;
      }

      int lhs = -1;
      int rhs = 1000000010;

      for(int round = 0; round < 50; round++){
	int mid = (lhs + rhs) / 2;
	if(cards[i].first + mid > upper){
	  rhs = mid;
	}
	if(cards[i].first + mid <= upper){
	  lhs = mid;
	}
      }

      int count = min(rhs,cards[i].second - cards[i].first + 1);

      if(offset + count >= last){
	count -= offset + count - last;
      }

      if(offset < first){
	count -= first - offset - 1;
      }

      if(count > 0){
	res += count;
      }

      offset += current;
    }
    printf("%d\n",res);

  }
}
