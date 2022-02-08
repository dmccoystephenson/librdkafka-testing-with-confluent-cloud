# librdkafka testing with confluent cloud
Testing the utilization of the librdkafka library with Confluent Cloud.

## Steps to Compile
1) Pull the librdkafka submodule
2) Enter the "librdkafka" directory
3) Type cmake .
4) Set required environment variables and alter the config file if necessary
5) Type make
6) Return to the project directory
7) Type make

## Required Environment Variables If Hard Coding Configuration
- CONFLUENT_BROKERS
- CONFLUENT_KEY
- CONFLUENT_SECRET
- TOPIC
- MESSAGE

## Required Environment Variables If Loading Config From File
- TOPIC
- MESSAGE
