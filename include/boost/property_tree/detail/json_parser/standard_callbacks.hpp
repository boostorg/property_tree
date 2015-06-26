#ifndef BOOST_PROPERTY_TREE_DETAIL_JSON_PARSER_STANDARD_CALLBACKS_HPP
#define BOOST_PROPERTY_TREE_DETAIL_JSON_PARSER_STANDARD_CALLBACKS_HPP

#include <boost/property_tree/ptree.hpp>
#include <vector>

namespace boost { namespace property_tree {
    namespace json_parser { namespace detail
{

    namespace constants
    {
        template <typename Ch> const Ch* null_value();
        template <> inline const char* null_value() { return "null"; }
        template <> inline const wchar_t* null_value() { return L"null"; }

        template <typename Ch> const Ch* true_value();
        template <> inline const char* true_value() { return "true"; }
        template <> inline const wchar_t* true_value() { return L"true"; }

        template <typename Ch> const Ch* false_value();
        template <> inline const char* false_value() { return "false"; }
        template <> inline const wchar_t* false_value() { return L"false"; }
    }

    template <typename Ptree>
    class standard_callbacks {
    public:
        typedef typename Ptree::data_type string;
        typedef typename string::value_type char_type;

        void on_null() {
            new_value() = constants::null_value<char_type>();
        }

        void on_boolean(bool b) {
            new_value() = b ? constants::true_value<char_type>()
                            : constants::false_value<char_type>();
        }

        template <typename Range>
        void on_number(Range code_units) {
            new_value().assign(code_units.begin(), code_units.end());
        }
        void on_begin_number() {
            new_value();
        }
        void on_digit(char_type d) {
            current_value() += d;
        }
        void on_end_number() {}

        void on_begin_string() {
            new_value();
        }
        template <typename Range>
        void on_code_units(Range code_units) {
            current_value().append(code_units.begin(), code_units.end());
        }
        void on_code_unit(char_type c) {
            current_value() += c;
        }
        void on_end_string() {}

        void on_begin_array() {
            new_tree();
            stack.back().k = kind::array;
        }
        void on_end_array() {
            if (stack.back().k == kind::leaf) stack.pop_back();
            stack.pop_back();
        }

        void on_begin_object() {
            new_tree();
            stack.back().k = kind::object;
        }
        void on_end_object() {
            if (stack.back().k == kind::leaf) stack.pop_back();
            stack.pop_back();
        }

        Ptree& output() { return root; }

    protected:
        bool is_key() const {
            return stack.back().k == kind::key;
        }
        string& current_value() {
            auto& l = stack.back();
            switch (l.k) {
            case kind::key: return key_buffer;
            default: return l.t->data();
            }
        }

    private:
        Ptree root;
        string key_buffer;
        enum class kind { array, object, key, leaf };
        struct layer { kind k; Ptree* t; };
        std::vector<layer> stack;

        Ptree& new_tree() {
            if (stack.empty()) {
                stack.push_back({kind::leaf, &root});
                return root;
            }
            auto& l = stack.back();
            switch (l.k) {
            case kind::array:
                l.t->push_back(std::make_pair(string(), Ptree()));
                stack.push_back({kind::leaf, &l.t->back().second});
                return *stack.back().t;
            case kind::object:
                assert(false); // must start with string, i.e. call new_value
            case kind::key:
                l.t->push_back(std::make_pair(std::move(key_buffer), Ptree()));
                l.k = kind::object;
                stack.push_back({kind::leaf, &l.t->back().second});
                return *stack.back().t;
            case kind::leaf:
                stack.pop_back();
                return new_tree();
            }
            assert(false);
            __assume(false);
        }
        string& new_value() {
            if (stack.empty()) return new_tree().data();
            auto& l = stack.back();
            switch (l.k) {
            case kind::leaf:
                stack.pop_back();
                return new_value();
            case kind::object:
                l.k = kind::key;
                key_buffer.clear();
                return key_buffer;
            }
            return new_tree().data();
        }
    };

}}}}

#endif
