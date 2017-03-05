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
              string file,ip;
              file = username + "_uploaded_" + filename;
              int server_id = selectServer(file, size);
              if (server_id == -1)
              {
                socket_clients.send("no ok :(");
              }else{
                ip = getServerIp(server_id);
                clean_message(mc);
                mc << "ok" << ip;
                socket_clients.send(mc);
                //update the list of servers
                update_upload(server_id, file, size);
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
            string str_id, cmd,filename,size;
            message ms;//hammer!
            socket_servers.receive(ms);
            //the message sintax is:
            //username, command, total space
            ms >> str_id;
            ms >> cmd;
            if (cmd == "connect"){
              cout << "se ha conectado el servidor " << str_id << endl;
            }
          }
          //if the stdin has messages
          if(mein_poll.has_input(console)){
              //do something
            }
        }
    }
}

int selectServer(string file, string size){
  int id= -1;
  return id;
}

string getServerIp(int server_id){
  string ip("tcp://localhost:4244");
  return ip;
}

void update_upload(int server_id, string file, string size){
  return;
}
