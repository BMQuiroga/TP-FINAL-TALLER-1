#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include "client_protocol.h"
#include "filehandler.h"

class Client {
private:
    const std::string hostname;
    const std::string servname;
    ClientProtocol protocol;
    FileHandler file_handler;

public:
    explicit Client(
        const std::string& hostname,
        const std::string& servname,
        const std::string& commands_file_name);
    void start_communication();
};
