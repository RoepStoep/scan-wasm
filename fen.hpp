
#ifndef FEN_HPP
#define FEN_HPP

// includes

#include <string>

#include "common.hpp"
#include "libmy.hpp"

class Pos;

// functions

Pos pos_from_fen (const std::string & s); // REMOVE ME
Pos pos_from_hub (const std::string & s);

std::string pos_fen (const Pos & pos); // REMOVE ME
std::string pos_hub (const Pos & pos);

#endif // !defined FEN_HPP

