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
send a list of files stored into disk
remove files with a command
*/

int main() {
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

    return 0;
}
