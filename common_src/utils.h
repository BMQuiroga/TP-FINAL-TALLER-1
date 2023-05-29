#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <string>
#include <vector>

// Split the string s by the delimiter string delim, and return a vector with the results
std::vector<std::string> strsplit(const std::string &s, const std::string &delim);
// Get a random UUID
std::string get_uuid();

#endif