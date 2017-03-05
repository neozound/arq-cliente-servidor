#ifndef _ATENEA_HH_
  #define _ATENEA_HH_

#include <unistd.h>

class Server {
  protected:
    int id;
    int max_size;
    int available_space;
    string ip;
    vector<string> file_list;

  public:
    Server();
    bool isAvailable();

};

int selectServer(string file, string size);
string getServerIp(int server_id);
void update_upload(int server_id, string file, string size);

#endif