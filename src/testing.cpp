#include <string>
#include <iostream>
#include <fstream>
#include "librdkafka/rdkafkacpp.h" // VSCode thinks this is a problem but as far as I can tell this is correct

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

void printErrorStringIfNotEmpty(string* errorString) {
    if (*errorString != "") {
        cout << "[ERROR] " << *errorString << endl;
        *errorString = "";
    }
}

RdKafka::Conf* loadConfigFromFile() {
    log("Loading configuration in from file.");

    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    
    ifstream configFile ("cc.config");
    
    if (!configFile.is_open()) {
        cout << "[ERROR] Couldn't open config file." << endl;
    }

    string errorString = "";

    string line;
    while (getline(configFile, line)) {
        if (line[0] == '#') {
            continue;
        }

        auto indexOfEqualsSign = line.find("=");
        auto key = line.substr(0, indexOfEqualsSign);
        auto value = line.substr(indexOfEqualsSign + 1);

        conf->set(key, value, errorString);
        printErrorStringIfNotEmpty(&errorString);
    }

    return conf;
}

int main() {
    log("Executing program.");d

    // get configuration
    RdKafka::Conf* conf = loadConfigFromFile();

    // create producer instance
    log("Creating producer instance.");
    string errorString = "";
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errorString);
    printErrorStringIfNotEmpty(&errorString);

    // delete config
    log("Deleting configuration.");
    delete conf;

    // produce
    log("Producing to topic.");
    string topic = getEnvironmentVariable("TOPIC");
    string message = getEnvironmentVariable("MESSAGE");
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
    log("Flushing...");
    producer->flush(10*1000);

    // delete producer
    log("Deleting producer.");
    delete producer;

    log("Program finished executing.");
    return 0;
}