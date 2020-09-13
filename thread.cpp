
// includes

#include <deque>
#include <iostream>
#include <string>

#include "libmy.hpp"
#include "thread.hpp"
#include "main.hpp"

// types

class Input {

private:

   std::deque<std::string> m_deque;

public:

   bool peek_line (std::string & line);
   bool get_line  (std::string & line);

   void put_line (const std::string & line);

   bool has_input () const { return !m_deque.empty(); }
};

// variables

Input G_Input;

// functions

bool has_input() {
   return G_Input.has_input();
}

void put_line(const std::string & line) {
   G_Input.put_line(line);
}

bool peek_line(std::string & line) {
   return G_Input.peek_line(line);
}

bool get_line(std::string & line) {
   return G_Input.get_line(line);
}

bool Input::peek_line(std::string & line) {

   assert(!m_deque.empty());
   line = m_deque.front();

   return true;
}

bool Input::get_line(std::string & line) {

   assert(!m_deque.empty());
   line = m_deque.front();

   m_deque.pop_front();

   return true;
}

void Input::put_line(const std::string & line) {
   m_deque.push_back(line);
}