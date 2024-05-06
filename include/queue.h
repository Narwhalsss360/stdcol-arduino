#ifndef queue_h
#define queue_h

#include "dynamic_array.h"

namespace stdcol {
    template <typename collectable_t>
    class queue : public dynamic_array<collectable_t> {
    public:
        queue() : dynamic_array<collectable_t>() {}

        queue(const initializer_list<collectable_t>& init_list) : dynamic_array<collectable_t>(init_list) {}

        queue(const queue<collectable_t>& other) : dynamic_array<collectable_t>(other) {}

        queue(index size) : dynamic_array<collectable_t>(size) {}

        bool enqueue(const collectable_t& item) {
            return this->insert(this->size(), item);
        }

        bool operator+=(const collectable_t& item) {
            return enqueue(item);
        }

        bool dequeue(collectable_t* const out) {
            if (this->size() == 0) {
                return false;
            }
            if (out != nullptr) {
                *out = *this->at(0);
            }
            this->remove(0);
            return true;
        }

        bool dequeue(collectable_t& out) {
            return dequeue(&out);
        }

        bool operator-=(collectable_t& out) {
            return dequeue(out);
        }

        queue<collectable_t>& operator=(const queue<collectable_t>& other) {
            return static_cast<dynamic_array<collectable_t>&>(*this) = static_cast<const dynamic_array<collectable_t>&>(other);
        }
    };
}

#endif
