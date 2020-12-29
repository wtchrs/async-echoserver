#include "server.hpp"

#include <algorithm>
#include <asio.hpp>
#include <functional>
#include <iostream>
#include <string>

int main() {
    asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();

    std::cout << "End\n";

    return 0;
}

// class Session {{{

void Session::post_receive() {
    memset(&receive_buffer_, '\0', sizeof(receive_buffer_));
    socket_.async_read_some(
        asio::buffer(receive_buffer_),
        std::bind(&Session::handle_receive, this, std::placeholders::_1,
                  std::placeholders::_2));
}

void Session::handle_receive(const asio::error_code& error,
                             size_t bytes_transferred) {
    if (error) {
        if (error == asio::error::eof) {
            std::cout << "Disconnected from client\n";
        } else {
            std::cout << "Error " << error.value() << " : " << error.message()
                      << '\n';
        }
    } else {
        const std::string recv_msg = receive_buffer_.data();
        std::cout << "Get : " << recv_msg << ", size : " << bytes_transferred
                  << '\n';

        char msg[128] = {
            0,
        };
        sprintf(msg, "Re:%s", recv_msg.c_str());
        write_message_ = msg;

        asio::async_write(
            socket_, asio::buffer(write_message_),
            std::bind(&Session::handle_write, this, std::placeholders::_1,
                      std::placeholders::_2));

        post_receive();
    }
}

// }}} End class Session

// class tcp_server {{{

tcp_server::tcp_server(asio::io_context& io_context)
    : io_context_(io_context),
      acceptor_(io_context,
                asio::ip::tcp::endpoint(asio::ip::tcp::v4(), PORT_NUMBER)) {
    session_ = nullptr;
    start_accept();
}

tcp_server::~tcp_server() {
    if (session_ != nullptr) {
        delete session_;
    }
}

void tcp_server::start_accept() {
    std::cout << "Waiting for client connect\n";
    session_ = new Session(io_context_);
    acceptor_.async_accept(session_->socket(),
                           std::bind(&tcp_server::handle_accept, this, session_,
                                     std::placeholders::_1));
}

void tcp_server::handle_accept(Session* session,
                               const asio::error_code& error) {
    if (!error) {
        std::cout << "Connected to client\n";
        session_->post_receive();
    }
}

// }}} End class tcp_server
