FROM ubuntu:14.04

# Install prerequisites  ( you can switch between gcc an clang here )
#RUN apt-get -y update  && apt-get -y upgrade && apt-get -y install cmake byacc flex bison libxml2-dev clang-3.5 rpm && rm -rf /var/lib/apt/lists/*
RUN apt-get -y update  && apt-get -y upgrade && apt-get -y install cmake byacc flex bison libxml2-dev gcc g++ rpm && rm -rf /var/lib/apt/lists/*

# Add Sources and build directory
ADD . /certi
WORKDIR /certi/build
 
# Run CMake ( again, you can switch between gcc and clang here )
#RUN CC=clang-3.5 CXX=clang++-3.5 cmake -DCMAKE_INSTALL_PREFIX=/usr  -DBUILD_SHARED=OFF -DCOMPILE_WITH_CXX11=ON ..
RUN CC=gcc CXX=g++ cmake -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_SHARED=OFF -DCOMPILE_WITH_CXX11=ON ..

# Make
RUN make
RUN make package
RUN make install
