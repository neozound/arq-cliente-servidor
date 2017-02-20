#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace zmqpp;

// formating the code: clang-format -i server.cc

/*
It is capable of
recieving incomming connections of a client
recieving files and save them to disk
send stored files
send a list of files stored into disk
remove files with a command
*/

void listf(socket &s);
void uploadf(socket &s,string filename);
void downloadf(socket &s);
void erasef(socket &s);

void clean_message(message& m);
void messageToFile(const message& msg, const string& fileName);

int main(int argc, char *argv[]) {
  // initialize the context (blackbox)
  context bbox;
  // generate a reply socket
  socket s(bbox, socket_type::reply);
  // bind to the socket
  s.bind("tcp://*:4242");

  while (true) {
    // receive the message
    cout << "Receiving message..." << endl;
    message req;
    s.receive(req);

    string option;
    string fname;
    req >> option;
    req >> fname;

    if (option == "list")
    {
      listf(s);
    }else if (option == "upload")
    {
      uploadf(s,fname);
    }else if (option == "")
    {
      /* code */
    }else if (option == "")
    {
      /* code */
    }else{

    }

  cout << "Finished." << endl;
}
}

//the name of the function indiques the request of the client

void listf(socket &s){
    message req;
    req << "Ok!, List request received";
    s.send(req);
    cout << "A client asked for list" << endl;
}

void uploadf(socket &s,string filename){
    message m;

    s.send("Ready to recieve");

    s.receive(m);
    filename = "down_" + filename;
    messageToFile(m,filename);

    clean_message(m);
    cout << "Finished" << endl;
    s.send("Saved");
}

void downloadf(socket &s){}

void erasef(socket &s){}

//file manager functions
void clean_message(message& m){
  while(m.parts() > 0){
    m.pop_back();
  }
}

void messageToFile(const message& msg, const string& fileName) {
  const void *data;
  msg.get(&data, 0);
  size_t size = msg.size(0);

  ofstream ofs(fileName, ios::binary);
  ofs.write((char*)data, size);
}
