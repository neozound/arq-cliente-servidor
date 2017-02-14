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

int main(int argc, char const *argv[]) {

    //Created a context (blackbox)
    context bbox;
    //Created the socket and the conection
    socket s(bbox, socket_type::request);
    s.connect("tcp://localhost:4242");

    char option=' ';
    do
    {
      //display of the options
      cout << "==== Select an option:" << endl;
      cout << " * (L)ist" << endl;
      cout << " * (U)pload" << endl;
      cout << " * (D)ownload" << endl;
      cout << " * (E)rase" << endl;
      cout << " * E(X)it" << endl;
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
  streampos size;
  char * memblock;

  //ate: Is a flag that points the file pointer to the end of the file
  ifstream file ("escher.jpg", ios::in|ios::binary|ios::ate);
  ofstream outputFile ("escher2.jpg", ios::binary);

  if (file.is_open())
  {
    /*
    tellg() gets the position that points file, in this case it means the
    total size of file
    */
    size = file.tellg();
    cout << size << endl;
    // We now request the allocation of a memory as big as size
    memblock = new char [size];
    //we position the pointer to the begin of the file
    file.seekg(0, ios::beg);
    //we store the content of the read file to memblock
    file.read(memblock, size);
    file.close();

    cout << "the entire file is in memory" << endl;

    outputFile.write (memblock, size);

    outputFile.close();

    delete[] memblock;
  }

}

void downloadf(socket &s){}

void erasef(socket &s){}
