
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


template<typename Tag>
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
        : m_value(o.m_value)
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
    }
    
    lifecycle_counter& operator=(lifecycle_counter&& o)
    {
        m_value = o.m_value;
        move_assignments++;
    }
    
    bool operator == (const lifecycle_counter& o) const
    {
        return m_value = o.m_value;
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

static void test_container()
{
    test_ptree a;
    a.push_back({"key", test_ptree("value")});
    
}

int test_main(int, char *[])
{
    test_container();
    
    return 0;
}

#endif