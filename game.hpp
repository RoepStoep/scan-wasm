
#ifndef GAME_HPP
#define GAME_HPP

// includes

#include <string>

#include "common.hpp"
#include "libmy.hpp"
#include "pos.hpp"

// types

class Game {

private:

   static const int Size {1024};

   Pos m_pos_start;

   ml::Array<Node, Size> m_node;
   ml::Array<Move, Size> m_move;

public:

   Game () { clear(); }

   void clear    () { init(pos::Start); }
   void init     (const Pos & pos);
   void add_move (Move mv);

   Side turn   () const { return pos().turn(); }
   bool is_end () const { return node().is_end(); }

   int  size        ()      const { return m_move.size(); }
   Move move        (int i) const { return m_move[i]; }
   Move operator [] (int i) const { return m_move[i]; }

   Pos          start_pos () const { return m_pos_start; }
   Pos          pos       () const { return Pos(node()); }
   const Node & node      () const { return m_node[m_node.size() - 1]; }
};

#endif // !defined GAME_HPP

