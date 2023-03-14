#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include <tuple>
#include <QDebug>
#include <QPoint>

using Pattern = std::vector<QPoint>;

struct SizedPattern {
  Pattern pat;
  int w;
  int h;
};

inline Pattern shift(Pattern& p) {
  int minX = std::numeric_limits<int>::max();
  int minY = std::numeric_limits<int>::max();
  for(QPoint& pt : p) {
    minX = std::min(pt.x(), minX);
    minY = std::min(pt.y(), minY);
  }
  Pattern res(p);
  for(QPoint& pt: res) {
    pt -= QPoint(minX, minY);
  }
  return res;
}

inline std::vector<Pattern> rotations(Pattern& p) {
  std::vector<Pattern> res(4);
  res[0] = p;
  for (int i = 1; i < 4; ++i) {
    Pattern rotated(p.size());
    for (auto j = 0u; j < p.size(); ++j) {
      rotated[j].setX(res[i-1][j].y());
      rotated[j].setY(-res[i-1][j].x());
    }
    res[i] = shift(rotated);
  }
  return res;
}

inline Pattern mirrored(Pattern& p) {
  Pattern m(p);
  for (auto i = 0u; i < p.size(); ++i) {
    m[i].setY(-p[i].y());
  }
  return shift(m);
}

inline SizedPattern sized(Pattern& p) {
  SizedPattern res;
  res.pat = p;
  int maxX = std::numeric_limits<int>::min();
  int maxY = std::numeric_limits<int>::min();
  for(QPoint& pt : p) {
    maxX = std::max(pt.x(), maxX);
    maxY = std::max(pt.y(), maxY);
  }
  res.w = maxX + 1;
  res.h = maxY + 1;
  return res;
}

inline std::vector<SizedPattern> generate(Pattern p, bool symmetric=false) {
  auto s = rotations(p);
  std::vector<Pattern> res1;
  if (!symmetric) {
    Pattern m = mirrored(p);
    auto r = rotations(m);
    res1.reserve(s.size() + r.size());
    std::copy(r.begin(), r.end(), std::back_inserter(res1));
  }else{
    res1.reserve(s.size());
  }
  std::copy(s.begin(), s.end(), std::back_inserter(res1));
  std::vector<SizedPattern> res2;
  res2.reserve(res1.size());
  std::transform(res1.begin(), res1.end(), std::back_inserter(res2), [](Pattern& pt){
    return sized(pt);
  });
  return res2;
}

const Pattern four_p = {{0, 0}, {1, 1}, {0, 2}, {0, 3}};
const Pattern five_p = {{0, 0}, {0, 1}, {1, 2}, {0, 3}, {0, 4}};
const Pattern seven_p = {{0, 0}, {1, 1}, {1, 2}, {1, 3}, {2, 0}, {3, 0}, {4, 0}};

const std::vector<SizedPattern> fours = generate(four_p);
const std::vector<SizedPattern> fives = generate(five_p);
const std::vector<SizedPattern> sevens = generate(seven_p);

class Board {
  std::vector<int> board;
  std::default_random_engine e1{static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count())};
  std::uniform_int_distribution<int> uniform_dist{1, 5};
  std::uniform_int_distribution<int> uniform_dist_2;
  std::uniform_int_distribution<int> uniform_dist_3;
  std::uniform_int_distribution<int> coin{1, 42};
  int w;
  int h;
  std::set<std::pair<int, int>> matched_patterns;
  std::set<std::pair<int, int>> magic_tiles;
