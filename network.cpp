#include "network.hpp"

Session::Session(boost::asio::io_context &io_context) : socket(io_context) {}
Session::~Session() {}

void Session::handle_read(const boost::system::error_code &error,
                          size_t bytes_transferred) {
  // do sync read to get serialized data
  boost::asio::streambuf data_buffer;
  boost::asio::read(socket, data_buffer.prepare(header));
  data_buffer.commit(header);
  std::istream stream(&data_buffer);
  boost::archive::binary_iarchive serialized_data(stream);

  handle_deserialization(serialized_data);

  // start async read again
  socket.async_read_some(
      boost::asio::buffer(&header, sizeof(uint32_t)),
      boost::bind(&Session::handle_read, this, boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void Session::handle_write() {}

ServerSession::ServerSession(boost::asio::io_context &io_context,
                             std::string host)
    : Session(io_context) {

  // connect to server
  boost::asio::ip::tcp::resolver resolver(io_context);
  boost::asio::ip::tcp::resolver::results_type endpoints =
      resolver.resolve(host, "8069");
  boost::asio::connect(socket, endpoints);
  std::cout << "Connected to Server" << std::endl;

  // wait for a header packet to come in
  /*
  socket.async_read_some(
      boost::asio::buffer(&header, sizeof(uint32_t)),
      boost::bind(&ServerSession::handle_read, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
  */
}

ServerSession::~ServerSession() {}

void ServerSession::handle_deserialization(
    boost::archive::binary_iarchive &data) {
  // TODO: deserialize drawable
}

ClientSession::ClientSession(boost::asio::io_context &io_context,
                             std::shared_ptr<input::InputQueue> inputRef)
    : Session(io_context), inputQueue(inputRef) {}

ClientSession::~ClientSession() {}

void ClientSession::startListening() {
  socket.async_read_some(
      boost::asio::buffer(&header, sizeof(uint32_t)),
      boost::bind(&Session::handle_read, this, boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void ClientSession::handle_deserialization(
    boost::archive::binary_iarchive &data) {
  // deserialize input and add it to the queue
  input::Input msg;
  data &msg;
  inputQueue->push(msg);
}

GameServer::GameServer(boost::asio::io_context &io_context,
                       std::shared_ptr<input::InputQueue> inputRef)
    : acceptor(io_context, boost::asio::ip::tcp::endpoint(
                               boost::asio::ip::tcp::v4(), 8069)),
      inputQueue(inputRef), tickInterval(30),
      tickTimer(new boost::asio::deadline_timer(io_context, tickInterval)) {

  this->io_context = &io_context;
  std::shared_ptr<ClientSession> session(
      new ClientSession(io_context, inputQueue));
  acceptor.async_accept(session->socket,
                        boost::bind(&GameServer::handle_connection_accept, this,
                                    session, boost::asio::placeholders::error));
}

GameServer::~GameServer() {}

void GameServer::handle_connection_accept(
    std::shared_ptr<ClientSession> session,
    const boost::system::error_code &error) {

  // start and save session
  session->startListening();
  clients.push_back(session);

  // if we don't have four players yet, keep trying to connect to clients
  if (clients.size() < 2) {
    std::shared_ptr<ClientSession> session(
        new ClientSession(*io_context, inputQueue));
    acceptor.async_accept(session->socket,
                          boost::bind(&GameServer::handle_connection_accept,
                                      this, session,
                                      boost::asio::placeholders::error));
  } else {
    // once everyone connects start the update loop
    tickTimer->async_wait(
        boost::bind(&GameServer::tick, this, boost::asio::placeholders::error));
  }
}

// do game update logic and send to clients
void GameServer::tick(const boost::system::error_code &e) {
  //consume an input if there is one
  if (!inputQueue->empty()) {
    input::Input i;
    inputQueue->pop(i);

    // update the game based on input
    std::cout << "key: " << i.getKey() << ", isDown: " << i.getDown()
              << std::endl;
  }

  // start timer again
  tickTimer->async_wait(
      boost::bind(&GameServer::tick, this, boost::asio::placeholders::error));
}
