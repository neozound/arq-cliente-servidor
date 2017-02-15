#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <string>
#include <vector>
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
void uploadf(socket &s, message &m);
void downloadf(socket &s);
void erasef(socket &s);

int main(int argc, char *argv[]) {
  // initialize the context (blackbox)
  context bbox;
  // generate a reply socket
  socket s(bbox, socket_type::reply);
  // bind to the socket
  s.bind("tcp://*:4242");

  //while (true) {
    // receive the message
    cout << "Receiving message..." << endl;
    message req;
    s.receive(req);

    string option;
    req >> option;

    if (option == "list"){
      listf(s);
    }
    if (option == "upload"){
      uploadf(s, req);
    }
    if (option == ""){
      /* code */
    }
    if (option == ""){
      /* code */
    }
    else{

    }

  cout << "Finished." << endl;
//}
}



void listf(socket &s){
    message req;
    req << "Ok!, List request received";
    s.send(req);
    cout << "A client asked for list" << endl;
}

void uploadf(socket &s, message &m){
  long size;
  string fname;
  cout << "Entro a upload";
  //ofstream outfile (fname,ios::binary);


  m >> fname;
  fname = "1" + fname;
  m >> size;

  cout << fname;
  cout << size;

  const char* dataPointer;
  dataPointer = new char(size);
  dataPointer = static_cast<const char*>(m.raw_data(2));

  ofstream outfile(fname,  ios::binary);
  outfile.write(dataPointer, size);
  outfile.close();
}

void downloadf(socket &s){}

void erasef(socket &s){}
