#include <cassert>
#include <iostream>

#include "ticket.hpp"

int main() {
  Node last{{103, "Pago duplicado"}};
  Node first{{101, "Error al iniciar sesión"}, &last};
  Node inserted{{102, "Cambiar foto de perfil"}};

  // Caso incorrecto controlado: no recorrer esta cadena con un while.
  first.next = &inserted;
  inserted.next = first.next;
  assert(inserted.next == &inserted);
  std::cout << "Orden incorrecto: 101 -> 102 -> 102 (ciclo)\n";

  first.next = &last;
  inserted.next = nullptr;

  inserted.next = first.next;
  first.next = &inserted;

  assert(first.next == &inserted && inserted.next == &last);
  std::cout << "Orden correcto: ";
  for (const Node* current = &first; current != nullptr; current = current->next) {
    std::cout << current->ticket.id << " -> ";
  }
  std::cout << "nullptr\n";
}
