#include <functional>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::endl;

vector<int> get_next(const string& pattern) {
  vector<int> next(pattern.size());
  next[0] = -1;
  next[1] = 0;
  int current_inspect = 2;
  int dummy_next = 0;
  while (current_inspect < pattern.size()) {
   if (pattern[dummy_next] == pattern[current_inspect - 1]||dummy_next == -1){
     next[current_inspect++] = ++dummy_next;
   }
   else{
     dummy_next = next[dummy_next];
   }
  }
  cout << "next generated" << endl;
  return next;
}

void kmp_completion(const string& mother, const string& son){
  auto next = get_next(son);
  int son_index = 0;
  int mother_index = 0;
  while (mother_index < mother.size()-son.size()){
    if(mother[mother_index] == son[son_index]){
      son_index += 1;
      mother_index += 1;
      if (son_index == son.size())
        {
          cout << "complete !" << endl;
          return;
        }
    }
    else {
      if (son_index >= 0) 
        son_index = next[son_index];
      else{
        son_index = 0;
        mother_index ++ ;
      }
    }
  }
  cout << "not completed !" << endl;
  return;
}

int main(){
  string mother;
  string son;
  std::getline(std::cin, mother);
  std::getline(std::cin, son);
  kmp_completion(mother, son);

  return 0;  
}
