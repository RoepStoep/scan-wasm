
// includes

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "common.hpp"
#include "libmy.hpp"
#include "var.hpp"

namespace var {

// variables

Variant_Type Variant;
Eval_Type Eval;
bool Ponder;
int  TT_Size;
bool BB;
int  BB_Size;

static std::map<std::string, std::string> Var;

// prototypes

static bool get_bool (const std::string & name);
static int  get_int  (const std::string & name);

// functions

void init() {

   set("variant", "normal");
   set("eval", "pattern");
   set("ponder", "false");
   set("tt-size", "22");
   set("bb-size", "0");

   update();
}

void load(const std::string & file_name) {

   std::ifstream file(file_name.c_str());

   if (!file) {
      std::cerr << "unable to open file \"" << file_name << "\"" << std::endl;
      std::exit(EXIT_FAILURE);
   }

   while (true) {

      std::string name;
      file >> name;
      if (file.eof()) break;

      if (name == "#") { // comment

         std::string comment;
         std::getline(file, comment);

         if (file.eof()) {
            std::cerr << "invalid INI file" << std::endl;
            std::exit(EXIT_FAILURE);
         }

         file >> name;
         if (file.eof()) break;
      }

      std::string sep;
      file >> sep;
      if (file.eof() || sep != "=") {
         std::cerr << "invalid INI file" << std::endl;
         std::exit(EXIT_FAILURE);
      }

      std::string value;
      file >> value;
      if (file.eof()) {
         std::cerr << "invalid INI file" << std::endl;
         std::exit(EXIT_FAILURE);
      }

      set(name, value);
   }

   update();
}

void update() {

   std::string variant = get("variant");

   if (false) {
   } else if (variant == "normal") {
      Variant = Normal;
   } else if (variant == "killer") {
      Variant = Killer;
   } else if (variant == "breakthrough" || variant == "bt") {
      Variant = BT;
   } else if (variant == "frisian") {
      Variant = Frisian;
   } else if (variant == "losing") {
      Variant = Losing;
   } else {
      std::cerr << "error: variant = \"" << variant << "\"" << std::endl;
      std::exit(EXIT_FAILURE);
   }

   std::string eval = get("eval");

   if (false) {
   } else if (eval == "pst") {
      Eval = PST;
   } else if (eval == "pattern") {
      Eval = Pattern;
   } else {
      std::cerr << "error: eval = \"" << eval << "\"" << std::endl;
      std::exit(EXIT_FAILURE);
   }

   Ponder      = get_bool("ponder");
   TT_Size     = 1 << get_int("tt-size");
   BB_Size     = get_int("bb-size");
   BB          = BB_Size > 0;
}

std::string get(const std::string & name) {

   if (Var.find(name) == Var.end()) {
      std::cerr << "unknown variable: \"" << name << "\"" << std::endl;
      std::exit(EXIT_FAILURE);
   }

   return Var[name];
}

void set(const std::string & name, const std::string & value) {
   Var[name] = value;
}

static bool get_bool(const std::string & name) {

   std::string value = get(name);

   if (value == "true") {
      return true;
   } else if (value == "false") {
      return false;
   } else {
      std::cerr << "not a boolean: variable " << name << " = \"" << value << "\"" << std::endl;
      std::exit(EXIT_FAILURE);
      return false;
   }
}

static int get_int(const std::string & name) {
   return std::stoi(get(name));
}

std::string variant_name() {

   switch (Variant) {
      case Normal :  return "";
      case Killer :  return "_killer";
      case BT :      return "_bt";
      case Frisian : return "_frisian";
      case Losing :  return "_losing";
   }
}

} // namespace var

