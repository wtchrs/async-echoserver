#include <algorithm>
#include <asio.hpp>
#include <functional>
#include <iostream>
#include <string>

const char PROGRAM_NAME[] = "[async-echoserver] ";

constexpr unsigned short PORT_NUMBER = 31400;

class Session
{
private:
    asio::ip::tcp::socket socket_;
    std::string write_message_;
    std::array<char, 128> receive_buffer_;

public:
    Session(asio::io_context& io_context) : socket_(io_context) {}

    asio::ip::tcp::socket& socket() {
        return socket_;
    }

    void post_receive();

private:
    void handle_write(const asio::error_code&, size_t) {}
    void handle_receive(const asio::error_code&, size_t);
};

class tcp_server
{
private:
    int seq_number_;
    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
    Session* session_;

public:
    tcp_server(asio::io_context&);
    ~tcp_server();

private:
    void start_accept();
    void handle_accept(Session*, const asio::error_code&);
};
