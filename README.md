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
6) Set required environment variables and alter the config file if necessary
7) Type make

## Environment Variables
### Required Environment Variables If Loading Config From File
- TOPIC
- MESSAGE

It should be noted that when loading the config from a file, the "cc.config" file needs to be edited to include the brokers, key and secret for Confluent Cloud integration.

### Required Environment Variables If Hard Coding Configuration
- CONFLUENT_BROKERS
- CONFLUENT_KEY
- CONFLUENT_SECRET
- TOPIC
- MESSAGE
