#include <cassert>
#include <cstddef>
#include <iostream>

#include "ticket-list.hpp"

int main() {
  std::cout << "n,comparaciones_al_buscar_el_ultimo\n";
  for (const int count : {4, 8, 16}) {
    TicketList tickets;
    for (int id = count; id >= 1; --id) {
      tickets.pushFront({id, "Ticket"});
    }

    std::size_t comparisons = 0;
    const Node* found = nullptr;
    for (const Node* current = tickets.head(); current != nullptr; current = current->next) {
      ++comparisons;
      if (current->ticket.id == count) {
        found = current;
        break;
      }
    }
    assert(found != nullptr);
    assert(comparisons == static_cast<std::size_t>(count));
    std::cout << count << ',' << comparisons << '\n';
  }
}
