#ifndef BVALUE_H
#define BVALUE_H

#include <iostream>
#include <variant>
#include <vector>
#include <map>

namespace bencode{
  struct Value{
    using ValueType = std::variant<int64_t, std::string, std::map<std::string, Value>, std::vector<Value>>;
    ValueType value;  
  };
}
#endif
