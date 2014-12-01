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
 
using namespace std;
 
typedef long long ll;
typedef pair <char,char> P;
typedef pair <int,P > PP;
 
const int tx[] = {+0,+1,+0,-1};
const int ty[] = {-1,+0,+1,+0};
 
static const double EPS = 1e-8;

struct ParseResult{
  int pos;
  int num;
  ParseResult(int pos,int num) : 
    pos(pos),num(num) {}
  ParseResult();
};

class Strategy {
public:
  virtual void calc(int& lhs,int rhs) = 0;
};

class AddStrategy : public Strategy{
public:
  void calc(int& lhs,int rhs){
    lhs += rhs;
  }
};

class SubStrategy : public Strategy{
public:
  void calc(int& lhs,int rhs){
    lhs -= rhs;
  }
};

class DivStrategy : public Strategy{
public:
  void calc(int& lhs,int rhs){
    lhs /= rhs;
  }
};

class MulStrategy : public Strategy{
public:
  void calc(int& lhs,int rhs){
    lhs *= rhs;
  }
};

class Processor {
private:
  Strategy* _strategy;
public:
  void setStrategy(char ope){
    if(ope == '+'){
      _strategy = new AddStrategy();
    }
    else if(ope == '-'){
      _strategy = new SubStrategy();
    }
    else if(ope == '*'){
      _strategy = new MulStrategy();
    }
    else if(ope == '/'){
      _strategy = new DivStrategy();
    }
  }
  void calc(int& lhs,int rhs){
    _strategy->calc(lhs,rhs);
  }
};

ParseResult expr(int pos,const string& str);
ParseResult expr(int pos,const string& str,char ope1);
ParseResult expr(int pos,const string& str,char ope1,char ope2);
ParseResult expr(int pos,const string& str,char ope1,char ope2,char ope3);
ParseResult factor(int pos,const string& str,char ope1);
ParseResult factor(int pos,const string& str,char ope1,char ope2);
ParseResult factor(int pos,const string& str,char ope1,char ope2,char ope3);
ParseResult term(int pos,const string& str);
ParseResult term(int pos,const string& str,char ope1);
ParseResult term(int pos,const string& str,char ope1,char ope2);
ParseResult term(int pos,const string& str,char ope1,char ope2,char ope3);
ParseResult num(int pos,const string& str);

vector<char> infer_ope(char ope1){
  bool used[256] = {};
  used[ope1] = true;
  vector<char> res;
  if(!used['+']) res.push_back('+');
  if(!used['-']) res.push_back('-');
  if(!used['*']) res.push_back('*');
  if(!used['/']) res.push_back('/');
  return res;
}

vector<char> infer_ope(char ope1,char ope2){
  bool used[256] = {};
  used[ope1] = true;
  used[ope2] = true;
  vector<char> res;
  if(!used['+']) res.push_back('+');
  if(!used['-']) res.push_back('-');
  if(!used['*']) res.push_back('*');
  if(!used['/']) res.push_back('/');
  return res;
}

vector<char> infer_ope(char ope1,char ope2,char ope3){
  bool used[256] = {};
  used[ope1] = true;
  used[ope2] = true;
  used[ope3] = true;
  vector<char> res;
  if(!used['+']) res.push_back('+');
  if(!used['-']) res.push_back('-');
  if(!used['*']) res.push_back('*');
  if(!used['/']) res.push_back('/');
  return res;
}

ParseResult expr(int pos,const string& str){
  ParseResult r = term(pos,str);
  while(r.pos < str.size()
	&& (str[r.pos] == '+' || str[r.pos] == '-' || str[r.pos] == '*' || str[r.pos] == '/')){
    ParseResult tmp = term(r.pos+1,str);
    if(str[r.pos] == '+'){
      Processor pr;
      pr.setStrategy('+');
      pr.calc(r.num,tmp.num);
    }
    if(str[r.pos] == '-'){
      Processor pr;
      pr.setStrategy('-');
      pr.calc(r.num,tmp.num);
    }
    if(str[r.pos] == '/'){
      Processor pr;
      pr.setStrategy('/');
      pr.calc(r.num,tmp.num);
    }
    if(str[r.pos] == '*'){
      Processor pr;
      pr.setStrategy('*');
      pr.calc(r.num,tmp.num);
    }
    r.pos = tmp.pos;
  }
  return r;
}

ParseResult expr(int pos,const string& str,char ope1){
  vector<char> rem = infer_ope(ope1);
  ParseResult r = factor(pos,str,rem[0],rem[1],rem[2]);
  while(r.pos < str.size()
	&& (str[r.pos] == ope1)){
    ParseResult tmp = factor(r.pos+1,str,rem[0],rem[1],rem[2]);
    if(str[r.pos] == ope1){
      Processor pr;
      pr.setStrategy(ope1);
      pr.calc(r.num,tmp.num);
    }
    r.pos = tmp.pos;
  }
  return r;
}

ParseResult expr(int pos,const string& str,char ope1,char ope2){
  vector<char> rem = infer_ope(ope1,ope2);
  ParseResult r = factor(pos,str,rem[0],rem[1]);
  while(r.pos < str.size()
	&& (str[r.pos] == ope1 || str[r.pos] == ope2)){
    ParseResult tmp = factor(r.pos+1,str,rem[0],rem[1]);
    if(str[r.pos] == ope1){
      Processor pr;
      pr.setStrategy(ope1);
      pr.calc(r.num,tmp.num);
    }
    if(str[r.pos] == ope2){
      Processor pr;
      pr.setStrategy(ope2);
      pr.calc(r.num,tmp.num);
    }
    r.pos = tmp.pos;
  }
  return r;
}

