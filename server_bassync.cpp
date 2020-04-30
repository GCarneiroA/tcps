
// https://www.boost.org/doc/libs/1_70_0/doc/html/boost_asio/tutorial/

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;
using boost::asio::ip::tcp;

class con_handler : public boost::enable_shared_from_this<con_handler>
{
private:
    tcp::socket sock;
    std::string message = "Hello from server!";
    enum { max_length = 1024 };
    char data[max_length];
public:
    typedef boost::shared_ptr<con_handler> pointer;
    con_handler(boost::asio::io_service &io_service): sock(io_service) {}

    static pointer create(boost::asio::io_service &io_service)
    {
        return pointer(new con_handler(io_service));
    }

    tcp::socket &socket()
    {
        return sock;
    }

    void start()
    {
        sock.async_read_some(
            boost::asio::buffer(data, max_length),
            boost::bind(&con_handler::handle_read,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        
        sock.async_write_some(
            boost::asio::buffer(message, max_length),
            boost::bind(&con_handler::handle_write,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void handle_read(const boost::system::error_code &err, size_t bytes_transferred)
    {
        if (!err) {
            // Dados recebidos podem ser tratados aqui
            cout << data << endl;
        } else {
            std::cerr << "Error: " << err.message() << std::endl;
            sock.close();
        }
    }

    void handle_write(const boost::system::error_code &err, size_t bytes_transferred)
    {
        if (!err) {
            // Dados a serem enviados devem chegar aqui
            cout << "Server sent Hello message!" << endl;
        } else {
            std::cerr << "error: " << err.message() << endl;
            sock.close();
        }
    }
};

class Server
{
private:
    boost::asio::io_context &io_context_;
    tcp::acceptor acceptor_;
    void start_accept()
    {
        con_handler::pointer connection = con_handler::create(io_context_);
        acceptor_.async_accept(connection->socket(),
            boost::bind(&Server::handle_accept, this, connection,
            boost::asio::placeholders::error));
    }
public:
    Server(boost::asio::io_service &io_context):
        io_context_(io_context),
        acceptor_(io_context_, tcp::endpoint(tcp::v4(), 1234))
    {
        start_accept();
    }

    void handle_accept(con_handler::pointer connection, const boost::system::error_code &err)
    {
        if (!err) {
            connection->start();
        }
        start_accept();
    }
};

int main(int argc, char *argv[])
{
    try
    {
        boost::asio::io_service io_service;
        Server server(io_service);
        io_service.run();
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << endl;
    }
    return 0;
}