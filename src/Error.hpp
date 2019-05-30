#ifndef ORNG_ERROR_H
#define ORNG_ERROR_H

#include <exception>
#include <string>

struct OrngError : public std::exception {
    virtual std::string caused_by() = 0;
};

#endif