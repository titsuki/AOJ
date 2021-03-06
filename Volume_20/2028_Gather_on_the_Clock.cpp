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
#include <complex>
#include <assert.h>

using namespace std;

typedef long long ll;
typedef pair <int,int> P;

static const double EPS = 1e-12;

static const int tx[] = {+0,+1,+0,-1};
static const int ty[] = {-1,+0,+1,+0};

int dp[101][101];

int main(){
  int num_of_tests;
  while(~scanf("%d",&num_of_tests)){
    for(int test_i = 0; test_i < num_of_tests; test_i++){
      int num_of_cards;
      scanf("%d",&num_of_cards);
      vector<int> cards;
      for(int card_i = 0; card_i < num_of_cards; card_i++){
	int card;
	scanf("%d",&card);
	cards.push_back(card);
      }

      memset(dp,0,sizeof(dp));
      for(int len = 1; len <= cards.size(); len++){
	for(int pos = 0; pos < cards.size(); pos++){
	  for(int mid = 0; mid < len; mid++){
	    dp[pos][len] = max(dp[pos][len],
			       dp[pos][mid]
			       + dp[(pos + mid) % cards.size()][len - mid]
			       + abs(cards[pos] - cards[(pos + mid) % cards.size()]));
	  }
	}
      }

      int res = 0;
      for(int pos = 0; pos < cards.size(); pos++){
	res = max(dp[pos][cards.size()],res);
      }
      printf("%d\n",res);
    }
  }
}
