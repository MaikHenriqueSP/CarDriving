#ifndef ACTIONDATA_HPP_
#define ACTIONDATA_HPP_

#include <bitset>

enum class Action { Right, Left, Forward, Backward };

struct ActionData {
  std::bitset<5> m_input;  
};

#endif