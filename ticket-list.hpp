#pragma once

#include <utility>

#include "ticket.hpp"

// Contenedor didáctico propietario de los nodos; los Node* devueltos son observadores.
class TicketList {
 public:
  TicketList() = default;
  ~TicketList() { clear(); }

  // Una copia superficial duplicaría la propiedad de los mismos nodos.
  TicketList(const TicketList&) = delete;
  TicketList& operator=(const TicketList&) = delete;
  TicketList(TicketList&&) = delete;
  TicketList& operator=(TicketList&&) = delete;

  [[nodiscard]] const Node* head() const noexcept { return head_; }
  [[nodiscard]] bool empty() const noexcept { return head_ == nullptr; }

  void pushFront(Ticket ticket) {
    Node* inserted = new Node{std::move(ticket), head_};
    head_ = inserted;
  }

  [[nodiscard]] Node* find(int id) noexcept {
    Node* current = head_;
    while (current != nullptr && current->ticket.id != id) {
      current = current->next;
    }
    return current;
  }

  // Precondición: previous es un nodo vivo de ESTA lista. No lo buscamos de nuevo.
  void insertAfter(Node& previous, Ticket ticket) {
    Node* inserted = new Node{std::move(ticket), nullptr};
    inserted->next = previous.next;
    previous.next = inserted;
  }

  // Misma precondición que insertAfter; false si no hay sucesor.
  bool eraseAfter(Node& previous) noexcept {
    Node* removed = previous.next;
    if (removed == nullptr) {
      return false;
    }
    previous.next = removed->next;
    delete removed;
    return true;
  }

  bool popFront() noexcept {
    if (head_ == nullptr) {
      return false;
    }
    Node* removed = head_;
    head_ = removed->next;
    delete removed;
    return true;
  }

  void clear() noexcept {
    // Iterativo: la profundidad de la pila de llamadas no crece con la lista.
    while (popFront()) {
    }
  }

 private:
  Node* head_ = nullptr;
};
