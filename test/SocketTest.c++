#include <gtest/gtest.h>
#include "Socket.h++"

using namespace std;
using namespace StiltFox::DialUp;

TEST(Socket, listenPort_will_return_true_if_the_port_is_available_and_bound)
{
    //given we have a port that's not used.
    Socket openPort(0);
    
    //when we open the port for listening
    bool actual = openPort.listenPort();

    //then we get back that the port is opened
    EXPECT_TRUE(actual);
}

TEST(Socket, listenPort_will_return_false_if_the_port_is_already_taken)
{
    //given we have a port that's already open
    Socket openPort(400);
    Socket usedPort(400);
    openPort.listenPort();

    //when we open the second port
    bool actual = usedPort.listenPort();

    //then we get back that the port could not be opened
    EXPECT_FALSE(actual);
}

TEST(Connection, receiveData_will_get_the_information_sent_to_it_by_the_client)
{
    //given we have a port that we're listing to
    Socket openPort(0);
    openPort.listenPort();
    HttpMessage expectedData(HttpMessage::PUT, "/asdf", {}, "漬物はいいですよね。");

    //when we open a connection
    HttpMessage* actual = nullptr;

    thread connectionListenerThread([&openPort, &actual](){
        Connection* clientConnection = openPort.openConnection();
        actual = new HttpMessage(clientConnection->receiveData());
        delete clientConnection;
    });
    
    //and we send data to the port
    openPort.sendData(expectedData);
    connectionListenerThread.join();

    // //then the data is received
    EXPECT_EQ(actual[0], expectedData);
    if (actual != nullptr) delete actual;
}