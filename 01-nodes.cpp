#include <iostream>

#include "ticket.hpp"

int main() {
  Node third{{103, "Pago duplicado"}};
  Node second{{102, "Cambiar foto de perfil"}, &third};
  Node first{{101, "Error al iniciar sesión"}, &second};
  const Node* head = &first;

  for (const Node* current = head; current != nullptr; current = current->next) {
    std::cout << current->ticket.id << " -> ";
  }
  std::cout << "nullptr\n";
}
