
// ----------------------------------------------------------------------------
// Copyright (C) 2017 Maciej Gajewski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
// These test are to make sure that no uneccesary copy of storwd object is performend, and that move is prefered over copy
// whenever applicable

#include <boost/property_tree/ptree.hpp>
#include <boost/test/minimal.hpp>


template<typename TAG>
struct lifecycle_counter
{
    static int default_ctors;
    static int copy_ctors;
    static int move_ctors;
    static int init_ctors;
    static int copy_assigments;
    static int move_assignments;
    static int destructors;

    static void reset_counters()
    {
        default_ctors = 0;
        copy_ctors = 0;
        move_ctors = 0;
        init_ctors = 0;
        copy_assigments = 0;
        move_assignments = 0;
        destructors = 0;
    }
    
    std::string m_value;
    
    lifecycle_counter()
    {
        default_ctors++;
    }
    
    lifecycle_counter(const std::string& from)
        : m_value(from)
    {
        init_ctors++;
    }
    
    lifecycle_counter(const char* from)
        : m_value(from)
    {
        init_ctors++;
    }

    lifecycle_counter(const lifecycle_counter& o)
        : m_value(o.m_value)
    {
        copy_ctors++;
    }
    
    lifecycle_counter(lifecycle_counter&& o)
        : m_value(std::move(o.m_value))
    {
        move_ctors++;
    }
    
    ~lifecycle_counter()
    {
        destructors++;
    }
    
    lifecycle_counter& operator=(const lifecycle_counter& o)
    {
        m_value = o.m_value;
        copy_assigments++;
        return *this;
    }
    
    lifecycle_counter& operator=(lifecycle_counter&& o)
    {
        m_value = o.m_value;
        move_assignments++;
        return *this;
    }
    
    bool operator == (const lifecycle_counter& o) const
    {
        return m_value == o.m_value;
    }

    bool operator < (const lifecycle_counter& o) const
    {
        return m_value < o.m_value;
    }
};

template<typename TAG> int lifecycle_counter<TAG>::default_ctors = 0;
template<typename TAG> int lifecycle_counter<TAG>::copy_ctors = 0;
template<typename TAG> int lifecycle_counter<TAG>::move_ctors = 0;
template<typename TAG> int lifecycle_counter<TAG>::init_ctors = 0;
template<typename TAG> int lifecycle_counter<TAG>::copy_assigments = 0;
template<typename TAG> int lifecycle_counter<TAG>::move_assignments = 0;
template<typename TAG> int lifecycle_counter<TAG>::destructors = 0;


using key_type = lifecycle_counter<struct key_tag>;
using data_type = lifecycle_counter<struct data_tag>;

using test_ptree = boost::property_tree::basic_ptree<key_type, data_type>; 

namespace boost { namespace property_tree {

template<>
struct path_of<key_type>
{
    using type = std::string;
};
    
}}

static void reset_counters()
{
    key_type::reset_counters();
    data_type::reset_counters();
}

// static test_ptree make_one_element_tree()
// {
//     test_ptree a;
//     a.push_back({"key", test_ptree("value")});
//     return a;
// }

// Test copy/move constructor of the entire test_container
static void test_constructor()
{
    test_ptree a("value");
    
    // Copy - expect value to be copied
    reset_counters();
    test_ptree b = a;
    BOOST_CHECK(data_type::copy_ctors == 1);
    
    // Move - expect the vaule not to be copied
    reset_counters();
    test_ptree c = std::move(a);
    BOOST_CHECK(data_type::copy_ctors == 0);
    BOOST_CHECK(data_type::copy_assigments == 0);
}

static void test_push_back()
{
    test_ptree tree;
    
    // push_back - copy
    test_ptree::value_type child1("key1", test_ptree("c1"));

    reset_counters();
    tree.push_back(child1);
    
    BOOST_CHECK(data_type::copy_ctors == 1);
    BOOST_CHECK(key_type::copy_ctors == 1);

    // push_back - move
    test_ptree::value_type child2("key2", test_ptree("c2"));

    reset_counters();
    tree.push_back(std::move(child2));
    
    BOOST_CHECK(data_type::copy_ctors == 0);
    BOOST_CHECK(data_type::copy_assigments == 0);
}

static void test_push_front()
{
    test_ptree tree;
    
    // push_back - copy
    test_ptree::value_type child1("key1", test_ptree("c1"));

    reset_counters();
    tree.push_front(child1);
    
    BOOST_CHECK(data_type::copy_ctors == 1);
    BOOST_CHECK(key_type::copy_ctors == 1);

    // push_back - move
    test_ptree::value_type child2("key2", test_ptree("c2"));

    reset_counters();
    tree.push_front(std::move(child2));
    
    BOOST_CHECK(data_type::copy_ctors == 0);
    BOOST_CHECK(data_type::copy_assigments == 0);
}

int test_main(int, char *[])
{
    test_constructor();
    test_push_back();
    test_push_front();
    
    return 0;
}

#endif // BOOST_NO_CXX11_RVALUE_REFERENCES