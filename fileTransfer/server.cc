#include <iostream>
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <fstream>

using namespace std;
using namespace zmqpp;

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

  cout << cmd << " " << filename << endl;
  return 0;
}
