#include "hermes.cc"

/*
It is capable of
loading a binary file
conecting to a server
recieving files and save them to disk
send stored files
remove files with a command
list the files stored in the server
*/

void listf(socket &socket_broker, string username);
void uploadf(socket &socket_broker, string username);
void downloadf(socket &socket_broker, string username);
void erasef(socket &socket_broker, string username);

int main(int argc, char const *argv[]) {

    if ( not ( 2 == argc ) ) {
      cerr << "Wrong number of arguments, remember to provide your username" << endl;
      return 1;
    }
    //take the username
    string username(argv[1]);
    cout << "Welcome " <<  username <<endl;

    //Created a context (blackbox)
    context brkr;
    //BROKER CONNECTION
    //Created the socket and the conection
    socket socket_broker(brkr, socket_type::request);
    socket_broker.connect("tcp://localhost:4242");

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
            listf(socket_broker,username);
            break;
        case 'U':
            uploadf(socket_broker,username);
            break;
        case 'D':
            downloadf(socket_broker,username);
            break;
        case 'E':
            erasef(socket_broker,username);
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

void listf(socket &socket_broker, string username){
    //ask for list
    message req;
    req << "list";
    socket_broker.send(req);
    //wait for answer
    message ans;
    string chain;
    socket_broker.receive(ans);
    ans >> chain;
    cout << "Files in the server: "<< chain << endl;

}

void uploadf(socket &socket_broker, string username){
  //declaration
  string filename;

  //get the name of the file
  cout << "write the filename: ";
  cin >> filename;
  //check if file exist
  ifstream infile(filename);
  if (!infile.good())
  {
    cout << "Bad filename!"<< endl;
    return;
  }//get out if bad filename

  //the privated client-server comand
  string cmd = "upload";
  string strMsg;
  message m;
  //create the object for manage the file
  //jack the ripper
  FileSplitter chop(filename);
  int size = chop.getSize();
  //send a message to the boker
  //whit the info of the file
  string ssize;
  ssize = number_to_string(size);
  m << username << cmd << filename << ssize;
  socket_broker.send(m);
  //wipe and receive;
  clean_message(m);
  socket_broker.receive(m);
  //the broker send:
  //status, the server ip and port
  string status;
  string servip;
  m >> status;
  if(status != "ok"){
    //something went wrong
    cout << "The broker answer with bad news"<< endl;
    return;
  }
    m >> servip;
    //establish new connection in an existent context
    cout << servip << endl;
    return;
    context srvr;
    socket socket_server(srvr, socket_type::push);
    socket_server.connect(servip);
    clean_message(m);
    //it's ready to send the file
    int number_of_parts = chop.getNumberOfParts();
    string tmp;
    tmp = number_to_string(number_of_parts);
    m << username << cmd << filename << ssize << tmp;
    socket_server.send(m);
    clean_message(m);
    //to this time te server is ready to receive all of the data
  while(!chop.isOver()){
    chop.nextChunkToMesage(m);
    socket_server.send(m);
    clean_message(m);
  }
  //ACK from the server
  socket_server.receive(m);

}

void downloadf(socket &socket_broker, string username){
  //first ask for the file
  //if file exists wait for answer
  //if file doesn't exist then exit
  string filename;

  //get the name of the file
  cout << "write the filename: ";
  cin >> filename;

  //the privated client-server comand
  string cmd = "download";
  string answer;

  message m;
  create_message(cmd, filename, m);
  socket_broker.send(m);

  socket_broker.receive(m);
  m >> answer;

  clean_message(m);

  if (answer == "bad")
  {
    cout << "Bad filename!"<< endl;
    return;
  }else if(answer == "good"){
    //recive the file
    socket_broker.send("ready to receive");
    //receive the number of parts
    clean_message(m);
    socket_broker.receive(m);
    int number_of_parts = 0;
    string cosas;
    cosas = m.get(0);
    number_of_parts = string_to_number(cosas);
    clean_message(m);
    socket_broker.send("everything is well");
    //define the prefix
    filename = "new_" + filename;
    remove( filename.c_str() );
    for (int i = 0; i < number_of_parts; ++i)
    {
      socket_broker.receive(m);
      socket_broker.send("ACK");
      messageToPartialFile(m,filename);
      clean_message(m);
    }
    socket_broker.receive(m);
    cout << "Finished" << endl;
  }
}

void erasef(socket &socket_broker, string username){
   //ask for the file deletion
  string filename;

  //get the name of the file
  cout << "write the filename: ";
  cin >> filename;

  //the privated client-server comand
  string cmd = "erase";
  string answer;

  message m;
  create_message(cmd, filename, m);
  socket_broker.send(m);

  socket_broker.receive(m);
  m >> answer;

  cout << "Attempting to delete a file from the server..." << answer << endl;
}