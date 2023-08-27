
// includes

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "bb_base.hpp"
#include "bb_comp.hpp"
#include "bb_index.hpp"
#include "bit.hpp"
#include "common.hpp"
#include "eval.hpp"
#include "fen.hpp"
#include "game.hpp"
#include "gen.hpp"
#include "hash.hpp"
#include "hub.hpp"
#include "libmy.hpp"
#include "list.hpp"
#include "move.hpp"
#include "pos.hpp"
#include "search.hpp"
#include "sort.hpp"
#include "thread.hpp"
#include "tt.hpp"
#include "util.hpp"
#include "var.hpp"

// variables

static Game game;
static Search_Input si;
static bool in_search;

// prototypes

static void hub_loop ();

static void init_low ();

static void param_bool (const std::string & name);
static void param_int  (const std::string & name, int min, int max);
static void param_enum (const std::string & name, const std::string & values);

// functions

int main(int argc, char * argv[]) {

   // init

   bit::init();
   hash::init();
   pos::init();
   var::init();

   bb::index_init();
   bb::comp_init();

   ml::rand_init(); // after hash keys

   in_search = false;
}

extern "C" void scan_command(const char *c_cmd) {

   std::string line(c_cmd);
   put_line(line);

   hub_loop();
}

static void hub_loop() {

   while (!in_search && has_input()) { // WAS while (true) #

      std::string line = hub::read();
      hub::Scanner scan(line);

      if (scan.eos()) { // empty line
         hub::error("missing command");
         continue;
      }

      std::string command = scan.get_command();

      if (false) {

      } else if (command == "go") {

         bool think = false; // ignored
         bool ponder = false;
         bool analyze = false;

         while (!scan.eos()) {

            auto p = scan.get_pair();

            if (false) {
            } else if (p.first == "think") {
               think = true;
            } else if (p.first == "ponder") {
               ponder = true;
            } else if (p.first == "analyze") {
               analyze = true;
            }
         }

         si.move = !analyze;
         si.input = true;
         si.ponder = ponder;

         in_search = true;
         search(game.node(), si);

      } else if (command == "hub") {

         std::string line = "id";
         hub::add_pair(line, "name", Engine_Name);
         hub::add_pair(line, "version", Engine_Version);
         hub::add_pair(line, "author", "Fabien Letouzey");
         hub::add_pair(line, "country", "France");
         hub::write(line);

         param_enum("variant", "normal killer bt frisian losing");
         param_enum("eval", "pst pattern");
         param_bool("ponder");
         param_int ("tt-size", 16, 30);
         param_int ("bb-size", 0, 7);

         hub::write("wait");

      } else if (command == "init") {

         init_low();
         hub::write("ready");

      } else if (command == "level") {

         int depth = -1;
         double move_time = -1.0;

         bool smart = false;
         int moves = 0;
         double game_time = 30.0;
         double inc = 0.0;

         bool infinite = false; // ignored
         bool ponder = false; // ignored

         while (!scan.eos()) {

            auto p = scan.get_pair();

            if (false) {
            } else if (p.first == "depth") {
               depth = std::stoi(p.second);
            } else if (p.first == "move-time") {
               move_time = std::stod(p.second);
            } else if (p.first == "moves") {
               smart = true;
               moves = std::stoi(p.second);
            } else if (p.first == "time") {
               smart = true;
               game_time = std::stod(p.second);
            } else if (p.first == "inc") {
               smart = true;
               inc = std::stod(p.second);
            } else if (p.first == "infinite") {
               infinite = true;
            } else if (p.first == "ponder") {
               ponder = true;
            }
         }

         if (depth >= 0) si.depth = Depth(depth);
         if (move_time >= 0.0) si.time = move_time;

         if (smart) si.set_time(moves, game_time, inc);

      } else if (command == "new-game") {

         G_TT.clear();

      } else if (command == "ping") {

         hub::write("pong");

      } else if (command == "ponder-hit") {

         // no-op (handled during search)

      } else if (command == "pos") {

         std::string pos = pos_hub(pos::Start);
         std::string moves;
         std::string wolf;

         while (!scan.eos()) {

            auto p = scan.get_pair();

            if (false) {
            } else if (p.first == "start") {
               pos = pos_hub(pos::Start);
            } else if (p.first == "pos") {
               pos = p.second;
            } else if (p.first == "moves") {
               moves = p.second;
            } else if (p.first == "wolf") {
               wolf = p.second;
            }
         }

         // position

         Pos hub_pos = pos_from_hub(pos);

         if (var::Variant == var::Frisian) {
            pos::wolf_from_hub(wolf, hub_pos);
         }

         game.init(hub_pos);

         // moves

         std::stringstream ss(moves);

         std::string arg;

         while (ss >> arg) {

            Move mv = move::from_hub(arg, game.pos());

            if (!move::is_legal(mv, game.pos())) {
               hub::error("illegal move");
               break;
            } else {
               game.add_move(mv);
            }

         }

         si.init(); // reset level

      } else if (command == "quit") {

         std::exit(EXIT_SUCCESS);

      } else if (command == "set-param") {

         std::string name;
         std::string value;

         while (!scan.eos()) {

            auto p = scan.get_pair();

            if (false) {
            } else if (p.first == "name") {
               name = p.second;
            } else if (p.first == "value") {
               value = p.second;
            }
         }

         if (name.empty()) {
            hub::error("missing name");
            continue;
         }

         var::set(name, value);
         var::update();

      } else if (command == "stop") {

         // no-op (handled during search)

      } else { // unknown command

         hub::error("bad command");
         continue;
      }
   }
}

void after_search(Search_Output & so) {

   Move move = so.move;
   Move answer = so.answer;

   if (move == move::None) move = quick_move(game.node());

   if (move != move::None && answer == move::None) {
      answer = quick_move(game.node().succ(move));
   }

   Pos p0 = game.pos();
   Pos p1 = p0;
   if (move != move::None) p1 = p0.succ(move);

   std::string line = "done";
   if (move   != move::None) hub::add_pair(line, "move",   move::to_hub(move, p0));
   if (answer != move::None) hub::add_pair(line, "ponder", move::to_hub(answer, p1));
   hub::write(line);

   si.init(); // reset level

   in_search = false;
   hub_loop();
}

static void init_low() {

   bit::init(); // depends on the variant
   if (var::BB) bb::init();

   eval_init();
   G_TT.set_size(var::TT_Size);
}

static void param_bool(const std::string & name) {

   std::string line = "param";
   hub::add_pair(line, "name", name);
   hub::add_pair(line, "value", var::get(name));
   hub::add_pair(line, "type", "bool");
   hub::write(line);
}

static void param_int(const std::string & name, int min, int max) {

   std::string line = "param";
   hub::add_pair(line, "name", name);
   hub::add_pair(line, "value", var::get(name));
   hub::add_pair(line, "type", "int");
   hub::add_pair(line, "min", std::to_string(min));
   hub::add_pair(line, "max", std::to_string(max));
   hub::write(line);
}

static void param_enum(const std::string & name, const std::string & values) {

   std::string line = "param";
   hub::add_pair(line, "name", name);
   hub::add_pair(line, "value", var::get(name));
   hub::add_pair(line, "type", "enum");
   hub::add_pair(line, "values", values);
   hub::write(line);
}

