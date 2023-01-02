#include "input.hpp"

namespace input {
std::vector<Input> extractInputQueue() {
  std::vector<Input> inputs;

  if (IsKeyPressed(KEY_W)) {
    inputs.push_back(Input('w', true));
  } else if (IsKeyReleased(KEY_W)) {
    inputs.push_back(Input('w', false));
  }

  if (IsKeyPressed(KEY_S)) {
    inputs.push_back(Input('s', true));
  } else if (IsKeyReleased(KEY_S)) {
    inputs.push_back(Input('s', false));
  }

  if (IsKeyPressed(KEY_A)) {
    inputs.push_back(Input('a', true));
  } else if (IsKeyReleased(KEY_A)) {
    inputs.push_back(Input('a', false));
  }

  if (IsKeyPressed(KEY_D)) {
    inputs.push_back(Input('a', true));
  } else if (IsKeyReleased(KEY_D)) {
    inputs.push_back(Input('a', false));
  }

  if (IsKeyPressed(KEY_J)) {
    inputs.push_back(Input('j', true));
  } else if (IsKeyReleased(KEY_J)) {
    inputs.push_back(Input('j', false));
  }

  if (IsKeyPressed(KEY_I)) {
    inputs.push_back(Input('i', true));
  } else if (IsKeyReleased(KEY_I)) {
    inputs.push_back(Input('i', false));
  }

  if (IsKeyPressed(KEY_K)) {
    inputs.push_back(Input('k', true));
  } else if (IsKeyReleased(KEY_K)) {
    inputs.push_back(Input('k', false));
  }

  if (IsKeyPressed(KEY_L)) {
    inputs.push_back(Input('l', true));
  } else if (IsKeyReleased(KEY_L)) {
    inputs.push_back(Input('l', false));
  }

  if (IsKeyPressed(KEY_SPACE)) {
    inputs.push_back(Input(' ', true));
  } else if (IsKeyReleased(KEY_SPACE)) {
    inputs.push_back(Input(' ', false));
  }

  return inputs;
}
} // namespace input
