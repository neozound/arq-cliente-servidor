#ifndef _ATENEA_HH_
  #define _ATENEA_HH_

#include <unistd.h>

class Server {
  protected:
    string id;
    long long int max_size;
    string status; //online, offline, busy, ready
    long long int available_space;
    string ip;
    vector<string> file_list;

  public:
    Server(string id,string ip);
    bool isAvailable();
    long long int availableSpace();
    void changeStatus(string new_status);


};

string selectServer(string file, string size);
string getServerIp(string server_id);
void update_upload(string server_id, string file, string size);

#endif