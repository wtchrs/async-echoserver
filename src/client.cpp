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
        : io_context_(io_context), socket_(io_context), seq_number_(0) {
    }

    void connect(asio::ip::tcp::endpoint& endpoint) {
        socket_.async_connect(endpoint, std::bind(&tcp_client::handle_connect,
                                                  this, std::placeholders::_1));
    }

private:
    void post_write() {
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

        asio::async_write(
            socket_, asio::buffer(write_message_),
            std::bind(&tcp_client::handle_write, this, std::placeholders::_1,
                      std::placeholders::_2));

        post_receive();
    }

    void post_receive() {
        memset(&receive_buffer_, '\0', sizeof(receive_buffer_));

        socket_.async_read_some(
            asio::buffer(receive_buffer_),
            std::bind(&tcp_client::handle_receive, this, std::placeholders::_1,
                      std::placeholders::_2));
    }

    void handle_connect(const asio::error_code& error) {
        if (error) {
            std::cout << "Connect failed : " << error.message() << '\n';
        } else {
            std::cout << "Connected\n";
            post_write();
        }
    }

    void handle_write(const asio::error_code&, size_t) {
    }

    void handle_receive(const asio::error_code& error,
                        size_t bytes_transferred) {
        if (error) {
            if (error == asio::error::eof) {
                std::cout << "Disconnected from server\n";
            } else {
                std::cout << "Error " << error.value() << " : "
                          << error.message() << '\n';
            }
        } else {
            const std::string recv_msg = receive_buffer_.data();
            std::cout << "Received from server : " << recv_msg
                      << ", received size : " << bytes_transferred << '\n';
            post_write();
        }
    }
};

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
