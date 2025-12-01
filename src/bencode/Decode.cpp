#include "bencode/Decode.h"

namespace bencode{
  Decode::Decode(std::vector<std::byte>& byte_array)
    : m_byte_array{byte_array}
    {
    }

  Value Decode::parseByteArray(){
    try{
    std::pair<size_t, Value> pair {parseBValue(0)};
    std::cout << "Byte array was successfully decoded." << '\n';
    return pair.second;
    }catch(const std::invalid_argument& e){
      std::cerr << "Byte array is invalid" << e.what();
      return Value{};
    }
  }
  std::pair<size_t, Value> Decode::parseBString(size_t curr_pos){
    if(!isString(m_byte_array[curr_pos])){
      throw std::invalid_argument("Invalid BString: first byte has to contain a digit");
    }
    size_t start_pos {curr_pos};
    std::string digit_chars_string{};
    size_t string_length{};
    std::byte column_byte {58};
    while( curr_pos != std::size(m_byte_array) && m_byte_array[curr_pos] != column_byte){
      digit_chars_string += static_cast<char>(m_byte_array[curr_pos]);
      ++curr_pos;
    }
    ++curr_pos; //skip :
    std::istringstream(digit_chars_string) >> string_length;
    std::string chars_string{};
    for(size_t i = 0; i < string_length && curr_pos != std::size(m_byte_array); ++i){
      chars_string += static_cast<char>(m_byte_array[curr_pos]);
      ++curr_pos;
    }
    Value bstring{chars_string};
    return std::pair<size_t, Value>{curr_pos, bstring};
  }

  std::pair<size_t, Value> Decode::parseBInt(size_t curr_pos){
    ++curr_pos;
    bool isZero{false};
    bool isNegative{false};

    std::byte zero{48};
    std::byte minus{45};

    m_byte_array[curr_pos] == zero ? isZero=true : isZero=false;
    m_byte_array[curr_pos] == minus ? isNegative=true : isNegative=false;

    if(isNegative ||  m_byte_array[curr_pos + static_cast<size_t>(1)] == zero){
      throw std::invalid_argument("Invalid BInteger: Zero can not be negative.");
    } 
    
    std::string digit_chars_string {};
    int64_t  result_int{};
    while(curr_pos != std::size(m_byte_array) && m_byte_array[curr_pos] != m_end_delimeter){
      char current_char {static_cast<char>(m_byte_array[curr_pos])};
      if(!std::isdigit(current_char)){
        throw std::invalid_argument("Invalid BInteger: BInteger can only contain chars which can be evaluated in digits.");
      }
      digit_chars_string += current_char;
      ++curr_pos; 
    }
    ++curr_pos; //skip end delimeter "e"
    std::istringstream(digit_chars_string) >> result_int;
    Value bint{result_int};
    return std::pair<size_t, Value>{curr_pos, bint};
  }

  std::pair<size_t, Value> Decode::parseBList(size_t curr_pos){
    std::vector<Value> bvalues_vector{};
    ++curr_pos; //skip start delimeter "l"
    while(curr_pos != std::size(m_byte_array) && m_byte_array[curr_pos] != m_end_delimeter){
      std::pair<size_t, Value> size_bvalue_pair{parseBValue(curr_pos)};
      curr_pos = size_bvalue_pair.first;
      bvalues_vector.push_back(size_bvalue_pair.second);
    }
    ++curr_pos; //skip end delimeter "e"
    Value bvector{bvalues_vector};
    return std::pair<size_t, Value>{curr_pos, bvector};
  }

  std::pair<size_t, Value> Decode::parseBDict(size_t curr_pos){
    std::map<std::string, Value> string_bvalue_map{};
    std::string key{};
    Value value{};
    ++curr_pos; //skip start delimeter "d"
    while(curr_pos != std::size(m_byte_array) &&  m_byte_array[curr_pos] != m_end_delimeter){
      try{
        std::pair<size_t, Value> pos_bstring_pair{parseBString(curr_pos)};
        curr_pos = pos_bstring_pair.first;
        Value bstring = pos_bstring_pair.second;
        std::string converted_bstring{std::get<std::string>(bstring.value)};
        key = converted_bstring;
      }catch(const  std::invalid_argument&){
        std::cerr<<"Invalid BDict: Key has to be a string.";
      }
      try{
        std::pair<size_t, Value> pos_bvalue_pair{parseBValue(curr_pos)};
        curr_pos = pos_bvalue_pair.first;
        Value bvalue{pos_bvalue_pair.second};
        value = bvalue;
      }catch(const std::invalid_argument& exception){
        std::cerr<<"Invalid BDict: " << exception.what();
      }
      string_bvalue_map.insert({key, value});
    }
    ++curr_pos;
    Value bmap{string_bvalue_map};
    return std::pair<size_t, Value>{curr_pos, bmap};
  }


  bool Decode::isString(std::byte byte){
    if(std::isdigit(static_cast<char>(byte))){
      return true;
    }else{
      return false;
    }

  }

  std::pair<size_t, Value> Decode::parseBValue(size_t curr_pos){
      if(m_byte_array[curr_pos] == m_integer_start){
        return parseBInt(curr_pos); 
      }else if(m_byte_array[curr_pos] == m_list_start){
        return parseBList(curr_pos);
      }else if(m_byte_array[curr_pos] == m_dict_start){
        return parseBDict(curr_pos);
      }else{
        return parseBString(curr_pos);
      }
  }
}
