#include "hermes.cc"
#include "atenea.hh"

// formating the code: clang-format -i server.cc

/*
It is capable of
recieving incomming connections of a client
and send to the apropiate server
*/
int main(int argc, char *argv[])
{
  vector<Server> servers;
  // initialize the context (blackbox)
  context sbox;
  context cbox;
  // generate a reply socket
  socket socket_clients(cbox, socket_type::reply);
  socket socket_servers(sbox, socket_type::pull);
  // bind to the socket
  socket_clients.bind("tcp://*:4242");
  socket_servers.bind("tcp://*:4243");
  
  //string with a list of the uploaded filenames
  string files;

  //definition of a poller
  poller mein_poll;
  int console = fileno(stdin);
  //added to the poller the stdin and the sockets
  mein_poll.add(socket_clients, poller::poll_in);
  mein_poll.add(socket_servers, poller::poll_in);
  mein_poll.add(console, poller::poll_in);
  //the endless history (loop)
  while (true){
      if(mein_poll.poll()){
          //read a message from the sockets
          if(mein_poll.has_input(socket_clients)){
            string username, cmd,filename,size;
            message mc;//hammer!
            socket_clients.receive(mc);
            //the message sintax is:
            //username, command, filename, size
            mc >> username;
            mc >> cmd;
            if (cmd == "list"){
              //do something
            }
            if(cmd == "upload"){
              mc >> filename;
              mc >> size;
              cout << "User " << username << " is trying to upload " << size << " bytes of the file " << filename << endl;
              //select the best server
              string file;
              file = username + "_uploaded_" + filename;
              string server_ip = selectServer(file, size,servers);
              if (server_ip == "null")
              {
                socket_clients.send("no ok :(");
              }else{
                clean_message(mc);
                mc << "ok" << server_ip << file;
                socket_clients.send(mc);
                //update the list of servers
                update_upload(server_ip, file, size);
              }
            }
            if (cmd == "download"){
              //do something 
            }
            if (cmd == "erase"){
              //do something
            }

          }
          if(mein_poll.has_input(socket_servers)){
            string server_ip, cmd,filename,max_size;
            message ms;//hammer!
            socket_servers.receive(ms);
            //the message sintax is:
            //username, command, ip address
            ms >> server_ip;
            ms >> cmd;
            if (cmd == "connect"){
              ms >> max_size;
              long long int msize = string_to_big_number(max_size);
              cout << "the server " << server_ip << " is now online"<< endl;
              Server new_server(server_ip,msize);
              servers.push_back(new_server);
            }
            if (cmd == "busy"){
              cout << "the server " << server_ip  << " is now in a busy state"<< endl;
            }
            if (cmd == "ready"){
              cout << "the server " << server_ip  << " is now ready"<< endl;
            }
            if (cmd == "disconnect"){
              cout << "the server " << server_ip  << " is now offline"<< endl;
            }
          }
          //if the stdin has messages
          if(mein_poll.has_input(console)){
              //do something
            }
        }
    }
}

string selectServer(string file, string size, vector<Server> &servers){
  float lowest_charge = 100; //percent of charge
  string selected_ip("null");
  for(auto serv_i : servers)
  {
    if (serv_i.isAvailable()){
      //take every available server
      //and select te ip (only if the charge is lower)
      float charge = serv_i.getCharge();
      if (charge < lowest_charge){
        selected_ip = serv_i.getAddress();
        lowest_charge = charge;
      }
    }
  }
  //after look all the servers return the selected ip
  return selected_ip;
}

void update_upload(string server_ip, string file, string size){
  return;
}


Server::Server(string ip, long long int max_size) :ip(ip),max_size(max_size) {
  status = "ready";
  available_space = max_size;
}

string Server::getAddress(){
  return ip;
}

bool Server::isAvailable(){
  return ("ready" == status);
}

float Server::getCharge(){
  float charge;
    charge = 100 - (available_space*100/max_size);
  return charge;
}