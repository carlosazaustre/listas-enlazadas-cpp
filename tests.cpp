#include <cassert>
#include <initializer_list>
#include <iostream>
#include <type_traits>

#include "ticket-list.hpp"

void expectIds(const TicketList& tickets, std::initializer_list<int> expected) {
  const Node* current = tickets.head();
  for (const int id : expected) {
    assert(current != nullptr);
    assert(current->ticket.id == id);
    current = current->next;
  }
  assert(current == nullptr);
}

int main() {
  static_assert(!std::is_copy_constructible_v<TicketList>);
  static_assert(!std::is_copy_assignable_v<TicketList>);
  static_assert(!std::is_move_constructible_v<TicketList>);
  static_assert(!std::is_move_assignable_v<TicketList>);

  TicketList tickets;
  assert(tickets.empty());
  assert(tickets.find(999) == nullptr);
  assert(!tickets.popFront());
  tickets.clear();
  expectIds(tickets, {});

  tickets.pushFront({103, "Pago duplicado"});
  Node* last = tickets.find(103);
  assert(last != nullptr);
  assert(!tickets.eraseAfter(*last));
  tickets.pushFront({101, "Error al iniciar sesión"});
  Node* first = tickets.find(101);
  assert(first != nullptr);
  tickets.insertAfter(*first, {102, "Cambiar foto de perfil"});
  expectIds(tickets, {101, 102, 103});
  assert(tickets.find(103) == last);
  assert(tickets.find(101) == first);
  assert(tickets.find(999) == nullptr);

  tickets.insertAfter(*last, {104, "Exportar facturas"});
  expectIds(tickets, {101, 102, 103, 104});
  assert(tickets.eraseAfter(*first));
  expectIds(tickets, {101, 103, 104});
  assert(tickets.eraseAfter(*last));
  expectIds(tickets, {101, 103});
  assert(!tickets.eraseAfter(*last));
  assert(tickets.popFront());
  expectIds(tickets, {103});
  assert(tickets.popFront());
  assert(tickets.empty());
  assert(!tickets.popFront());

  tickets.pushFront({101, "Reutilizar lista vacía"});
  expectIds(tickets, {101});
  tickets.clear();
  tickets.clear();
  expectIds(tickets, {});

  {
    TicketList longList;
    for (int id = 0; id < 100000; ++id) {
      longList.pushFront({id, "Destrucción iterativa"});
    }
  }
  std::cout << "OK: vacía, singleton, inicio, medio, final, búsqueda, estabilidad y destrucción\n";
}
