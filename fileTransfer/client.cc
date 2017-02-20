#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

using namespace std;
using namespace zmqpp;

/*
It is capable of
loading a binary file
conecting to a server
recieving files and save them to disk
send stored files
remove files with a command
list the files stored in the server
*/


void listf(socket &s);
void uploadf(socket &s);
void downloadf(socket &s);
void erasef(socket &s);

void clean_message(message& m);
vector<char> readFileToBytes(const string& fileName) ;
void fileToMesage(const string& fileName, message& msg);
void create_message(const string& cmd, const string& filename, message& msg);

int main(int argc, char const *argv[]) {

    //Created a context (blackbox)
    context bbox;
    //Created the socket and the conection
    socket s(bbox, socket_type::request);
    s.connect("tcp://localhost:4242");

    char option=' ';
    cout << "Available options:" << endl;
    cout << " * (L)ist" << endl;
    cout << " * (U)pload" << endl;
    cout << " * (D)ownload" << endl;
    cout << " * (E)rase" << endl;
    cout << " * E(X)it" << endl;
    do
    {
      //display of the options
      cout << "> ";
      cin >> option;

      switch(option){
        case 'L':
            listf(s);
            break;
        case 'U':
            uploadf(s);
            break;
        case 'D':
            downloadf(s);
            break;
        case 'E':
            erasef(s);
            break;
        case 'X':
            return 0;
            break;
        default:
            cout << "Unrecognized option: " << option << endl;
        break;
      }
    } while (option!='X');

  cout << "Finished." << endl;
  return 0;
}

void listf(socket &s){
    //ask for list
    message req;
    req << "list";
    s.send(req);
    cout << "List request sent." << endl;
    //wait for answer
    message ans;
    string chain;
    s.receive(ans);
    ans >> chain;
    cout << "Response: " << chain << endl;

}

void uploadf(socket &s){
  //declaration
  streampos size;
  string filename;

  //get the name of the file
  cout << "write the filename: ";
  cin >> filename;

  //the privated client-server comand
  string cmd = "upload";
  string strMsg;

  message m;
  create_message(cmd, filename, m);
  s.send(m);

  s.receive(m);
  m >> strMsg;
  cout << strMsg << endl;

  clean_message(m);

  fileToMesage(filename,m);
  s.send(m);
  s.receive(m);

  m >> strMsg;
  cout << strMsg << endl;
  clean_message(m);
  
}

void downloadf(socket &s){}

void erasef(socket &s){}


//Functions of messaging & file manage

void clean_message(message& m){
  while(m.parts() > 0){
    m.pop_back();
  }
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
