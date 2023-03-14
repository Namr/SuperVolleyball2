#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/asio.hpp>

#include "input.hpp"
#include "network.hpp"
#include "raylib.h"

int main() {
  boost::asio::io_context io_context;
  std::string host("127.0.0.1");

  //TODO: make scene object on heap and pass to session
  ServerSession session(io_context, host);

  std::thread io_thread([&] {
    while (true) {
      io_context.run();
    }
  });
  
  // window loop
  InitWindow(800, 600, "Super Volleyball 2");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    std::vector<input::Input> inputs = input::extractInputQueue();
    std::for_each(inputs.begin(), inputs.end(), [&](input::Input &i) {
      session.sendSerializable(i);
    });

    BeginDrawing();

    // TODO: draw scene

    ClearBackground(BEIGE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
