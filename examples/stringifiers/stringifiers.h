#ifndef stringifiers_h
#define stringifiers_h

#include <stdcol>

#if defined(stdcol_platform_arduino)
#include <Arduino.h>
#elif defined(stdcol_nostl)
#error "Unkown platform without STL"
#else
#include <string>
#endif

namespace stdcol {
    namespace stringifiers {
        #ifdef stdcol_nostl
        using string_t = String;

        string_t to_string(index idx) {
            return String(idx.value);
        }

        template <typename T>
        string_t to_string(T value) {
            return String(value); //Overload this function to create to_string for undefined types.
        }
        #else
        using std::to_string;
        using string_t = std::string;

        string_t to_string(index idx) {
            return to_string(idx.value);
        }
        #endif

        namespace string_operations {
            void append_to(string_t& str, char c) {
                str += c;
            }

            void append_to(string_t& str, string_t s) {
                str += s;
            }
        }

        namespace printers {
            using namespace string_operations;

            struct printer {
                virtual void pre_loop(string_t& out, index size) = 0;

                virtual void pre_item(string_t& out, index size, index itemidx) = 0;

                virtual void post_item(string_t& out, index size, index itemidx) = 0;

                virtual void post_loop(string_t& out, index size) = 0;
            };

            struct basic : printer {
                void pre_loop(string_t& out, index size) override {}

                void pre_item(string_t& out, index size, index itemidx) override {}

                void post_item(string_t& out, index size, index itemidx) override {
                    append_to(out, ' ');
                }

                void post_loop(string_t& out, index size) override {}
            };

            struct python : printer {
                void pre_loop(string_t& out, index size) override {
                    append_to(out, '[');
                }

                void pre_item(string_t& out, index size, index itemidx) override {}

                void post_item(string_t& out, index size, index itemidx) override {
                    if (itemidx != size - 1) {
                        append_to(out, ", ");
                    }
                }

                void post_loop(string_t& out, index size) override {
                    append_to(out, ']');
                }
            };

            struct indexed : printer {
                void pre_loop(string_t& out, index size) override {}

                void pre_item(string_t& out, index size, index itemidx) override {
                    append_to(out, '[');
                    append_to(out, to_string(itemidx));
                    append_to(out, "]: ");
                }

                void post_item(string_t& out, index size, index itemidx) override {
                    if (itemidx != size - 1) {
                        append_to(out, ' '); 
                    }
                }

                void post_loop(string_t& out, index size) override {}
            };

            python default_printer;
            printer* global_printer = &default_printer;
        }

        template <typename collectable_t>
        string_t to_string(collection<collectable_t>& collection, printers::printer* printer = nullptr) {
            using namespace printers;
            printer = printer ? printer : global_printer;

            string_t out;
            index csize = size(collection);

            printer->pre_loop(out, csize);
            for (auto e : enumerate(collection)) {
                printer->pre_item(out, csize, e.index);
                append_to(out, to_string(e.value));
                printer->post_item(out, csize, e.index);
            }
            printer->post_loop(out, csize);
            return out;
        }

        template <typename collectable_t>
        string_t to_string(collection<collectable_t>& collection, printers::printer& printer) {
            return to_string(collection, &printer);
        }

        template <typename collectable_t, typename to_string_arg_t>
        string_t to_string(collection<collectable_t>& collection, printers::printer* printer, string_t (*custom_to_string)(to_string_arg_t)) {
            using namespace printers;
            printer = printer ? printer : global_printer;

            string_t out;
            index csize = size(collection);

            printer->pre_loop(out, csize);
            for (auto e : enumerate(collection)) {
                printer->pre_item(out, csize, e.index);
                append_to(out, custom_to_string(e.value));
                printer->post_item(out, csize, e.index);
            }
            printer->post_loop(out, csize);
            return out;
        }

        template <typename collectable_t, typename to_string_arg_t>
        string_t to_string(collection<collectable_t>& collection, printers::printer& printer, string_t (*custom_to_string)(to_string_arg_t)) {
            return to_string(collection, &printer, custom_to_string);
        }
    }
}

#endif