ParseResult expr(int pos,const string& str,char ope1,char ope2,char ope3){
  vector<char> rem = infer_ope(ope1,ope2,ope3);
  ParseResult r = factor(pos,str,rem[0]);
  while(r.pos < str.size()
	&& (str[r.pos] == ope1 || str[r.pos] == ope2 || str[r.pos] == ope3)){
    ParseResult tmp = factor(r.pos+1,str,rem[0]);
    if(str[r.pos] == ope1){
      Processor pr;
      pr.setStrategy(ope1);
      pr.calc(r.num,tmp.num);
    }
    if(str[r.pos] == ope2){
      Processor pr;
      pr.setStrategy(ope2);
      pr.calc(r.num,tmp.num);
    }
    if(str[r.pos] == ope3){
      Processor pr;
      pr.setStrategy(ope3);
      pr.calc(r.num,tmp.num);
    }
    r.pos = tmp.pos;
  }
  return r;
}

ParseResult factor(int pos, const string& str,char ope1){
  vector<char> rem = infer_ope(ope1);
  ParseResult r = term(pos,str,rem[0],rem[1],rem[2]);
  while(r.pos < str.size()
	&& (str[r.pos] == ope1)){
    ParseResult tmp = term(r.pos+1,str,rem[0],rem[1],rem[2]);
    if(str[r.pos] == ope1){
      Processor pr;
      pr.setStrategy(ope1);
      pr.calc(r.num,tmp.num);
    }
    r.pos = tmp.pos;
  }
  return r;
}

ParseResult factor(int pos, const string& str,char ope1,char ope2){
  vector<char> rem = infer_ope(ope1,ope2);
  ParseResult r = term(pos,str,rem[0],rem[1]);
  while(r.pos < str.size()
	&& (str[r.pos] == ope1 || str[r.pos] == ope2)){
    ParseResult tmp = term(r.pos+1,str,rem[0],rem[1]);
    if(str[r.pos] == ope1){
      Processor pr;
      pr.setStrategy(ope1);
      pr.calc(r.num,tmp.num);
    }
    if(str[r.pos] == ope2){
      Processor pr;
      pr.setStrategy(ope2);
      pr.calc(r.num,tmp.num);
    }
    r.pos = tmp.pos;
  }
  return r;
}

ParseResult factor(int pos, const string& str,char ope1,char ope2,char ope3){
  vector<char> rem = infer_ope(ope1,ope2,ope3);
  ParseResult r = term(pos,str,rem[0]);
  while(r.pos < str.size()
	&& (str[r.pos] == ope1 || str[r.pos] == ope2 || str[r.pos] == ope3)){
    ParseResult tmp = term(r.pos+1,str,rem[0]);
    if(str[r.pos] == ope1){
      Processor pr;
      pr.setStrategy(ope1);
      pr.calc(r.num,tmp.num);
    }
    if(str[r.pos] == ope2){
      Processor pr;
      pr.setStrategy(ope2);
      pr.calc(r.num,tmp.num);
    }
    if(str[r.pos] == ope3){
      Processor pr;
      pr.setStrategy(ope3);
      pr.calc(r.num,tmp.num);
    }
    r.pos = tmp.pos;
  }
  return r;
}

ParseResult term(int pos, const string& str,char ope1,char ope2,char ope3){
  if(str[pos] == '('){
    ParseResult r = expr(pos+1,str,ope1,ope2,ope3);
    r.pos++; // for ')'
    return r;
  }
  else {
    return num(pos,str);
  }
}

ParseResult term(int pos, const string& str,char ope1,char ope2){
  if(str[pos] == '('){
    ParseResult r = expr(pos+1,str,ope1,ope2);
    r.pos++; // for ')'
    return r;
  }
  else {
    return num(pos,str);
  }
}

ParseResult term(int pos, const string& str,char ope1){
  if(str[pos] == '('){
    ParseResult r = expr(pos+1,str,ope1);
    r.pos++; // for ')'
    return r;
  }
  else {
    return num(pos,str);
  }
}

ParseResult term(int pos, const string& str){
  if(str[pos] == '('){
    ParseResult r = expr(pos+1,str);
    r.pos++; // for ')'
    return r;
  }
  else {
    return num(pos,str);
  }
}

ParseResult num(int pos,const string& str){
  int sum = 0;
  while(pos < str.size() && isdigit(str[pos])){
    sum *= 10;
    sum += str[pos] - '0';
    pos++;
  }
  return ParseResult(pos,sum);
}

int main(){
  string str;
  char patterns[4] = {'-','+','*','/'};
  while(cin >> str){
    if(str == "#") break;

    for(int i = 0; i < 4; i++){
      for(int j = i+1; j < 4; j++){
	ParseResult r = expr(0,str,patterns[i],patterns[j]);
	cout << r.num << endl;
      }
    }
    for(int i = 0; i < 4; i++){
      ParseResult r = expr(0,str,patterns[i]);
      cout << r.num << endl;
    }
    for(int i = 0; i < 4; i++){
      vector<char> tmp;
      for(int j = 0; j < 4; j++){
	if(i != j) tmp.push_back(patterns[j]);
      }
      ParseResult r = expr(0,str,tmp[0],tmp[1],tmp[2]);
      cout << r.num << endl;
    }
    ParseResult r2 = expr(0,str);
    cout << r2.num << endl;
  }
}
