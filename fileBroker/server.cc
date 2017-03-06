#include "hermes.cc"

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

int main(int argc, char *argv[])
{
  if ( not ( 3 == argc ) ) {
    cerr << "Wrong number of arguments, remember to provide your server ip address and port" << endl;
    return 1;
  }
  //take the sid
  string sip(argv[1]);
  string port(argv[2]);

  // initialize the context (blackbox)
  context bbox;
  // generate a reply socket
  socket socket_broker(bbox, socket_type::push);
  socket socket_clients(bbox, socket_type::pull);
  // bind to the socket
  socket_broker.connect("tcp://localhost:4243");
  string cip("tcp://*:");
  cip = cip + port;
  socket_clients.bind(cip);

  //full ip is de identity of the server
  string full_ip("tcp://");
  full_ip += sip + ":" + port;

  //send a echo of connect
  message mess;
  mess << full_ip << "connect";
  socket_broker.send(mess);
  clean_message(mess);

  while (true){
    string uname,cmd,fname,size,parts;
    socket_clients.receive(mess);
    //the message sintax is:
    //command, filename, size, nparts
    mess >> cmd;
    if(cmd == "upload"){
      mess >> fname;
      mess >> size;
      mess >> parts;
      int nparts = string_to_number(parts);
      clean_message(mess);
      cout << "Incomming file: " << fname << " (" << size << " bytes )" << endl;

      //send to broker: busy
      mess << full_ip << "busy";
      socket_broker.send(mess);
      clean_message(mess);

      //start to receive the file
      remove( fname.c_str() );
      for (int i = 0; i < nparts; ++i)
      {
        socket_clients.receive(mess);
        messageToPartialFile(mess,fname);
        clean_message(mess);
      }

      //send to broker: ready
      mess << full_ip << "ready";
      socket_broker.send(mess);
      clean_message(mess);
    }

  }
 
  //ready
  mess << full_ip << "ready";
  socket_broker.send(mess);
  clean_message(mess);
  //disconnect
  mess << full_ip << "disconnect";
  socket_broker.send(mess);
  clean_message(mess);

}

//the name of the function indiques the request of the client

void listf(socket &s, const string &files)
{
  //create a message an put in it the files list
  message m;
  m << files;
  //send to the client the files list
  s.send(m);
  //report the action
  cout << "A client asked for list" << endl;
}

void uploadf(socket &s,string filename, string &files)
{
  //create a message and start a "conversation" with the client
  message m;
  s.send("Ready to recieve");
  
  s.receive(m);
    int number_of_parts = 0;
    string cosas;
    cosas = m.get(0);
    number_of_parts = string_to_number(cosas);
    clean_message(m);
    s.send("everything is well");
    //define the prefix
    filename = "new_" + filename;
    remove( filename.c_str() );
    for (int i = 0; i < number_of_parts; ++i)
    {
      s.receive(m);
      s.send("ACK");
      messageToPartialFile(m,filename);
      clean_message(m);
    }
  
  //after saved the file add it to the 
  seek_n_destroy(files,filename);
  files += "\n" + filename;
}

void downloadf(socket &s,string filename)
{
 //declaration of varibles
  string strMsg;

  //check if file exist
  ifstream infile(filename);
  if (!infile.good())
  {
    //if it doen't exist, give the bad news to the client
    s.send("bad");
    return;
  }//get out

  //if All Izz Well report it!
  s.send("good");
  cout << "Client downloading "<< filename << "... ";

  //receive the ACK (Acknowledgement) from the client
  message m;
  string ack;
  s.receive(m);
  m >> ack;
  //report the news from the client   
  cout << ack << endl;
  //wipe the message
  clean_message(m);
  //create the object for manage the file
  //jack the ripper
  FileSplitter chop(filename);
  int number_of_parts = chop.getNumberOfParts();
  string tmp;
  tmp = number_to_string(number_of_parts);
  m << tmp;
  s.send(m);
  cout << "Sending " << number_of_parts << " parts" << endl;
  s.receive(m);
  clean_message(m);
  while(!chop.isOver()){
    chop.nextChunkToMesage(m);
    s.send(m);
    s.receive(m);
    clean_message(m);
  }
  s.send("End!");
  //report to the console that all is ou-key
  cout << "ok" << endl;
}

void erasef(socket &s, string filename , string &files)
{
  //report what the hell are the client trying to do 
  cout << "Client erasing "<< filename << "... ";
  //try to delete a file and send the result to the client
  if( remove( filename.c_str() ) != 0 ){
    //report the bad news
    s.send("Error deleting file" );
    cout << "Error" << endl;
  }else{
    // proclaim the good news
    s.send("File successfully deleted" );
    cout << "Ok" << endl;
    //if ok then delete the file from the list
    filename = "\n" + filename;
    seek_n_destroy(files,filename);
  }
  return ;
}
