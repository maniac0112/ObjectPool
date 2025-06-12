# ⚙️ C++ Object Pool Benchmark

This project implements a **custom Object Pool** in C++ to manage memory more efficiently for frequently created and destroyed objects. It compares the performance of object pooling versus standard dynamic allocation using `new`/`delete`.

---

## 📦 Features

- Lightweight, generic `ObjectPool<T>` implementation
- Manual memory management using `malloc`, `placement new`, and `explicit destructor calls`
- RAII support via `std::shared_ptr` with custom deleter
- Comparison between:
  - Object pool allocation
  - Standard `new` / `delete` allocation
- High-precision timing support via `scoped_timer.cpp` [Still in Progress]

---

## 🧠 Concepts Used

- Object Pool pattern
- Smart pointers with custom deleters
- Placement new & explicit destructor calls
- Manual memory management using `malloc`/`free`
- High-resolution benchmarking
- Reusability of pre-constructed objects

---


