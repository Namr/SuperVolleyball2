#pragma once
#include "raylib.h"
#include <vector>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/lockfree/queue.hpp>

namespace input {
class Input {
public:
  Input(char targetKey, bool isPressedDown)
      : key{targetKey}, isDown{isPressedDown} {}
  Input() : key{' '}, isDown{false} {}
  char getKey() { return key; };
  bool getDown() { return isDown; };

private:
  char key;
  bool isDown;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &key;
    ar &isDown;
  }
};

std::vector<Input> extractInputQueue();

typedef boost::lockfree::queue<Input> InputQueue;
} // namespace input
