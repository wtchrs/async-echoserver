#include "client.hpp"

#include <asio.hpp>
#include <functional>
#include <iostream>

int main() {
    asio::io_context io_context;
    asio::ip::tcp::endpoint endpoint{asio::ip::address::from_string(SERVER_IP),
                                     PORT_NUMBER};
    tcp_client client{io_context};

    client.connect(endpoint);
    io_context.run();

    std::cout << "Finished network connect\n";

    return 0;
}

// class tcp_client {{{

void tcp_client::connect(asio::ip::tcp::endpoint& endpoint) {
    socket_.async_connect(endpoint, std::bind(&tcp_client::handle_connect, this,
                                              std::placeholders::_1));
}

void tcp_client::post_write() {
    if (!socket_.is_open()) {
        return;
    }
    if (seq_number_ > 7) {
        socket_.close();
        return;
    }
    ++seq_number_;

    char message[128] = {
        0,
    };
    sprintf(message, "%d - Send Message", seq_number_);

    write_message_ = message;

    asio::async_write(socket_, asio::buffer(write_message_),
                      std::bind(&tcp_client::handle_write, this,
                                std::placeholders::_1, std::placeholders::_2));

    post_receive();
}

void tcp_client::post_receive() {
    memset(&receive_buffer_, '\0', sizeof(receive_buffer_));

    socket_.async_read_some(
        asio::buffer(receive_buffer_),
        std::bind(&tcp_client::handle_receive, this, std::placeholders::_1,
                  std::placeholders::_2));
}

void tcp_client::handle_connect(const asio::error_code& error) {
    if (error) {
        std::cout << "Connect failed : " << error.message() << '\n';
    } else {
        std::cout << "Connected\n";
        post_write();
    }
}

void tcp_client::handle_receive(const asio::error_code& error,
                                size_t bytes_transferred) {
    if (error) {
        if (error == asio::error::eof) {
            std::cout << "Disconnected from server\n";
        } else {
            std::cout << "Error " << error.value() << " : " << error.message()
                      << '\n';
        }
    } else {
        const std::string recv_msg = receive_buffer_.data();
        std::cout << "Received from server : " << recv_msg
                  << ", received size : " << bytes_transferred << '\n';
        post_write();
    }
}

// }}} End class tcp_client
