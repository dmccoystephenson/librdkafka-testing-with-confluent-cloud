# librdkafka testing with confluent cloud
Testing the utilization of the librdkafka library with Confluent Cloud.

## SASL
Install SASL via the command line. The librdkafka library will use this during compilation.

## Steps to Test
1) Build the docker image
2) Run the docker image
3) Exec into the docker image
4) Alter the cc.config file to include the brokers, key and secret for Confluent Cloud integration
5) Set the TOPIC and MESSAGE environment variables
6) Compile the project by typing make
7) Execute the project by typing ./testing

## Environment Variables
### Required Environment Variables If Loading Config From File
- TOPIC
- MESSAGE

## cc.config possible options
- bootstrap.servers
- security.protocol
- sasl.mechanisms
- sasl.username
- sasl.password
- debug
- api.version.request
- api.version.fallback
- broker.version.fallback
