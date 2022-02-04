FROM ubuntu:18.04
USER root

WORKDIR /asn1_codec

VOLUME ["/asn1_codec_share"]

# Add build tools.
RUN apt-get update && apt-get install -y software-properties-common wget git make gcc-7 g++-7 gcc-7-base && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 100 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 100

#install editors vim and nano
#RUN apt-get update && apt-get install -y vim
#RUN apt-get update && apt-get install -y nano

# Install cmake.
RUN wget https://cmake.org/files/v3.7/cmake-3.7.2.tar.gz && tar -xvf cmake-3.7.2.tar.gz
RUN cd cmake-3.7.2 && ./bootstrap && make && make install && cd /home

# install libtool and automake
RUN apt-get update && apt-get install -y automake libtool

# Install librdkafka.
ADD ./librdkafka /asn1_codec/librdkafka
RUN cd /asn1_codec/librdkafka && ln -s /usr/bin/python3 /usr/bin/python && ./configure && make && make install

RUN echo "export LD_LIBRARY_PATH=/usr/local/lib" >> ~/.profile
RUN echo "export LD_LIBRARY_PATH=/usr/local/lib" >> ~/.bashrc
RUN echo "export CC=gcc" >> ~/.profile
RUN echo "export CC=gcc" >> ~/.bashrc

# run ACM
RUN make && make install
CMD ["./testing"]