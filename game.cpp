
// includes

#include "common.hpp"
#include "game.hpp"
#include "libmy.hpp"
#include "pos.hpp"

// functions

void Game::init(const Pos & pos) {

   m_pos_start = pos;

   m_move.clear();
   m_node.clear();
   m_node.add(Node(m_pos_start));
}

void Game::add_move(Move mv) {
   m_move.add(mv);
   m_node.add(node().succ(mv));
}

