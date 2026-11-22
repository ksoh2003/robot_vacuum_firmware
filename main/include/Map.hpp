#pragma once
#include <vector>
#include <cstdint>

struct AreaObj {
  int x {};
  int y {};
  std::int8_t areaState {};
  bool clearState {};
};

class Map {
  public:
    Map();
    std::vector<AreaObj> map;

  private:
    void MapResizePlus();
};