#include <algorithm>
#include <forward_list>
#include <iostream>
#include <list>

#include "ticket.hpp"

template <typename Range>
void print(const Range& tickets) {
  for (const Ticket& ticket : tickets) {
    std::cout << ticket.id << " -> ";
  }
  std::cout << "fin\n";
}

int main() {
  std::forward_list<Ticket> singly{
      {101, "Error al iniciar sesión"}, {103, "Pago duplicado"}};
  auto previous = std::find_if(singly.begin(), singly.end(),
                               [](const Ticket& ticket) { return ticket.id == 101; });
  if (previous != singly.end()) {
    singly.insert_after(previous, {102, "Cambiar foto de perfil"});
    print(singly);
    // Acabamos de insertar un sucesor: erase_after tiene una posición válida.
    singly.erase_after(previous);
    print(singly);
  }
  singly.insert_after(singly.before_begin(), {100, "Revisar documentación"});
  print(singly);

  std::list<Ticket> doubly{
      {101, "Error al iniciar sesión"}, {103, "Pago duplicado"}};
  auto position = std::find_if(doubly.begin(), doubly.end(),
                               [](const Ticket& ticket) { return ticket.id == 103; });
  if (position != doubly.end()) {
    auto inserted = doubly.insert(position, {102, "Cambiar foto de perfil"});
    print(doubly);
    doubly.erase(inserted);
    print(doubly);
  }
}