public:
  int width() { return w; }
  int height() { return h; }
  int trios_removed = 0;
  Board(int _w, int _h) : w{_w}, h{_h} {
    board.resize(w * h);
    std::fill(std::begin(board), std::end(board), 0);
    uniform_dist_2 = std::uniform_int_distribution<int>(0, w);
    uniform_dist_3 = std::uniform_int_distribution<int>(0, h);
  }
  Board(const Board& b){
    w = b.w;
    h = b.h;
    board = b.board;
    trios_removed = b.trios_removed;
    matched_patterns = b.matched_patterns;
  }
  Board operator=(const Board& b){
    w = b.w;
    h = b.h;
    board = b.board;
    trios_removed = b.trios_removed;
    matched_patterns = b.matched_patterns;
    return *this;
  }
  bool operator==(const Board& a){
    for(int i = 0; i < w * h; ++i){
      if (board[i] != a.board[i]){
        return false;
      }
    }
    return true;
  }
  bool match_pattern(int x, int y, const SizedPattern& p) {
    int color = at(x + p.pat[0].x(), y + p.pat[0].y());
    for(auto i = 1u; i < p.pat.size(); ++i) {
      if(color != at(x + p.pat[i].x(), y + p.pat[i].y())) {
        return false;
      }
    }
    return true;
  }
  void match_patterns() {
    matched_patterns.clear();
    for(const SizedPattern& sp : fours){
      for(int i = 0; i <= w - sp.w; ++i){
        for(int j = 0; j <= h - sp.h; ++j){
          if(match_pattern(i, j, sp)){
            for(const QPoint& p: sp.pat){
              matched_patterns.insert({i + p.x(), j + p.y()});
            }
          }
        }
      }
    }
    for(const SizedPattern& sp : fives){
      for(int i = 0; i <= w - sp.w; ++i){
        for(int j = 0; j <= h - sp.h; ++j){
          if(match_pattern(i, j, sp)){
            for(const QPoint& p: sp.pat){
              matched_patterns.insert({i + p.x(), j + p.y()});
            }
          }
        }
      }
    }
    for(const SizedPattern& sp : sevens){
      for(int i = 0; i <= w - sp.w; ++i){
        for(int j = 0; j <= h - sp.h; ++j){
          if(match_pattern(i, j, sp)){
            for(const QPoint& p: sp.pat){
              matched_patterns.insert({i + p.x(), j + p.y()});
            }
          }
        }
      }
    }
  }
  bool is_matched(int x, int y) {
    return matched_patterns.count({x, y}) > 0;
  }
  bool is_magic(int x, int y) {
    return magic_tiles.count({x, y}) > 0;
  }
  void fill(){
    trios_removed = 0;
    for(auto& x : board){
      x = uniform_dist(e1);
    }
  }
  int& at(int a, int b){
    return board[a * h + b];
  }
  bool reasonable_coord(int i, int j){
    return i >= 0 && i < w && j >= 0 && j < h;
  }
  void remove_trios() {
    std::vector<std::tuple<int,int,int>> remove_i;
    std::vector<std::tuple<int,int,int>> remove_j;
    for(int i = 0; i < w; ++i){
      for(int j = 0; j < h; ++j){
        int offset_j = 1;
        int offset_i = 1;
        while(j + offset_j < h && at(i,j) == at(i, j+offset_j)){
          offset_j += 1;
        }
        if(offset_j > 2){
          remove_i.push_back({i, j, offset_j});
        }
        while(i + offset_i < w && at(i,j) == at(i+offset_i, j)){
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
        offset = h;
      }
      for(int jj = j; jj < j + offset; ++jj){
        at(i, jj) = 0;
        if (is_magic(i, jj)){
            trios_removed -= 3;
            magic_tiles.erase({i, jj});
        }
        trios_removed += 1;
      }
      if(offset == 5){
        for(int i = 0; i < w; ++i){
          at(uniform_dist_2(e1), uniform_dist_3(e1)) = 0;
        }
      }
    }
    for(auto t : remove_j){
      int i = std::get<0>(t);
      int j = std::get<1>(t);
      int offset = std::get<2>(t);
      if(offset == 4){
        i = 0;
        offset = w;
      }
      for(int ii = i; ii < i + offset; ++ii){
        at(ii, j) = 0;
        if (is_magic(ii, j)){
            trios_removed -= 3;
            magic_tiles.erase({ii, j});
        }
        trios_removed += 1;
      }
      if(offset == 5){
        for(int i = 0; i < w; ++i){
          at(uniform_dist_2(e1), uniform_dist_3(e1)) = 0;
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
    for(int i = 0; i < w; ++i){
      for(int j = 0; j < h; ++j){
        if(at(i,j) == 0){
          curr_i = i;
          while(curr_i < w - 1 && at(curr_i + 1, j) == 0){
            curr_i += 1;
          }
          for(int k = curr_i; k >= 0; --k){
            if(at(k, j) != 0){
              at(curr_i,j) = at(k, j);
              if (is_magic(k, j)) {
                  magic_tiles.erase({k, j});
                  magic_tiles.insert({curr_i, j});
              }
              curr_i -= 1;
            }
          }
          for(int k = curr_i; k >= 0; --k){
            at(k, j) = uniform_dist(e1);
            if (coin(e1) == 1) {
              magic_tiles.insert({k, j});
            }
          }
        }
      }
    }
  }
};

#endif // TYPES_HPP
