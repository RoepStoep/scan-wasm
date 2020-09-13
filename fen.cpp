
// includes

#include <string>

#include "bit.hpp"
#include "common.hpp"
#include "fen.hpp"
#include "libmy.hpp"
#include "pos.hpp"
#include "util.hpp"
#include "var.hpp"

// constants

static const std::string Sides  = "WB";
static const std::string Pieces = "wbWBe";

// prototypes

static Side parse_side   (Scanner_Number & scan);
static void parse_pieces (Bit piece[], Scanner_Number & scan);

static Pos pos_from_pieces (Side turn, Bit wm, Bit bm, Bit wk, Bit bk);

static std::string pos_pieces (const Pos & pos, Side sd);

static std::string run_string (Piece_Side ps, int from, int to);

static int find (char c, const std::string & s);

// functions

Pos pos_from_fen(const std::string & s) {

   Scanner_Number scan(s);

   Side turn = parse_side(scan);

   Bit piece_side[Side_Size][Piece_Size] {};

   while (!scan.eos()) {

      std::string token = scan.get_token();

      Side sd = parse_side(scan);
      parse_pieces(piece_side[sd], scan);
   }

   return pos_from_pieces(turn,
                          piece_side[White][Man],
                          piece_side[Black][Man],
                          piece_side[White][King],
                          piece_side[Black][King]);
}

static Side parse_side(Scanner_Number & scan) {

   std::string token = scan.get_token();

   if (token == "W") {
      return White;
   } else {
      return Black;
   } 
}

static void parse_pieces(Bit piece[], Scanner_Number & scan) {

   std::string token;

   while (true) {

      Piece pc = Man;

      token = scan.get_token();
      if (token == ",") token = scan.get_token();

      if (token.empty()) { // EOS
         return;
      } else if (token == ":") {
         scan.unget_char(); // HACK: no unget_token
         return;
      } else if (token == "K") {
         pc = King;
         token = scan.get_token();
      }

      int from = std::stoi(token);

      token = scan.get_token();

      if (token != "-") {

         if (token.size() == 1) scan.unget_char(); // HACK: no unget_token

         bit::set(piece[pc], square_from_std(from));

      } else {

         token = scan.get_token();

         int to = std::stoi(token);

         for (int sq = from; sq <= to; sq++) {
            bit::set(piece[pc], square_from_std(sq));
         }
      }
   }
}

Pos pos_from_hub(const std::string & s) {

   int i = 0;

   // turn

   Side turn = side_make(find(s[i++], Sides));

   // squares

   Bit piece_side[Piece_Side_Size + 1] {};

   for (Square sq : bit::Squares) {
      Piece_Side ps = Piece_Side(find(s[i++], Pieces));
      bit::set(piece_side[ps], sq);
   }

   // wrap up

   return pos_from_pieces(turn,
                          piece_side[White_Man],
                          piece_side[Black_Man],
                          piece_side[White_King],
                          piece_side[Black_King]);
}

static Pos pos_from_pieces(Side turn, Bit wm, Bit bm, Bit wk, Bit bk) {

   return Pos(turn, wm, bm, wk, bk);
}

std::string pos_fen(const Pos & pos) {

   std::string s;

   s += (pos.turn() == White) ? "W" : "B";
   s += ":W" + pos_pieces(pos, White);
   s += ":B" + pos_pieces(pos, Black);

   return s;
}

static std::string pos_pieces(const Pos & pos, Side sd) {

   std::string s;

   int prev = -1;
   int from = 0;
   int to   = 0;

   for (int sq = 1; sq <= Dense_Size; sq++) {

      Piece_Side ps = pos::piece_side(pos, square_from_std(sq));

      if (ps == prev) {

         to += 1;

      } else {

         if (prev >= 0) {
            if (!s.empty()) s += ",";
            s += run_string(piece_side_make(prev), from, to);
         }

         if (piece_side_is_side(ps, sd)) {
            prev = ps;
            from = sq;
            to   = sq;
         } else {
            prev = -1;
            from = 0;
            to   = 0;
         }
      }
   }

   if (prev >= 0) {
      if (!s.empty()) s += ",";
      s += run_string(piece_side_make(prev), from, to);
   }

   return s;
}

static std::string run_string(Piece_Side ps, int from, int to) {

   assert(ps != Empty);
   assert(1 <= from && from <= to && to <= Dense_Size);

   std::string s;

   if (piece_side_is_piece(ps, King)) s += "K";

   s += std::to_string(from);

   if (to == from) { // length 1
      // no-op
   } else if (to == from + 1) { // length 2
      s += ",";
      if (piece_side_is_piece(ps, King)) s += "K";
      s += std::to_string(to);
   } else { // length 3+
      s += "-";
      s += std::to_string(to);
   }

   return s;
}

std::string pos_hub(const Pos & pos) {

   std::string s;

   s += Sides[pos.turn()];

   for (Square sq : bit::Squares) {
      Piece_Side ps = pos::piece_side(pos, sq);
      s += Pieces[ps];
   }

   return s;
}

static int find(char c, const std::string & s) {

   auto i = s.find(c);
   return i;
}

