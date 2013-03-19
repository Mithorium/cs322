#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <ctime>
#include <exception>
#include <stdexcept>
#include <algorithm>

namespace Util {
  template<typename T>
  T pop_random(std::vector<T> &v) {
    unsigned int randomIndex = rand() % v.size();
    T item = v.at(randomIndex);
    v.erase(v.begin() + randomIndex);
    return item;
  }
}

#endif