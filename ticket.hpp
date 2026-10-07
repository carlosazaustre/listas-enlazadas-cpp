#pragma once

#include <string>

struct Ticket {
  int id;
  std::string title;
};

struct Node {
  Ticket ticket;
  Node* next = nullptr;
};
