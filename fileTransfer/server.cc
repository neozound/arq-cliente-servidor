#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <regex>

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
void downloadf(socket &s,string filename);
void erasef(socket &s, string filename, string &files);

void seek_n_destroy(string& victim, string& part);

void clean_message(message& m);
void messageToFile(const message& msg, const string& fileName);
vector<char> readFileToBytes(const string& fileName) ;
void fileToMesage(const string& fileName, message& msg);
void create_message(const string& cmd, const string& filename, message& msg);



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
    cout << "Receiving message... ";
    s.receive(req);

    string option;
    string fname;
    req >> option;

    cerr << option << endl;

    if (option == "list")
    {
      const string chain(files);
      listf(s,chain);
    }else if (option == "upload")
    {
      req >> fname;
      uploadf(s,fname,files);
    }else if (option == "download")
    {
      req >> fname;
      downloadf(s,fname);
    }else if (option == "erase")
    {
      req >> fname;
      erasef(s,fname,files);
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
    cout << "Image " << filename << " saved" << endl;
    s.send("Saved");

    //after saved the file add it to the 
    files += "\n" + filename;
}

void downloadf(socket &s,string filename){
 //declaration
  streampos size;
  string strMsg;

  //check if file exist
  ifstream infile(filename);
  if (!infile.good())
  {
    s.send("bad");
    return;
  }//get out if bad filename

  s.send("good");
  cout << "Client downloading "<< filename << "... ";

  message m;
  s.receive(m);
  string ack;
  m >> ack;
  
  cout << ack << "... ";

  clean_message(m);
  fileToMesage(filename,m);
  s.send(m);

  cout << "ok" << endl;
  
}

void erasef(socket &s, string filename , string &files){
  //try to delete a file and send the result to the client
  cout << "Client erasing "<< filename << "... ";
   if( remove( filename.c_str() ) != 0 ){

    s.send("Error deleting file" );
    cout << "Error" << endl;
   }else{
    s.send("File successfully deleted" );
    cout << "Ok" << endl;
    //if ok then delete the file from the list
    seek_n_destroy(filename,files);
  }
  return ;
}

//other functions

void seek_n_destroy(string& victim, string& part)
{
  //kill 'em all
  regex pattern(part);
  cout << regex_replace(victim, pattern, "");
}
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

vector<char> readFileToBytes(const string& fileName) {
  ifstream ifs(fileName, ios::binary | ios::ate);
  ifstream::pos_type pos = ifs.tellg();

  vector<char> result(pos);

  ifs.seekg(0, ios::beg);
  ifs.read(result.data(), pos);

  return result;
}

void fileToMesage(const string& fileName, message& msg) {
  vector<char> bytes = readFileToBytes(fileName);
  msg.add_raw(bytes.data(), bytes.size());
}

void create_message(const string& cmd, const string& filename, message& msg){
  msg << cmd << filename;
}
