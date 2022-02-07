#include <string>
#include <iostream>
#include <fstream>
#include "librdkafka/rdkafkacpp.h"

using namespace std;

void log(string message) {
    cout << "[LOG] " << message << endl;
}

const char* getEnvironmentVariable(const char* variableName) {
    const char* toReturn = getenv(variableName);
    if (!toReturn) {
        cout << "[ERROR] Something went wrong attempting to retrieve the environment variable " << variableName << endl;
    }
    return toReturn;
}

void printErrorStringIfNotEmpty(string errorString) {
    if (errorString != "") {
        cout << "[ERROR] " << errorString << endl;
    }
}

RdKafka::Conf* getConfig() {
    string confType = "file"; // hardcode or file
    cout << "confType: " << confType << endl;

    string errorString = "";

    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    if (confType == "file") {
        log("Loading configuration in from file.");
        
        ifstream configFile ("cc.config");
        if (configFile.is_open()) {
            string line;
            while (getline(configFile, line)) {
                if (line[0] == '#') {
                    continue;
                }

                auto indexOfEqualsSign = line.find("=");
                auto key = line.substr(0, indexOfEqualsSign);
                auto value = line.substr(indexOfEqualsSign + 1);

                conf->set(key, value, errorString);
                printErrorStringIfNotEmpty(errorString);
            }
        }
        else {
            cout << "[ERROR] Couldn't open config file." << endl;
        }
    }
    else if (confType == "hardcode") {
        log("Creating configuration manually.");

        string brokers = getEnvironmentVariable("CONFLUENT_BROKERS");
        const char* username = getEnvironmentVariable("CONFLUENT_KEY");
        const char* password = getEnvironmentVariable("CONFLUENT_SECRET");

        log("Setting up Confluent Cloud configuration key/value pairs.");

        conf->set("bootstrap.servers", brokers, errorString);
        printErrorStringIfNotEmpty(errorString);

        conf->set("security.protocol", "SASL_SSL", errorString); // SASL_SSL or SSL_PLAINTEXT
        printErrorStringIfNotEmpty(errorString);

        conf->set("sasl.mechanisms", "PLAIN", errorString);
        printErrorStringIfNotEmpty(errorString);

        conf->set("sasl.username", username, errorString);
        printErrorStringIfNotEmpty(errorString);

        conf->set("sasl.password", password, errorString);
        printErrorStringIfNotEmpty(errorString);

        log("Finished setting up Confluent Cloud configuration key/value pairs.");
    }
    return conf;
}

int main() {
    log("Executing program.");

    string topic = "topic.Asn1DecoderInput";
    string message = "test";
    
    string errorString = "";

    // get configuration
    RdKafka::Conf* conf = getConfig();

    // TODO: set delivery port callback?

    // create producer instance
    log("Creating producer instance.");
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errorString);
    printErrorStringIfNotEmpty(errorString);

    // delete config
    log("Deleting configuration.");
    delete conf;

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