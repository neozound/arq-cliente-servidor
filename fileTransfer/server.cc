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

void listf(socket &s, const string &files);
void uploadf(socket &s,string filename, string &files);
void downloadf(socket &s);
void erasef(socket &s, string &files);

void clean_message(message& m);
void messageToFile(const message& msg, const string& fileName);

int main(int argc, char *argv[]) {
  // initialize the context (blackbox)
  context bbox;
  // generate a reply socket
  socket s(bbox, socket_type::reply);
  // bind to the socket
  s.bind("tcp://*:4242");
  
  

  //vector with all the uploaded filenames
  string files;

  while (true) {
    // receive the message
    message req;
    cout << "Receiving message..." << endl;
    s.receive(req);

    string option;
    string fname;
    req >> option;

    cerr << "option: " << option << endl;

    if (option == "list")
    {
      const string chain(files);
      listf(s,chain);
    }else if (option == "upload")
    {
      req >> fname;
      uploadf(s,fname,files);
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

void listf(socket &s, const string &files){
    message m;
    m << files;
    s.send(m);
    cout << "A client asked for list" << endl;
}

void uploadf(socket &s,string filename, string &files){
    message m;

    s.send("Ready to recieve");

    s.receive(m);
    filename = "uploaded_" + filename;
    messageToFile(m,filename);

    clean_message(m);
    cout << "Finished" << endl;
    s.send("Saved");

    //after saved the file add it to the 
    files += "\n" + filename;
}

void downloadf(socket &s){}

void erasef(socket &s, string &files){}

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
