#ifndef tree_h
#define tree_h

#include "abstract_tree.h"
#include "linked.h"

namespace stdcol {
    template <typename T>
    class tree;

    template <typename T>
    class tree_node : public abstract_tree_node<T> {
    public:
        using node = abstract_tree_node<T>;

        using link = tree_node<T>*;

        tree_node()
            : value(T()), parent_node(nullptr), children_nodes(linked<link>()) {}

        tree_node(link parent_node) = delete;

        template <typename... args_t>
        tree_node(link parent_node, args_t... args)
            : value(args...), parent_node(nullptr), children_nodes(linked<link>()) {}

        T& get_value() override {
            return value;
        };

        const T& get_value() const override {
            return value;
        }

        collection<typename abstract_tree_node<T>::tree_link>& children() override {
            return (collection<typename abstract_tree_node<T>::tree_link>&)children_nodes;
        }

        linked<link>& links() {
            return children_nodes;
        }

        link parent() override {
            return parent_node;
        };

    protected:
        friend class tree<T>;

        T value;
        mutable link parent_node;
        mutable linked<link> children_nodes;
    };

    template <typename T>
    class tree : public abstract_tree<tree_node<T>> {
    public:
        using node = tree_node<T>;

        using link = node*;

        using const_link = const node*;

        link root() override {
            return root_node;
        }

        const_link root() const override {
            return root_node;
        }

        link set_root(link new_root) {
            link old = root_node;
            root_node = new_root;
            return old;
        }

        void remove(link node) {
            if (node == nullptr) {
                return;
            }

            for (link c : node->children_nodes) {
                remove(c);
            }

            delete node;
        }

        virtual ~tree() {
            remove(root_node);
        }

    protected:
        link root_node;
    };
}

#endif
