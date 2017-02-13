#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

using namespace std;
using namespace zmqpp;

/*
It is capable of
conecting to a server
recieving files and save them to disk
send stored files
send a list of files stored into disk
remove files with a command
*/

int main() {
    ifstream input( "escher.jpg", ios::binary);
    vector<char> buffer((
      istream_iterator<char>(input)),
      (istreambuf_iterator<char>()));
}
