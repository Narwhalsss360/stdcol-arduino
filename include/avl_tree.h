#ifndef avl_tree_h
#define avl_tree_h

#include "binary_tree.h"

namespace stdcol {
    template <typename T>
    class avl_tree : public abstract_tree<binary_tree_node<T>> {
    public:
        using node = typename binary_tree<T>::node;

        using link = typename binary_tree<T>::link;

        using const_link = typename binary_tree<T>::const_link;

        avl_tree() : bin_tree() {}

        link root() override {
            return bin_tree.root();
        }

        const_link root() const override {
            return bin_tree.root();
        }

        link set_root(link new_root) {
            return bin_tree.set_root(new_root);
        }

        template <typename... args_t>
        link emplace(args_t... args) {
            link new_node = bin_tree.emplace<args_t...>(args...);
            bin_tree.set_root(new_node->balance_ancestors());
            return new_node;
        }

        void remove(link node) {
            return bin_tree.remove(node);
        }

        link find(bool (*predicate)(const T&)) {
            return bin_tree.find(predicate);
        }

        link find(const T& value) {
            return bin_tree.find(value);
        }

        tree_traversals::in_order<T> begin() {
            return bin_tree.begin();
        }

        tree_traversals::in_order<T> end() {
            return bin_tree.end();
        }

        iterable<tree_traversals::in_order<T>> in_order() {
            return bin_tree.in_order();
        }

        iterable<tree_traversals::pre_order<T>> pre_order() {
            return bin_tree.pre_order();
        }

        iterable<tree_traversals::post_order<T>> post_order() {
            return bin_tree.post_order();
        }

        binary_tree<T>& get_bin_tree() {
            return bin_tree;
        }

        operator binary_tree<T>&() {
            return bin_tree;
        }

        operator const binary_tree<T>&() {
            return bin_tree;
        }

    protected:
        binary_tree<T> bin_tree;
    };
}

#endif
