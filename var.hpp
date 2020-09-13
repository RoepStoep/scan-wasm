
#ifndef VAR_HPP
#define VAR_HPP

// includes

#include <string>

#include "libmy.hpp"

namespace var {

// types

enum Variant_Type { Normal, Killer, BT, Frisian, Losing };
enum Eval_Type { PST, Pattern };

// variables

extern Variant_Type Variant;
extern Eval_Type Eval;
extern bool Ponder;
extern int  TT_Size;
extern bool BB;
extern int  BB_Size;

// functions

void init   ();
void load   (const std::string & file_name);
void update ();

std::string get (const std::string & name);
void        set (const std::string & name, const std::string & value);

std::string variant_name ();

} // namespace var

#endif // !defined VAR_HPP

