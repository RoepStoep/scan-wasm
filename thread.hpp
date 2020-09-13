
#ifndef THREAD_HPP
#define THREAD_HPP

// includes

#include <string>

#include "libmy.hpp"

// functions

bool has_input ();
void put_line  (const std::string & line);
bool peek_line (std::string & line);
bool get_line  (std::string & line); // only call if input is available #

#endif // !defined THREAD_HPP