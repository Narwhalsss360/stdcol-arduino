#ifndef directed_graph_h
#define directed_graph_h

#include "linked.h"

namespace stdcol {
    template <typename T>
    class graph_node {
    public:
        using node = graph_node<T>;

        using link = node*;

        template <typename... args_t>
        graph_node(args_t... args) : value(args...), edges(linked<link>()) {}

        T& get_value() {
            return value;
        }

        linked<link>& get_edges() const {
            return edges;
        }

        operator T&() {
            return value;
        }

        operator const T&() const {
            return value;
        }

        bool add_edge(link other) {
            if (other == nullptr) {
                return false;
            }

            if (other == this) {
                return false;
            }

            for (typename linked<link>::link current = edges.head(); current != nullptr; current = current->get_next()) {
                if (**current == other) {
                    return true;
                }
            }

            return edges.emplace(edges.size(), other);
        }

    protected:
        T value;
        mutable linked<link> edges;
    };

    template <typename T>
    class directed_graph {
    public:
        using node = typename graph_node<T>::node;

        using link = typename graph_node<T>::link;

        directed_graph() : vertices(linked<node>()) {}

        linked<node>& get_vertices() const {
            return vertices;
        }

        template <typename... args_t>
        link emplace(args_t... args) {
            if (!vertices.emplace(vertices.size(), args...)) {
                return nullptr;
            }

            typename linked<node>::link new_link = vertices.tail();

            for (typename linked<node>::link current = vertices.head(); current != nullptr; current = current->get_next()) {
                if (current == new_link) {
                    continue;
                }

                if (current->value == new_link->value) {
                    vertices.remove(vertices.size() - 1);
                    return &(*current);
                }
            }

            return &(*new_link);
        }

        bool add_edge(const T& source, const T& destination) {
            link source_edge = nullptr;
            link destination_edge = nullptr;

            for (typename linked<node>::link current = vertices.head(); current != nullptr && (source_edge == nullptr || destination_edge == nullptr); current = current->get_next()) {
                if (source_edge == nullptr) {
                    if (static_cast<T&>(**current) == source) {
                        source_edge = &**current;
                    }
                }

                if (destination_edge == nullptr) {
                    if (static_cast<T&>(**current) == destination) {
                        destination_edge = &**current;
                    }
                }
            }

            if (source_edge == nullptr || destination_edge == nullptr) {
                return false;
            }

            return source_edge->add_edge(destination_edge);
        }

        link get(const T& value) const {
            for (typename linked<node>::link current = vertices.head(); current != nullptr; current = current->get_next()) {
                if (static_cast<T&>(**current) == value) {
                    return &**current;
                }
            }
            return nullptr;
        }

    protected:
        linked<node> vertices;
    };
}

#endif

