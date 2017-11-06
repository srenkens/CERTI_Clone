FROM ubuntu:16.04

#install prerequisites 
RUN apt-get -y update  && \
    apt-get -y upgrade && \
    apt-get -y install cmake byacc flex bison libxml2-dev clang-3.8 rpm && \
    rm -rf /var/lib/apt/lists/*

# Add Sources and build directory
ADD . /certi
WORKDIR /certi/build
 
#the magic bit. gcc crashes on a circular reference error, clang doesn't
RUN CC=clang-3.8 CXX=clang++-3.8 cmake -DCMAKE_INSTALL_PREFIX=/usr ..
RUN make
RUN make package
RUN make install
