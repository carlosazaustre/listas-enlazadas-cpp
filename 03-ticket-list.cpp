#include <iostream>

#include "ticket-list.hpp"

void print(const TicketList& tickets) {
  for (const Node* current = tickets.head(); current != nullptr; current = current->next) {
    std::cout << current->ticket.id << " -> ";
  }
  std::cout << "nullptr\n";
}

int main() {
  TicketList tickets;
  tickets.pushFront({103, "Pago duplicado"});
  tickets.pushFront({101, "Error al iniciar sesión"});
  print(tickets);

  Node* previous = tickets.find(101);
  if (previous != nullptr) {
    tickets.insertAfter(*previous, {102, "Cambiar foto de perfil"});
    print(tickets);

    tickets.eraseAfter(*previous);
    print(tickets);
  }

  tickets.popFront();
  print(tickets);
  tickets.clear();
  print(tickets);
  std::cout << "Buscar 999: " << (tickets.find(999) == nullptr ? "no encontrado" : "encontrado") << '\n';
}
