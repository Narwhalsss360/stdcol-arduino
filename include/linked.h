#ifndef linked_h
#define linked_h

#include "dynamic_collection.h"

namespace stdcol {
    template <typename T>
    class linked;

    template <typename T>
    class linked_node {
    public:
        using link = linked_node<T>*;

        linked_node()
            : value(T()), previous(nullptr), next(nullptr) {}

        template <typename... args_t>
        linked_node(link previous, link next, args_t... args)
        : value(args...), previous(previous), next(next) {}

        T* operator&() { return &value; }

        const T* operator&() const { return &value; }

        T& operator*() { return value; }

        const T& operator*() const { return value; }

        link get_previous() const { return previous; }

        link get_next() const { return next; }

        T value;

    protected:
        friend class linked<T>;

        link previous, next;
    };

    template <typename T>
    class linked_iterator {
    public:
        using node = linked_node<T>;

        using link = node*;

        linked_iterator()
            : current(nullptr) {}

        linked_iterator(link current)
            : current(current) {}

        bool operator!=(const linked_iterator<T>& other) const {
            return current != nullptr;
        }

        linked_iterator<T>& operator++() {
            current = current->get_next();
            return *this;
        }

        linked_iterator<T>& operator++(int) { return operator++(); }

        T& operator*() const { return current->value; }

    protected:
        link current;
    };

    template <typename T>
    class linked : public dynamic_collection<T> {
    public:
        using node = linked_node<T>;

        using link = node*;

        linked()
            : head_node(nullptr), tail_node(nullptr) {}

        linked(const linked<T>& other) : linked() {
            *this = static_cast<const collection<T>&>(other);
        }

        T* const at(index index) override {
            stdcol::index i = 0;
            for (link current = head_node; current != nullptr; current = current->next, i++) {
                if (index == i) {
                    return &current->value;
                }
            }
            return &tail_node->value;
        }

        const T* const at(index index) const override {
            stdcol::index i = 0;
            for (link current = head_node; current != nullptr; current = current->next) {
                if (index == i) {
                    return &current->value;
                }
            }
            return &tail_node->value;
        }

        index size() const override {
            index i = 0;
            for (link n = head_node; n != nullptr; n = n->next, i++) {}
            return i;
        }

        index capacity() const override {
            return index::max;
        }

        bool reserve(index new_capacity) override {
            if (new_capacity == 0) {
                link current = head_node;
                while (current != nullptr) {
                    link next = current->next;
                    delete current;
                    current = next;
                }
                head_node = nullptr;
                tail_node = nullptr;
                return true;
            }

            //Items will be inserted whenever needed.
            if (new_capacity >= size()) {
                return true;
            }

            index i = 0;
            link n = tail_node;
            while (i - 1 != new_capacity) {
                link previous = n->previous;
                delete n;
                n = previous;
            }
            tail_node = n;
            return true;
        }

        bool resize(index new_size) override {
            link current = head_node;
            //current increases until new_size or current is nullptr.
            for (index i = 0; i < new_size && current != nullptr; i++, current = current->next);

            //new_size >= size()
            if (current == nullptr) {
                return true;
            }

            while (current != nullptr) {
                if (current->previous != nullptr) {
                    current->previous->next = nullptr;
                }

                link next = current->next;
                delete current;
                current = next;
            }

            return true;
        }

        bool insert(index index, const T& item) override {
            if (head_node == nullptr) {
                head_node = new node(nullptr, nullptr, item);
                if (head_node == nullptr) {
                    return false;
                }
                tail_node = head_node;
                return true;
            }

            if (index == 0) {
                link new_node = new node(nullptr, head_node, item);
                if (new_node == nullptr) {
                    return false;
                }
                head_node->previous = new_node;
                head_node = new_node;
                return true;
            }

            stdcol::index i = 0;
            link current = head_node;
            for (; current->next != nullptr; current = current->next, i++) {
                if (index == i) {
                    link new_link = new node(current->previous, current, item);
                    if (new_link == nullptr) {
                        return false;
                    }
                    if (current->previous != nullptr) {
                        current->previous->next = new_link;
                    }
                    new_link->next->previous = new_link;
                    return true;
                }
            }

            link new_link = new node(current, nullptr, item);
            if (new_link == nullptr) {
                return false;
            }
            current->next = new_link;
            tail_node = new_link;
            return true;
        }

        template <typename... args_t>
        bool emplace(index index, args_t... args) {
            if (head_node == nullptr) {
                head_node = new node(nullptr, nullptr, args...);
                if (head_node == nullptr) {
                    return false;
                }
                tail_node = head_node;
                return true;
            }

            if (index == 0) {
                link new_node = new node(nullptr, head_node, args...);
                if (new_node == nullptr) {
                    return false;
                }
                head_node->previous = new_node;
                head_node = new_node;
                return true;
            }

            stdcol::index i = 0;
            link current = head_node;
            for (; current->next != nullptr; current = current->next, i++) {
                if (index == i) {
                    link new_link = new node(current->previous, current, args...);
                    if (new_link == nullptr) {
                        return false;
                    }
                    if (current->previous != nullptr) {
                        current->previous->next = new_link;
                    }
                    new_link->next->previous = new_link;
                    return true;
                }
            }

            link new_link = new node(current, nullptr, args...);
            if (new_link == nullptr) {
                return false;
            }
            current->next = new_link;
            tail_node = new_link;
            return true;
        }

        bool remove(index idx) override {
            link current = head_node;
            for (index i = 0; current != nullptr; current = current->next, i++) {
                if (i == idx) {
                    if (current->previous != nullptr) {
                        current->previous->next = current->next;
                    }

                    if (current->next != nullptr) {
                        current->next->previous = current->previous;
                    }

                    break;
                }
            }
            return true;
        }

        link head() const {
            return head_node;
        }

        link tail() const {
            return tail_node;
        }

        linked<T>& operator=(const collection<T>& other) {
            index other_size = other.size();
            resize(0);

            for (int i = 0; i < other_size; i++) {
                if (!insert(i, other[i])) {
                    break;
                }
            }
            return *this;
        }

        linked<T>& operator=(const linked<T>& other) {
            return operator=(static_cast<const collection<T>&>(other));
        }

        ~linked() {
            reserve(0);
        }

    protected:
        link head_node, tail_node;
    };

    template <typename T>
    iterable<linked_iterator<T>> iterate(linked<T>& linked) {
        return iterate<linked_iterator<T>>(linked.head(), linked.tail());
    }

    template <typename T>
    enumerable<linked_iterator<T>, T&> enumerate(linked<T>& linked) {
        return enumerate<linked_iterator<T>, T&>(linked.head(), linked.tail());
    }
}

#endif
