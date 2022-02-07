#include <string>
#include <iostream>
#include "librdkafka/rdkafkacpp.h"

using namespace std;

int main() {
    cout << "Executing program." << endl;
    
    // prepare variables
    string brokers = "pkc-43n10.us-central1.gcp.confluent.cloud:9092";
    string topic = "topic.Asn1DecoderInput";
    const char* username = ""; // SET
    const char* password = ""; // SET
    string errorString = "";

    // create configuration
    cout << "Creating configuration" << endl;
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    // set up confluent cloud config
    cout << "Setting up Confluent Cloud configuration key/value pairs." << endl;
    conf->set("bootstrap.servers", brokers, errorString);
    conf->set("ssl.endpoint.identification.algorithm", "https", errorString);
    conf->set("security.protocol", "SASL_SSL", errorString); // SSL_SASL or SSL_PLAINTEXT
    conf->set("sasl.mechanisms", "PLAIN", errorString);
    conf->set("sasl.username", username, errorString);
    conf->set("sasl.password", password, errorString);

    conf->set("api.version.request", "true", errorString);

    cout << "Finished setting up Confluent Cloud configuration key/value pairs." << endl;

    // TODO: set delivery port callback?

    // create producer instance
    cout << "Creating producer instance." << endl;
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errorString);

    // delete config
    cout << "Deleting configuration." << endl;
    delete conf;

    string message = "test";

    // produce
    cout << "Producing." << endl;
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
        cout << "Failed to produce to topic " << topic << "." << endl;
    }
    else {
        cout << "Enqueued message (" << message.size() << "bytes) for topic " << topic << endl;
    }
    
    // flush
    cout << "Flushing final messages..." << endl;
    producer->flush(10*1000);

    // delete producer
    cout << "Deleting producer." << endl;
    delete producer;

    cout << "Program finished executing." << endl;
    return 0;
}