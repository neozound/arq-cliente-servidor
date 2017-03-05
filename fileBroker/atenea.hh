#ifndef _ATENEA_HH_
  #define _ATENEA_HH_

class Server {
  protected:
    int max_size;
    int available_space;
    string ip;
    vector<string> file_list;

  public:
    Server();
    bool isAvailable();

};


#endif