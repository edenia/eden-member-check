FROM ubuntu:focal
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -yq    \
    binaryen                \
    build-essential         \
    cmake                   \
    gdb                     \
    git                     \
    libboost-all-dev        \
    libcurl4-openssl-dev    \
    libgmp-dev              \
    libssl-dev              \
    libusb-1.0-0-dev        \
    pkg-config              \
    wget
RUN mkdir /root/work
WORKDIR /root/work 
RUN /bin/bash -c 'wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sdk-12.0-linux.tar.gz; \ 
	tar xf wasi-sdk-12.0-linux.tar.gz'
RUN /bin/bash -c 'wget https://github.com/eoscommunity/Eden/releases/download/sdk-v0.1.0-alpha/clsdk-ubuntu-20-04.tar.gz; \
	tar xf clsdk-ubuntu-20-04.tar.gz'
COPY /edenmember edenmember/
RUN /bin/bash -c 'cd edenmember ; export  WASI_SDK_PREFIX=/root/work/wasi-sdk-12.0; export CLSDK_PREFIX=/root/work/clsdk; export PATH=$CLSDK_PREFIX/bin:$PATH ; mkdir build;cd build; cmake `clsdk-cmake-args` .. ;make -j'
#RUN /bin/bash -c 'cd ~/work/edenmember ; mkdir build;cd build; cmake `clsdk-cmake-args` .. ;make -j'

