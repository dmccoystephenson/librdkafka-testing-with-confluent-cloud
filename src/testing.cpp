#include <string>
#include <iostream>
#include "librdkafka/rdkafkacpp.h"

using namespace std;

void log(string message) {
    cout << "[LOG] " << message << endl;
}

int main() {
    log("Executing program.");
    
    // prepare variables
    string brokers = "pkc-43n10.us-central1.gcp.confluent.cloud:9092";
    string topic = "topic.Asn1DecoderInput";
    const char* username = ""; // SET
    const char* password = ""; // SET
    string errorString = "";
    string confType = "hardcode"; // hardcode or file

    // create configuration
    RdKafka::Conf *conf =RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    cout << "confType: " << confType << endl;
    if (confType == "hardcode") {
        log("Creating configuration manually.");

        // set up confluent cloud config
        log("Setting up Confluent Cloud configuration key/value pairs.");

        conf->set("bootstrap.servers", brokers, errorString);
        conf->set("ssl.endpoint.identification.algorithm", "https", errorString);
        conf->set("security.protocol", "SASL_SSL", errorString); // SSL_SASL or SSL_PLAINTEXT
        conf->set("sasl.mechanisms", "PLAIN", errorString);
        conf->set("sasl.username", username, errorString);
        conf->set("sasl.password", password, errorString);

        log("Finished setting up Confluent Cloud configuration key/value pairs.");
    }
    else if (confType == "file") {
        log("Loading configuration in from file.");
        // TODO: load config in from file
    }

    // TODO: set delivery port callback?

    // create producer instance
    log("Creating producer instance.");
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errorString);

    // delete config
    log("Deleting configuration.");
    delete conf;

    string message = "test";

    // produce
    log("Producing to topic.");
    RdKafka::ErrorCode errorCode = producer->produce(
                        /* Topic name */
                        topic,
                        /* Any Partition */
                        RdKafka::Topic::PARTITION_UA,
                        /* Make a copy of the value */
                        RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
                        /* Value */
                        const_cast<char*>(message.c_str()), message.size(),
                        /* Key */
                        NULL, 0,
                        /* Timestamp (defaults to current time) */
                        0,
                        /* Message headers, if any */
                        NULL,
                        /* Per-message opaque value passed to delivery report */
                        NULL);
        
    // error checking
    if (errorCode != RdKafka::ERR_NO_ERROR) {
        cout << "[ERROR]" << "Failed to produce to topic " << topic << "." << endl;
    }
    else {
        cout << "[SUCCESS] " << "Enqueued message (" << message.size() << "bytes) for topic " << topic << endl;
    }
    
    // flush
    log("Flushing final messages...");
    producer->flush(10*1000);

    // delete producer
    log("Deleting producer.");
    delete producer;

    log("Program finished executing.");
    return 0;
}