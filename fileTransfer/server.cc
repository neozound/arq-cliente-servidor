#include <zmqpp/zmqpp.hpp>
#include <iostream>

// formating the code: clang-format -i server.cc

/*
It is capable of
recieving incomming connections of a client
recieving files and save them to disk
send stored files
send a list of files stored into disk
remove files with a command
*/

int main() {
    cout << "This is the file server\n";

    context ctx;
    socket s(ctx, socket_type);

    cout << "Binding socket to tcp port 5555\n";
    s.bind("tcp://*:5555");

    while (true) {
      message m;
    }
    
    return 0;
}
