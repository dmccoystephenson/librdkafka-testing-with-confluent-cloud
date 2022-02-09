# librdkafka testing with confluent cloud
Testing the utilization of the librdkafka library with Confluent Cloud.

## SASL
Install SASL via the command line. The librdkafka library will use this during compilation.

## Steps to Compile
1) Type git submodule update --init --recursive
2) Enter the "librdkafka" directory
3) Type cmake .
4) Type make
5) Return to the project directory
6) Set required environment variables.
7) If you are loading the config in from a file, edit the "cc.config" file to include the brokers, key and secret for Confluent Cloud integration.
8) Type make

## Environment Variables
### Required Environment Variables If Loading Config From File
- TOPIC
- MESSAGE

### Required Environment Variables If Hard Coding Configuration
- CONFLUENT_BROKERS
- CONFLUENT_KEY
- CONFLUENT_SECRET
- TOPIC
- MESSAGE

## cc.config possible options
- bootstrap.servers
- security.protocol
- sasl.mechanisms
- sasl.username
- sasl.password
- debug
