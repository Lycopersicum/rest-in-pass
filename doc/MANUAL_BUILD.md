## Building Manually

1. Download [rest-in-pass](https://github.com/Lycopersicum/rest-in-pass.git):
```
$ git clone --recursive https://github.com/Lycopersicum/rest-in-pass.git
$ cd rest-in-pass
```
_Note: If you already cloned REST In Pass without initializing submodules, you can do so by executing:_
```
$ git submodule update --init --recursive
```
2. Install required tools and libraries (example is for Debian based distributions):
```
$ sudo apt-get update
$ sudo apt-get install git-core cmake build-essential automake libtool gnutls-bin
$ sudo apt-get install libgnutls28-dev libmicrohttpd-dev libjansson-dev
```

3.1. Build and install Ulfius library (located in `third_party/ulfius`). You can do so by following instructions:
```
$ rm -rf third_party/ulfius/build
$ mkdir third_party/ulfius/build
$ cd third_party/ulfius/build
$ cmake ../ -DWITH_YDER=off -DBUILD_UWSC=off -DWITH_GNUTLS=on -DWITH_CURL=on -DBUILD_STATIC=on
$ make clean
$ make
$ sudo make install
$ cd -
```
_Note: alternatively you can follow [babelouest/ulfius](https://github.com/babelouest/ulfius) repository instructions._


4. Build REST In Pass server

If you installed locally or want to build REST In Pass the easy way:
```
$ SKIP_BOOTSTRAP=true ./script/setup
```
or build manually (_Note: this method should work only for globally installed libraries._).
```
$ mkdir build
$ cd build/
$ cmake ../
$ make clean
$ make
```
After last step you should have binary file called `rest-in-pass` in your `rest-in-pass/build/` directory.

