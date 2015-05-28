#include <fstream>
#include <ctime>

template <typename message>
void Basic_Logger::log_helper(message msg){
    if(output != nullptr)
        *output << msg;
    else{
        std::ofstream ofile(filepath, flags);
        if(!ofile.is_open()) return;
        ofile << msg;
        ofile.close();
    }
}
template <typename message0, typename... messages>
void Basic_Logger::log_helper(
    message0 first_message,
    messages... message_pack
){
    Basic_Logger::log_helper(first_message);
    Basic_Logger::log_helper(message_pack...);
}

template <typename... messages>
void Basic_Logger::log(messages... msgs){
    time_t raw;
    time(&raw);
    auto localt(localtime(&raw));
    const size_t buffer_size(100);
    char buffer[buffer_size]{};
    strftime(buffer, buffer_size, "[%c] ", localt);
    
    Basic_Logger::log_helper(std::string(buffer), msgs...);
    *output << std::endl;
}


std::ostream* Basic_Logger::output(nullptr);
std::string Basic_Logger::filepath("debug.log");
std::ios_base::openmode Basic_Logger::flags(std::ios_base::app);