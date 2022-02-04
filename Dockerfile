## Th
##
##
##
##

FROM ubuntu:18.04
USER root

WORKDIR /asn1_codec

VOLUME ["/asn1_codec_share"]

# install make
RUN apt-get update && apt-get install -y make

# Add build tools.
RUN apt-get update && apt-get install -y software-properties-common wget git make gcc-7 g++-7 gcc-7-base && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 100 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 100

# Install librdkafka.
ADD ./librdkafka /asn1_codec/librdkafka
RUN cd ./librdkafka && make && make install

RUN echo "export LD_LIBRARY_PATH=/usr/local/lib" >> ~/.profile
RUN echo "export LD_LIBRARY_PATH=/usr/local/lib" >> ~/.bashrc
RUN echo "export CC=gcc" >> ~/.profile
RUN echo "export CC=gcc" >> ~/.bashrc

# run ACM
RUN g++ src/testing.cpp -o testing ./librdkafka/src-cpp/rdkafkacpp.h