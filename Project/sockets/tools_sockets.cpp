#pragma once

#include "tools_sockets.hpp"
#include <thread>
#include <iostream>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/serialization/serialization.hpp>


using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
namespace pt = boost::property_tree;

using namespace std;
using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

string read_(tcp::socket& socket) {
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, "\n");
    string data = boost::asio::buffer_cast<const char*>(buf.data());
    return data;
}
void send_(tcp::socket& socket, const string& message) {
    const string msg = message + "\n";
    boost::asio::write(socket, boost::asio::buffer(message));
}


void connexionSocket() {
    boost::asio::io_service io_service;             //ioservice object is needed whenever a program is using asio.

    //socket creation
    tcp::socket socket(io_service);
    //connection
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234));            // connect to the server using localhost and the 
    // same port as we did for the serve

// request/message from client
    const string msg = "Hello from Client!\n";
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(msg), error);
    if (!error) {
        cout << "Client sent hello message!" << endl;
    }
    else {
        cout << "send failed: " << error.message() << endl;
    }
    // getting response from server
    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
    if (error && error != boost::asio::error::eof) {
        cout << "receive failed: " << error.message() << endl;
    }
    else {
        const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        cout << data << endl;
    }
}



void serverSocket() {
    boost::asio::io_service io_service;                 //io_service object is needed whenever a program is using asio.

    //listen for new connection
    tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234));        //connection being initialised to ipv4 and port 1234

    //socket creation 
    tcp::socket socket_(io_service);
    //waiting for connection
    acceptor_.accept(socket_);
    //read operation
    string message = read_(socket_);
    cout << message << endl;
    //write operation
    send_(socket_, "Hello From Server! Tu as compris sa marche enculé");
    cout << "Server sent Hello message to Client!" << endl;

    socket_.close();

}
