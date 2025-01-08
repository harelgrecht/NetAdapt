#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

class NetworkException : public std::exception {
protected:
    std::string message; // Holds the exception message
public:
    explicit NetworkException(const std::string& msg) : message(msg) {}

    // Override the what() method to return the exception message
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif