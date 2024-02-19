#ifndef hash_table_h
#define hash_table_h

#include "dictionary.h"

namespace stdcol {
    template <typename hashable_t, typename collectable_t, typename hasher_t = hasher<hashable_t>>
    class hash_table : public dictionary<hashable_t, collectable_t> {
    public:
        using dict = dictionary<hashable_t, collectable_t>;

        hash_table() : table_buckets(typename dict::buckets_t(11)), key_hasher(hasher_t()) {}

        hash_table(index size) : table_buckets(typename dict::buckets_t(size)), key_hasher(hasher_t()) {}

        bool contains(const hashable_t& key) override {
            for (const typename dict::kvp_t& kvp : bucket(key)) {
                if (kvp.key == key) {
                    return true;
                }
            }
            return false;
        }

        bool add(const hashable_t& key, const collectable_t& value) override {
            linked<key_value_pair<hashable_t, collectable_t>>& key_bucket = bucket(key);
            for (key_value_pair<hashable_t, collectable_t>& kvp : key_bucket) {
                if (kvp.key == key) {
                    kvp.value = value;
                    return true;
                }
            }

            return key_bucket.insert(0, { key, value });
        }

        bool remove(const hashable_t& key) override {
            linked<key_value_pair<hashable_t, collectable_t>>& key_bucket = bucket(key);
            for (auto ekvp : enumerate(key_bucket)) {
                if (ekvp.value.key == key) {
                    return key_bucket.remove(ekvp.index);
                }
            }
            return true;
        }

        bool resize(index bucket_count) override {
            typename dict::buckets_t new_buckets = typename dict::buckets_t(bucket_count);
            if (new_buckets.size() != bucket_count) {
                return false;
            }
            return true;
        }

        typename dict::kvp_t* get(const hashable_t& key) override {
            for (typename dict::kvp_t& kvp : bucket(key)) {
                if (kvp.key == key) {
                    return & kvp;
                }
            }
            return nullptr;
        }

        collectable_t& get_add(const hashable_t& key) override {
             typename dict::bucket_t& key_bucket = bucket(key);
             for (typename dict::kvp_t& kvp : key_bucket) {
                if (kvp.key == key) {
                    return kvp.value;
                }
            }

            key_bucket.insert(0, { key, collectable_t() });
            return key_bucket[0].value;
        }

        typename dict::buckets_t& buckets() override {
            return table_buckets;
        }

        typename dict::bucket_t& bucket(const hashable_t& hashable) override {
            return table_buckets[hash(hashable) % table_buckets.size()];
        }

        index hash(const hashable_t& key) override {
            return key_hasher(key);
        }

    protected:
        typename dict::buckets_t table_buckets;
        hasher_t key_hasher;
    };
}

#endif
