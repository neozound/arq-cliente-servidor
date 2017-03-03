# Client Server Architecture Repository

In this repository, we have stored the files related to our C/S Architecture
class.

In this repo you can find the next projects:

1. Client to Server - File uploader/downloader/eraser

Dependencies:

We are using the ZMQ and ZMQPP libraries. In order to compile the projectos
you have to install them manually from the source, which is really easy and fast.

```shell
sudo apt-get install libtool pkg-config build-essential autoconf automake
cd Downloads
sudo apt-get install cmake
wget https://github.com/zeromq/libzmq/releases/download/v4.2.1/zeromq-4.2.1.tar.gz
tar xvfz zeromq-4.2.1.tar.gz
mkdir buildzmq
cd buildzmq/
cmake ../zeromq-4.2.1
make -j 4
sudo make install
cd ..
git clone https://github.com/zeromq/zmqpp.git
mkdir buildzmqpp
cd buildzmqpp/
cmake ../zmqpp/
make -j 4
sudo make install
cd ..
```

And set the PATH variables for easy execution of the application:

```shell
sudo gedit /etc/ld.so.conf.d/randomLibs.conf
```

And within the file write the path to your libraries, or do whereis to look for them:

```shell
/usr/local/lib
```

At last you do:


```shell
sudo ldconfig
```
