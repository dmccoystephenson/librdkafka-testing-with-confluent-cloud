# librdkafka testing with confluent cloud
Testing the utilization of the librdkafka library with Confluent Cloud.

## SASL
Install SASL via the command line. The librdkafka library will use this during compilation.

## Steps to Compile
1) Type git submodule update --init --recursive
2) Enter the "librdkafka" directory
3) Type cmake .
4) Set required environment variables and alter the config file if necessary
5) Type make
6) Return to the project directory
7) Type make

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
