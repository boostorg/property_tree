// ----------------------------------------------------------------------------
// Copyright (C) 2014 Sebastian Redl
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_DETAIL_VERSIONED_NAMESPACE_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_VERSIONED_NAMESPACE_HPP_INCLUDED

// Utility for having multiple versions of a library, in inline namespaces.
//
// Usage:
// First, decide on a tag for your library, an all-caps identifier, such as
// YOUR_LIBRARY. For example, Boost.PropertyTree uses PROPERTY_TREE.
// Then, define BOOST_VERSION_YOUR_LIBRARY to be the active version number of
// your library:
//
// #if !defined(BOOST_VERSION_YOUR_LIBRARY)
// #define BOOST_VERSION_YOUR_LIBRARY 1
// #endif
//
// The check is so that the user can override your choice by defining the macro.
//
// Have your library namespace as usual. Then add the following inside
// your library namespace:
//
// BOOST_VERSIONED_NAMESPACE(YOUR_LIBRARY, 1) {
//   version 1 here
// }
// BOOST_VERSIONED_NAMESPACE(YOUR_LIBRARY, 2) {
//   version 2 here
// }
//
// That's it.
//
// Expansion:
// The macro BOOST_VERSIONED_NAMESPACE(tag, version) will expand thus:
// If BOOST_VERSION_<tag> does not equal <version>, it expands to
//   namespace v<version>
// (e.g. v1)
// If BOOST_VERSION_<tag> does equal <version>, if the compiler does not
// support inline namespaces, it expands to
//   extern "C++"
// This is just there to swallow the braces. The contents of the active version
// are directly within the outer namespace.
// If the compiler supports inline namespaces, it expands to
//   inline namespace v<version>

#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>

#define BOOST_VERSIONED_NAMESPACE(tag, version) \
    BOOST_PP_CAT(BOOST_VERSIONED_NAMESPACE_, \
        BOOST_PP_EQUAL(BOOST_PP_CAT(BOOST_VERSION_, tag), version))(version)
    /**/

#define BOOST_VERSIONED_NAMESPACE_0(version) \
    namespace BOOST_PP_CAT(v, version)
#if defined(BOOST_NO_CXX11_INLINE_NAMESPACES)
#  define BOOST_VERSIONED_NAMESPACE_1(version) \
       extern "C++"
#else
#  define BOOST_VERSIONED_NAMESPACE_1(version) \
       inline namespace BOOST_PP_CAT(v, version)
#endif

#endif
