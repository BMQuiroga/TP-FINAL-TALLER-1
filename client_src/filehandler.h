#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>

class FileHandler {
private:
    const std::string commands_file_name;
    std::string command_line;
    /**
     * Devuelve true si el comando es válido. 
     * Devuelve false en caso contrario.
    */
    bool is_valid_command();
    /**
     * Devuelve true si todavía hay un comando válido en el archivo. 
     * Devuelve false en caso contrario.
    */
    bool get_valid_command(std::ifstream &commands_file);
    
public:
    explicit FileHandler(
        const std::string& commands_file_name,
        const std::string& command_line = "");
    /**
     * Devuelve true si se llegó al final del archivo.
     * Devuelve false en caso contrario.
    */
    bool file_is_finished(std::ifstream &commands_file);
    /**
     * Devuelve el nombre del archivo.
    */
    std::string get_file_name();
    /**
     * Devuelve el próximo comando a ejecutar que es válido
    */
    std::tuple<std::string, std::vector<int>> get_next_command(
        std::ifstream &commands_file);
};
