#ifndef BVALUE_H
#define BVALUE_H

#include <iostream>
#include <variant>
#include <vector>
#include <map>


struct BValue{
  using ValueType = std::variant<int64_t, std::string, std::map<std::string, BValue>, std::vector<BValue>>;
  ValueType value;
};
#endif
