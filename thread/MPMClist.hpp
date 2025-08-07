#ifndef MPMCLIST_HPP
#define MPMCLIST_HPP
#include <atomic>
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>

class MPMClist {
public:
  using Task = std::function<void()>;
  MPMClist() {
    Node *dummy = new Node();
    tail.store(dummy);
    head.store(dummy);
  }
  ~MPMClist() {
    while (pop())
      ;
    delete (head.load());
  }
  // Deleting the copy constructor and assignment operator
  MPMClist(const MPMClist &) = delete;
  MPMClist &operator=(const MPMClist &) = delete;

  void push(Task tsk) {
    Node *new_node = new Node(std::move(tsk));
    Node *old_tail;

    while (true) {
      old_tail = tail.load(std::memory_order_acquire);
      Node *next = old_tail->next.load(std::memory_order_acquire);
      if (next == nullptr) {
        if (old_tail->next.compare_exchange_weak(next, new_node)) {
          break;
        }
      } else {
        tail.compare_exchange_weak(old_tail, next);
      }
    }
    assert(tail.compare_exchange_weak(old_tail, new_node));
  }

  std::unique_ptr<Task> pop() {
    Node *old_head;
    Node *next;
    while (true) {
      old_head = head.load(std::memory_order_acquire);
      next = old_head->next.load(std::memory_order_acquire);
      if (next == nullptr) {
        return nullptr;
      }
      if (head.compare_exchange_strong(old_head, next)) {
        std::unique_ptr<Task> tsk =
            std::make_unique<Task>(std::move(next->tsk_));
        delete old_head;
        return tsk;
      }
    }
  }

  bool empty() const {
    Node *head_ptr = head.load(std::memory_order_acquire);
    Node *next_ptr = head_ptr->next.load(std::memory_order_acquire);
    return next_ptr == nullptr;
  }

private:
  struct Node {
    Task tsk_;
    std::atomic<Node *> next;

    Node() : next(nullptr) {}
    Node(Task &&t) : tsk_(std::move(t)), next(nullptr) {}
  };

  std::atomic<Node *> tail;
  std::atomic<Node *> head;
};
#endif // MPMCLIST_HPP