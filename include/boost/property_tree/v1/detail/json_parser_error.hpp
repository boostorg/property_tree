// ----------------------------------------------------------------------------
// Copyright (C) 2002-2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_V1_DETAIL_JSON_PARSER_ERROR_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_V1_DETAIL_JSON_PARSER_ERROR_HPP_INCLUDED

#include <boost/property_tree/version.hpp>
#include <boost/property_tree/v1/detail/file_parser_error.hpp>
#include <string>

BOOST_PROPERTY_TREE_OPENNS(1) namespace json_parser {

    //! Json parser error
    class json_parser_error: public file_parser_error
    {
    public:
        json_parser_error(const std::string &message, 
                          const std::string &filename, 
                          unsigned long line): 
            file_parser_error(message, filename, line)
        { 
        }
    };

} BOOST_PROPERTY_TREE_CLOSENS()

#endif
