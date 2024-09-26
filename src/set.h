#ifndef set_h
#define set_h

#include "dynamic_array.h"

namespace stdcol {
    template <typename collectable_t>
    class set : public dynamic_collection<collectable_t> {
    public:
        set() : items(dynamic_array<collectable_t>()) {}

        set(index size) : items(dynamic_array<collectable_t>(size)) {}

        set(const initializer_list<collectable_t>& init_list) : items(dynamic_array<collectable_t>()) {
            *this = init_list;
        }

        set(const set<collectable_t>& other) : set() {
            *this = static_cast<const collection<collectable_t>&>(other);
        }

        bool contains(const collectable_t& item) const {
            for (index i = 0; i < this->size(); i++) {
                if (items[i] == item) {
                    return true;
                }
            }
            return false;
        }

        index size() const override {
            return items.size();
        }

        collectable_t* const at(index index) override { return items.at(index); }

        const collectable_t* const at(index index) const override { return items.at(index); }

        index capacity() const override { return items.capacity(); }

        bool reserve(index new_capacity) override { return items.reserve(new_capacity); }

        bool resize(index new_size) override { return items.resize(new_size); }

        bool insert(index index, const collectable_t& item) override {
            if (contains(item)) {
                return true;
            }
            return items.insert(index, item);
        }

        bool remove(index index) override { return items.remove(index); }

        set<collectable_t>& operator=(const initializer_list<collectable_t>& init_list) {
            if (capacity() < init_list.size())
                reserve(init_list.size());

            for (const auto& i : init_list) {
                if (!insert(size(), i)) {
                    break;
                }
            }
            return *this;
        }

        set<collectable_t>& operator=(const collection<collectable_t>& other) {
            items.resize(0);
            items.resize(other.size());
            for (int i = 0; i < other.size(); i++) {
                insert(items.size(), other[i]);
            }
            return *this;
        }

        set<collectable_t>& operator=(const set<collectable_t>& other) {
            return operator=(static_cast<const collection<collectable_t>&>(other));
        }

    protected:
        dynamic_array<collectable_t> items;
    };
}

#endif
