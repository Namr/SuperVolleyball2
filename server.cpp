#include "network.hpp"

int main() {
  boost::asio::io_context io_context;
  std::shared_ptr<input::InputQueue> inputQueue =
      std::shared_ptr<input::InputQueue>(new input::InputQueue(256));
  GameServer server(io_context, inputQueue);

  while (true) {
    io_context.run();
  }
}
