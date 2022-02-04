#include <string>
#include <iostream>
#include "rdkafkacpp.h"

using namespace std;

int main() {
    log("Executing program.");
    
    string brokers = "pkc-43n10.us-central1.gcp.confluent.cloud:9092";
    string topic = "topic.Asn1DecoderInput";

    log("Creating configuration");
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    string errorString;

    conf->set("bootstrap.servers", brokers, errorString);

    cout << "Setting up Confluent Cloud configuration key/value pairs." << std::endl; // DEBUG

    string username = "K4O367P4UIH3ZPJ7";
    string password = "QSWUoQAMxfFZqMjgqEmk18qcL6NMiLjF83X+7+8ScWrT8lzh7DqCMiDeSnnsWqOF";

    conf->set("security.protocol", "SASL_SSL", errorString);
    conf->set("sasl.mechanisms", "PLAIN", errorString);
    conf->set("sasl.username", username.c_str(), errorString);
    conf->set("sasl.password", password.c_str(), errorString);
    conf->set("session.timeout.ms", "45000", errorString);

    std::cout << "Finished setting up Confluent Cloud configuration key/value pairs." << std::endl;

    // TODO: set delivery port callback?

    // create producer instance
    log("Creating producer instance.");
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errorString);

    log("Deleting configuration.");
    delete conf;

    string message = "test";

    log("Producing.");

    retry:
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
        
        if (errorCode != RdKafka::ERR_NO_ERROR) {
            cout << "Failed to produce to topic " << topic << "." << endl;

            if (errorCode == RdKafka::ERR__QUEUE_FULL) {
                // if internal queue is full, wait for messages to be delivered and then retry
                producer->poll(1000);
                goto retry;
            }
        }
        else {
            cout << "Enqueued message (" << message.size() << "bytes) for topic " << topic << endl;
        }
    
    cout << "Flushing final messages..." << std::endl;
    producer->flush(10*1000);

    log("Deleting producer.");
    delete producer;

    log("Program finished executing.");
}

void log(string message) {
    cout << message << endl;
}