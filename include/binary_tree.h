#ifndef binary_tree_h
#define binary_tree_h

#include "abstract_tree.h"
#include "array.h"
#include "dynamic_array.h"
#include "stack.h"
#include "set.h"

namespace stdcol {
    template <typename T>
    class binary_tree;

    template <typename T>
    class binary_tree_node : public abstract_tree_node<T> {
    public:
        using node = abstract_tree_node<T>;

        using link = binary_tree_node<T>*;

        binary_tree_node()
            : value(T()), parent_node(nullptr), children_nodes({ nullptr, nullptr }) {}

        binary_tree_node(link) = delete;

        template <typename... args_t>
        binary_tree_node(link parent_node, args_t... args)
            : value(args...), parent_node(parent_node), children_nodes({ nullptr , nullptr }) {}

        T& get_value() override {
            return value;
        }

        link parent() override {
            return parent_node;
        }

        collection<typename node::tree_link>& children() override {
            return (collection<typename node::tree_link>&)children_nodes;
        }

        link& left() const {
            return children_nodes[0];
        }

        link& right() const {
            return children_nodes[1];
        }

    protected:
        friend class binary_tree<T>;

        T value;
        mutable link parent_node;
        mutable array<link, 2> children_nodes;
    };

    namespace tree_traversals {
        template <typename T>
        class in_order {
        public:
        in_order(binary_tree_node<T>* root)
            : next(stack<binary_tree_node<T>*>()), current(root) {}

        bool operator!=(const in_order<T>& other) const {
            return next.size() > 0 || current != nullptr;
        }

        in_order<T>& operator++() {
            current = current->right();
            return *this;
        }

        in_order<T>& operator++(int) {
            return operator++();
        }

        T& operator*() const {
            while (current != nullptr) {
                next += current;
                current = current->left();
            }

            next -= current;
            return **current;
        }

        protected:
            mutable stack<binary_tree_node<T>*> next;
            mutable binary_tree_node<T>* current;
        };

        template <typename T>
        class pre_order {
        public:
            pre_order(binary_tree_node<T>* root)
                : next(stack<binary_tree_node<T>*>({ root })), current(nullptr) {}

            bool operator!=(const pre_order<T>& other) const {
                return next.size() > 0;
            }

            pre_order<T>& operator++() {
                if (current != nullptr) {
                    if (current->right()) {
                        next += current->right();
                    }
                    if (current->left()) {
                        next += current->left();
                    }
                }
                return *this;
            }

            pre_order<T>& operator++(int) {
                return operator++();
            }

            T& operator*() const {
                next -= current;
                return **current;
            }

        protected:
            mutable stack<binary_tree_node<T>*> next;
            mutable binary_tree_node<T>* current;
        };

        template <typename T>
        class post_order {
        public:
            post_order(binary_tree_node<T>* root)
                : visited(set<binary_tree_node<T>*>()), current(root), root(root) {}

            bool operator!=(const post_order<T>& other) const {
                return current != nullptr && !visited.contains(current);
            }

            post_order<T>& operator++() {
                return *this;
            }

            post_order<T>& operator++(int) {
                return operator++();
            }

            T& operator*() const {
                if (current->left() != nullptr && !visited.contains(current->left())) {
                    current = current->left();
                    return operator*();
                } else if (current->right() != nullptr && !visited.contains(current->right())) {
                    current = current->right();
                    return operator*();
                } else {
                    T& value = **current;
                    visited.insert(visited.size(), current);
                    current = root;
                    return value;
                }
            }

        protected:
            mutable set<binary_tree_node<T>*> visited;
            mutable binary_tree_node<T>* current;
            mutable binary_tree_node<T>* root;
        };
    }

    template <typename T>
    class binary_tree : public abstract_tree<binary_tree_node<T>> {
    public:
        using node = binary_tree_node<T>;

        using link = node*;

        binary_tree()
            : root_node(nullptr) {}

        link root() override {
            return root_node;
        }

        template <typename... args_t>
        link emplace(args_t... args) {
            link new_node = new node(nullptr, args...);

            if (root_node == nullptr) {
                root_node = new_node;
            } else {
                link current = root_node;
                while (current != nullptr) {
                    if (new_node->value < current->value) {
                        if (current->left() == nullptr) {
                            new_node->parent_node = current;
                            current->left() = new_node;
                            break;
                        } else {
                            current = current->left();
                            continue;
                        }
                    } else {
                        if (current->right() == nullptr) {
                            new_node->parent_node = current;
                            current->right() = new_node;
                            break;
                        } else {
                            current = current->right();
                            continue;
                        }
                    }
                }
            }

            return new_node;
        }

        void remove(link node) {
            if (node == nullptr) {
                return;
            }

            link current = node, newCurrent = node;
            do {
                if (current->left() != nullptr) {
                    current = current->left();
                    continue;
                }

                if (current->right() != nullptr) {
                    current = current->right();
                    continue;
                }

                newCurrent = current->parent_node;

                if (newCurrent->left() == current) {
                    newCurrent->left() = nullptr;
                } else {
                    newCurrent->right() = nullptr;
                }
                delete current;
                current = newCurrent;
            } while (current != node || current->right() != nullptr);

            if (node->parent_node != nullptr) {
                if (node->parent_node->left() == node) {
                    node->parent_node->left() = nullptr;
                } else {
                    node->parent_node->right() = nullptr;
                }
            }

            if (node == root_node) {
                root_node = nullptr;
            }
            delete node;
        }

        link find(bool (*predicate)(const T&)) {
            if (predicate == nullptr) {
                return nullptr;
            }

            link current = root_node;

            while (current != nullptr) {
                int result = predicate(current->value);
                if (result == -1) {
                    current = current->left();
                } else if (result == 1) {
                    current = current->right();
                } else {
                    return current;
                }
            }

            return nullptr;
        }

        link find(const T& value) {
            link current = root_node;

            while (current != nullptr) {
                if (value < current->value) {
                    current = current->left();
                } else if (current->value < value) {
                    current = current->right();
                } else {
                    return current;
                }
            }

            return nullptr;
        }

        tree_traversals::in_order<T> begin() {
            return tree_traversals::in_order<T>(root());
        }

        tree_traversals::in_order<T> end() {
            return tree_traversals::in_order<T>(nullptr);
        }

        iterable<tree_traversals::in_order<T>> in_order() {
            return iterable<tree_traversals::in_order<T>>(tree_traversals::in_order<T>(root()), tree_traversals::in_order<T>(nullptr));
        }

        iterable<tree_traversals::pre_order<T>> pre_order() {
            return iterable<tree_traversals::pre_order<T>>(tree_traversals::pre_order<T>(root()), tree_traversals::pre_order<T>(nullptr));
        }

        iterable<tree_traversals::post_order<T>> post_order() {
            return iterable<tree_traversals::post_order<T>>(tree_traversals::post_order<T>(root()), tree_traversals::post_order<T>(nullptr));
        }

        ~binary_tree() {
            remove(root_node);
        }

    protected:
        link root_node;
    };

    template <typename T>
    iterable<tree_traversals::in_order<T>> iterate(binary_tree<T>& tree) {
        return tree.in_order();
    }
}

#endif
