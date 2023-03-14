#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <mutex>

#include "input.hpp"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

/*
 * Wrapper around a socket that can handle serializable data
 */
class Session {
public:
  Session(boost::asio::io_context &io_context);
  ~Session();
  void handle_read(const boost::system::error_code &error,
                   size_t bytes_transferred);
  void handle_write();

  template <class T> void sendSerializable(T message) {
    // serialize message and put it into a stream
    boost::asio::streambuf data_buffer;
    std::ostream stream(&data_buffer);
    boost::archive::binary_oarchive serialized_data(stream);

    // this is using an overloaded binary AND to serialize
    // the message into an archive, it looks weird
    serialized_data &message;

    // write header
    header = data_buffer.size();
    boost::asio::write(socket, boost::asio::buffer(&header, sizeof(header)));

    // write stream
    boost::asio::write(socket, data_buffer.data());
  }

  boost::asio::ip::tcp::socket socket;

protected:
  virtual void
  handle_deserialization(boost::archive::binary_iarchive &data) = 0;
  uint32_t header = 0;
};

/*
 * handles a connection from a client to a server (used by client)
 */
class ServerSession : public Session {
public:
  ServerSession(boost::asio::io_context &io_context, std::string host);
  ~ServerSession();

protected:
  void handle_deserialization(boost::archive::binary_iarchive &data);
  //TODO: need a pointer to the game scene
};

/*
 * handles a connection from a server to a client (used by server)
 */
class ClientSession : public Session {
public:
  ClientSession(boost::asio::io_context &io_context,
                std::shared_ptr<input::InputQueue> inputRef);
  ~ClientSession();
  void startListening();

protected:
  void handle_deserialization(boost::archive::binary_iarchive &data);
  std::shared_ptr<input::InputQueue> inputQueue;
};

class GameServer {
public:
  GameServer(boost::asio::io_context &io_context,
             std::shared_ptr<input::InputQueue> inputRef);
  ~GameServer();

private:
  void handle_connection_accept(std::shared_ptr<ClientSession> session,
                                const boost::system::error_code &error);
  void tick(const boost::system::error_code &e);

  boost::asio::io_context *io_context;
  boost::asio::ip::tcp::acceptor acceptor;
  boost::posix_time::milliseconds tickInterval;

  std::vector<std::shared_ptr<ClientSession>> clients;
  std::shared_ptr<input::InputQueue> inputQueue;
  std::unique_ptr<boost::asio::deadline_timer> tickTimer;
  //TODO: need a pointer to the game scene
};
