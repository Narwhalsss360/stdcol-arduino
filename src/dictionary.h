#ifndef dictionary_h
#define dictionary_h

#include "dynamic_array.h"
#include "linked.h"
#include "stdcol_hasher.h"

namespace stdcol {
    template <typename key_t, typename value_t>
    struct key_value_pair {
        key_t key;
        value_t value;
    };

    template <typename hashable_t, typename collectable_t>
    class dictionary {
    public:
        using kvp_t = key_value_pair<hashable_t, collectable_t>;

        using bucket_t = linked<kvp_t>;

        using buckets_t = dynamic_array<bucket_t>;

        virtual bool contains(const hashable_t& key) = 0;

        virtual bool add(const hashable_t& key, const collectable_t& value) = 0;

        virtual kvp_t* get(const hashable_t& key) = 0;

        virtual collectable_t& get_add(const hashable_t& key) = 0;

        virtual collectable_t& operator[](const hashable_t& key) {
            return get_add(key);
        }

        virtual index size() {
            index sum = 0;
            for (bucket_t& buck : buckets()) {
                sum += buck.size();
            }
            return sum;
        }

        virtual dynamic_array<hashable_t> keys() {
            dynamic_array<hashable_t> keys = dynamic_array<hashable_t>(size());
            for (bucket_t& buck : buckets()) {
                for (kvp_t& kvp : buck) {
                    keys.insert(keys.size(), kvp.key);
                }
            }
            return keys;
        }

        virtual dynamic_array<collectable_t> values() { 
            dynamic_array<collectable_t> values = dynamic_array<collectable_t>(size());
            for (bucket_t& buck : buckets()) {
                for (kvp_t& kvp : buck) {
                    values.insert(values.size(), kvp.value);
                }
            }
            return values;
        }

        virtual dynamic_array<kvp_t> key_values() {
            dynamic_array<kvp_t> key_values = dynamic_array<kvp_t>(size());
            for (bucket_t& buck : buckets()) {
                for (kvp_t& kvp : buck) {
                    key_values.insert(key_values.size(), kvp);
                }
            }
            return key_values;
        }


        virtual bool remove(const hashable_t& key) = 0;

        virtual bool resize(index bucket_count) = 0;

        virtual buckets_t& buckets() = 0;

        virtual bucket_t& bucket(const hashable_t& hashable) = 0;

        virtual index hash(const hashable_t& key) = 0;
    };
}

#endif
