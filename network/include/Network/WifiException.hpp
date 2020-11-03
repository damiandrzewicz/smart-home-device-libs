#pragma once 

#include <exception>
#include <string>

class WiFiException : public std::exception
{
public:
    WiFiException(const std::string &msg)
    {
        this->msg = msg;
    }

	const char * what () const throw ()
    {
    	return msg.c_str();
    }

private:
    std::string msg;
};