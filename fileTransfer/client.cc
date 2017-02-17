#include <iostream>
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <fstream>

using namespace std;
using namespace zmqpp;

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

int main(){
  string cmd = "upload";
  string filename = "lincoln.jpg";

  context ctx;
  socket s(ctx, socket_type::req);
  s.connect("tcp://localhost:5555");

  message m;
  create_message(cmd, filename, m);
  s.send(m);
  cout << m.parts();

  return 0;
}
