#include <string>
#include <iostream>
#include "../librdkafka/src-cpp/rdkafkacpp.h"

using namespace std;

int main() {
    cout << "Executing program." << endl;
    
    string brokers = "pkc-43n10.us-central1.gcp.confluent.cloud:9092";
    string topic = "topic.Asn1DecoderInput";

    cout << "Creating configuration" << endl;
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
    cout << "Creating producer instance." << endl;
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errorString);

    cout << "Deleting configuration." << endl;
    delete conf;

    string message = "test";

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
        
    if (errorCode != RdKafka::ERR_NO_ERROR) {
        cout << "Failed to produce to topic " << topic << "." << endl;
    }
    else {
        cout << "Enqueued message (" << message.size() << "bytes) for topic " << topic << endl;
    }
    
    cout << "Flushing final messages..." << std::endl;
    producer->flush(10*1000);

    cout << "Deleting producer." << endl;
    delete producer;

    cout << "Program finished executing." << endl;
    return 0;
}