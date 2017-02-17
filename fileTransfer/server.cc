#include <iostream>
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <fstream>

using namespace std;
using namespace zmqpp;

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

int main(){
  string cmd;
  string filename;
  context ctx;
  socket s(ctx, socket_type::rep);
  s.bind("tcp://*:5555");

  message m;
  s.receive(m);
  m >> cmd;
  m >> filename;

  clean_message(m);

  cout << cmd << " " << filename << endl;

  s.send("Ready to recieve");

  s.receive(m);
  filename = "down" + filename;
  messageToFile(m,filename);

  clean_message(m);
  cout << "Finished" << endl;
  s.send("Saved");
  return 0;
}
