// ----------------------------------------------------------------------------
// Copyright (C) 2014 Sebastian Redl
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_DETAIL_VERSION_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_VERSION_HPP_INCLUDED

// This defines the active version of PropertyTree. Feel free to redefine this
// macro to your purposes, but keep in mind that you're affecting everyone in
// your translation unit.
//
// Current versions:
//   1: C++03 compatible
//
// Planned versions:
//   2: C++11 updates

#include <boost/property_tree/detail/versioned_namespace.hpp>

#if !defined(BOOST_VERSION_PROPERTY_TREE)
#define BOOST_VERSION_PROPERTY_TREE 1
#endif

#if BOOST_VERSION_PROPERTY_TREE < 1 || BOOST_VERSION_PROPERTY_TREE > 1
#error Invalid requested version of PropertyTree.
#endif

#define BOOST_PROPERTY_TREE_OPENNS(version) \
    namespace boost { namespace property_tree { \
    BOOST_VERSIONED_NAMESPACE(PROPERTY_TREE, version) {
#define BOOST_PROPERTY_TREE_CLOSENS() }}}

#endif
