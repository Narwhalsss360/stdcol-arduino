#ifndef dynamic_array_h
#define dynamic_array_h

#include "dynamic_collection.h"
#include "stdcol_init_list.h"

namespace stdcol {
    template <typename collectable_t>
    class dynamic_array : public dynamic_collection<collectable_t> {
    public:
        dynamic_array() : block(nullptr), block_size(0), block_capacity(0) {}

        dynamic_array(const initializer_list<collectable_t>& init_list) : dynamic_array<collectable_t>() {
            *this = init_list;
        }

        dynamic_array(const dynamic_array<collectable_t>& other) : dynamic_array() {
            *this = static_cast<const collection<collectable_t>&>(other);
        }

        dynamic_array(index size) : dynamic_array() {
            resize(size);
        }

        collectable_t* const at(index index) override {
            return block + index;
        }

        const collectable_t* const at(index index) const override {
            return block + index;
        }

        index size() const override {
            return block_size;
        }

        index capacity() const override {
            return block_capacity;
        }

        bool reserve(index new_capacity) override {
            if (new_capacity == 0) {
                if (block)
                    delete[] block;
                block = nullptr;
                block_size = 0;
                block_capacity = 0;
                return true;
            }

            if (block) {
                collectable_t* new_block = new collectable_t[static_cast<typename index::int_type>(new_capacity)]();
                if (new_block == nullptr) {
                    return false;
                }

                for (index i = 0; i < block_size; i++) {
                    new_block[i] = block[i];
                }

                delete[] block;
                block = new_block;
            } else {
                block = new collectable_t[static_cast<typename index::int_type>(new_capacity)]();
                if (block == nullptr) {
                    return false;
                }
            }

            if (new_capacity < block_size) {
                block_size = new_capacity;
            }
            block_capacity = new_capacity;
            return true;
        }

        bool resize(index new_size) override {
            if (new_size > block_capacity) {
                if (!reserve(new_size)) {
                    return false;
                }
                block_size = new_size;
            } else {
                block_size = new_size;
            }

            return true;
        }

        bool insert(index index, const collectable_t& item) override {
            if (index >= size()) {
                if (!resize(index + 1)) {
                    return false;
                }
            } else {
                if (!resize(size() + 1)) {
                    return false;
                }
            }

            for (stdcol::findex src = size() - 2, dst = size() - 1; dst >= index && dst.found(); src--, dst--) {
                block[dst] = block[src];
            }

            block[index] = item;
            return true;
        }

        bool remove(index index) override {
            if (index >= size()) {
                return false;
            }

            for (stdcol::index src = index + 1, dst = index; src < size(); src++, dst++) {
                block[dst] = block[src];
            }

            block_size--;
            return true;
        }

        dynamic_array<collectable_t>& operator=(const initializer_list<collectable_t>& init_list) {
            if (capacity() < init_list.size())
                reserve(init_list.size());

            for (const auto& item : init_list) {
                if (!insert(size(), item)) {
                    break;
                }
            }
            return *this;
        }

        dynamic_array<collectable_t>& operator=(const collection<collectable_t>& other) {
            if (resize(other.size())) {
                for (index i = 0; i < size(); i++) {
                    block[i] = other[i];
                }
            }
            return *this;
        }

        dynamic_array<collectable_t>& operator=(const dynamic_array<collectable_t>& other) {
            return operator=(static_cast<const collection<collectable_t>&>(other));
        }

        virtual ~dynamic_array() {
            reserve(0);
        }

    protected:
        collectable_t* block;
        index block_size, block_capacity;
    };
}

#endif
