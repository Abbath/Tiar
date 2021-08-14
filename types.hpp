#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>
#include <algorithm>
#include <random>
#include <tuple>
#include <QDebug>

class Board {
  std::vector<int> board;
  std::random_device r;
  std::default_random_engine e1{r()};
  std::uniform_int_distribution<int> uniform_dist{1, 5};
  std::uniform_int_distribution<int> uniform_dist_2{0, 9};
public:
  int trios_removed = 0;
  Board() {
    board.resize(100);
    std::fill(std::begin(board), std::end(board), 0);
  }
  Board(const Board& b){
    board = b.board;
    trios_removed = b.trios_removed;
  }
  Board operator=(const Board& b){
    board = b.board;
    trios_removed = b.trios_removed;
    return *this;
  }
  bool operator==(const Board& a){
    for(int i = 0; i < 100; ++i){
      if (board[i] != a.board[i]){
        return false;
      }
    }
    return true;
  }
  void fill(){
    trios_removed = 0;
    for(auto& x : board){
      x = uniform_dist(e1);
    }
  }
  int& at(int a, int b){
    return board[a * 10 + b];
  }
  bool reasonable_coord(int i, int j){
    return i >= 0 && i < 10 && j >= 0 && j < 10;
  }
  void remove_trios() {
    std::vector<std::tuple<int,int,int>> remove_i;
    std::vector<std::tuple<int,int,int>> remove_j;
    for(int i = 0; i < 10; ++i){
      for(int j = 0; j < 10; ++j){
        int offset_j = 1;
        int offset_i = 1;
        while(j + offset_j < 10 && at(i,j) == at(i, j+offset_j)){
          offset_j += 1;
        }
        if(offset_j > 2){
          remove_i.push_back({i, j, offset_j});
        }
        while(i + offset_i < 10 && at(i,j) == at(i+offset_i, j)){
          offset_i += 1;
        }
        if(offset_i > 2){
          remove_j.push_back({i, j, offset_i});
        }
      }
    }
    for(auto t : remove_i){
      int i = std::get<0>(t);
      int j = std::get<1>(t);
      int offset = std::get<2>(t);
      if(offset == 4){
        j = 0;
        offset = 10;
      }
      for(int jj = j; jj < j + offset; ++jj){
        at(i, jj) = 0;
        trios_removed += 1;
      }
      if(offset == 5){
        for(int i = 0; i < 10; ++i){
          at(uniform_dist_2(e1), uniform_dist_2(e1)) = 0;
        }
      }
    }
    for(auto t : remove_j){
      int i = std::get<0>(t);
      int j = std::get<1>(t);
      int offset = std::get<2>(t);
      if(offset == 4){
        i = 0;
        offset = 10;
      }
      for(int ii = i; ii < i + offset; ++ii){
        at(ii, j) = 0;
        trios_removed += 1;
      }
      if(offset == 5){
        for(int i = 0; i < 10; ++i){
          at(uniform_dist_2(e1), uniform_dist_2(e1)) = 0;
          trios_removed += 5;
        }
      }
    }
    for(int i = 0; i < int(remove_i.size()); ++i){
      for(int j = 0; j < int(remove_j.size()); ++j){
        auto t1 = remove_i[i];
        auto t2 = remove_j[j];
        auto i1 = std::get<0>(t1);
        auto j1 = std::get<1>(t1);
        auto o1 = std::get<2>(t1);
        auto i2 = std::get<0>(t2);
        auto j2 = std::get<1>(t2);
        auto o2 = std::get<2>(t2);
        if(i1 >= i2 && i1 < (i2 + o2) && j2 >= j1 && j2 < (j1 + o1)){
          for(int m = -1; m < 2; ++m){
            for(int n = -1; n < 2; ++n){
              if(reasonable_coord(i1+m, j1+n)){
                at(i1+m, j1+n) = 0;
                trios_removed += 1;
              }
            }
          }
        }
      }
    }
  }
  void fill_up(){
    int curr_i = -1;
    for(int i = 0; i < 10; ++i){
      for(int j = 0; j < 10; ++j){
        if(at(i,j) == 0){
          curr_i = i;
          while(curr_i < 9 && at(curr_i + 1, j) == 0){
            curr_i += 1;
          }
          for(int k = curr_i; k >= 0; --k){
            if(at(k, j) != 0){
              at(curr_i,j) = at(k, j);
              curr_i -= 1;
            }
          }
          for(int k = curr_i; k >= 0; --k){
            at(k, j) = uniform_dist(e1);
          }
        }
      }
    }
  }
};

#endif // TYPES_HPP
