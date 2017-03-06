#ifndef _ATENEA_HH_
  #define _ATENEA_HH_

#include <unistd.h>

class Server {
  protected:
    string ip;
    long long int max_size;
    string status; //online, offline, busy, ready
    long long int available_space;
    vector<string> file_list;

  public:
    Server(string ip);
    bool isAvailable();
    long long int availableSpace();
    void changeStatus(string new_status);
    string getAddress();


};

string selectServer(string file, string size,vector<Server> &servers);
void update_upload(string server_id, string file, string size);

#endif