FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

# get dependencies
RUN apt update
RUN apt-get install -y git
RUN apt-get install -y cmake
RUN apt-get install -y g++
RUN apt-get install -y libsasl2-dev
RUN apt-get install -y libsasl2-modules
RUN apt-get install -y libssl-dev
RUN apt-get install -y librdkafka-dev

# clone project
RUN git clone https://github.com/dmccoystephenson/librdkafka-testing-with-confluent-cloud

# change directory
WORKDIR /librdkafka-testing-with-confluent-cloud

# compile the project
RUN make

# Note: config and environment variables need to be set before running the project