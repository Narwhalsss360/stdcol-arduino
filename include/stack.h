#ifndef stack_h
#define stack_h

#include "dynamic_array.h"

namespace stdcol {
    template <typename collectable_t>
    class stack : public dynamic_array<collectable_t> {
    public:
        stack() : dynamic_array<collectable_t>() {}

        stack(const initializer_list<collectable_t>& init_list) : dynamic_array<collectable_t>(init_list) {}

        stack (const stack<collectable_t>& other) : dynamic_array<collectable_t>(other) {}

        stack(index size) : dynamic_array<collectable_t>(size) {}

        bool push(const collectable_t& item) {
            return this->insert(this->size(), item);
        }

        bool operator+=(const collectable_t& item) {
            return push(item);
        }

        bool pop(collectable_t* const out) {
            if (this->size() == 0) {
                return false;
            }
            if (out != nullptr) {
                *out = *this->at(this->size() - 1);
            }
            this->remove(this->size() - 1);
            return true;
        }

        bool pop(collectable_t& out) {
            return pop(&out);
        }

        bool operator-=(collectable_t& out) {
            return pop(out);
        }

        stack<collectable_t>& operator=(const stack<collectable_t>& other) {
            return (dynamic_array<collectable_t>&)*this = (const dynamic_array<collectable_t>&)other;
        }
    };
}

#endif
