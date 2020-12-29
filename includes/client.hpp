#include <asio.hpp>
#include <functional>
#include <iostream>

constexpr char SERVER_IP[] = "127.0.0.1";
constexpr unsigned short PORT_NUMBER = 31400;
constexpr char PROGRAM_NAME[] = "[async-echoserver-client] ";

class tcp_client
{
private:
    asio::io_context& io_context_;
    asio::ip::tcp::socket socket_;
    int seq_number_;
    std::array<char, 128> receive_buffer_;
    std::string write_message_;

public:
    tcp_client(asio::io_context& io_context)
        : io_context_(io_context), socket_(io_context), seq_number_(0) {}
    void connect(asio::ip::tcp::endpoint& endpoint);

private:
    void post_write();
    void post_receive();
    void handle_connect(const asio::error_code&);
    void handle_write(const asio::error_code&, size_t) {}
    void handle_receive(const asio::error_code&, size_t);
};
