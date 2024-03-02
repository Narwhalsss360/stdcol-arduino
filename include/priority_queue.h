#ifndef priority_queue_h
#define priority_queue_h

#include "dynamic_array.h"

namespace stdcol {
    template <typename collectable_t>
    struct prioritized_element {
        int priority;
        collectable_t item;

        prioritized_element()
            : priority(0), item(collectable_t()) {}

        prioritized_element(int priority, collectable_t item)
            : priority(priority), item(item) {}
    };

    template <typename collectable_t>
    class priority_queue : dynamic_collection<prioritized_element<collectable_t>> {
    public:
        priority_queue() : items(dynamic_array<prioritized_element<collectable_t>>()) {}

        priority_queue(const initializer_list<collectable_t>& init_list) : items(dynamic_array<prioritized_element<collectable_t>>(init_list)) {}

        priority_queue(const priority_queue<collectable_t>& other) : items(dynamic_array<prioritized_element<collectable_t>>(other)) {}

        priority_queue(index size) : items(dynamic_array<prioritized_element<collectable_t>>(size)) {}

        prioritized_element<collectable_t>* const at(index index) override {
            return items.at(index);
        }

        const prioritized_element<collectable_t>* const at(index index) const override {
            return items.at(index);
        }

        index size() const override {
            return items.size();
        }

        index capacity() const override {
            return items.capacity();
        }

        bool reserve(index new_capacity) override {
            return items.reserve(new_capacity);
        }

        bool resize(index new_size) override {
            return items.resize(new_size);
        }

        bool insert(index index, const prioritized_element<collectable_t>& item) override {
            return items.insert(index, item);
        }

        bool remove(index index) override {
            return items.remove(index);
        }

        bool enqueue(prioritized_element<collectable_t> item) {
            int i;
            for (i = 0; i < items.size(); i++) {
                if (item.priority > items[i].priority) {
                    break;
                } else if (item.priority < items[i].priority) {
                    continue;
                }
            }
            return items.insert(i, item);
        }

        bool operator+=(prioritized_element<collectable_t> item) {
            return enqueue(item);
        }

        bool dequeue(collectable_t* const out) {
            if (items.size() == 0) {
                return false;
            }
            *out = items[0].item;
            items.remove(0);
            return true;
        }

        bool dequeue(collectable_t& out) {
            return dequeue(&out);
        }

        bool operator-=(collectable_t& out) {
            return dequeue(out);
        }

        priority_queue<collectable_t>& operator=(const collection<collectable_t>& other) {
            for (int i = 0; i < other.size(); i++) {
                enqueue({ 0, other[i] });
            }
            return *this;
        }

        priority_queue<collectable_t>& operator=(const priority_queue<collectable_t>& other) {
            return operator=(static_cast<const collection<collectable_t>&>(other));
        }

    protected:
        dynamic_array<prioritized_element<collectable_t>> items;
    };
}

#endif
