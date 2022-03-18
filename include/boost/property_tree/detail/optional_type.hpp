#ifndef BOOST_PROPERTY_TREE_DETAIL_OPTIONAL_TYPE_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_OPTIONAL_TYPE_HPP_INCLUDED

#if defined(BOOST_PROPERTY_TREE_USE_STD_OPTIONAL)
#  include <optional>
#else
#  include <boost/optional.hpp>
#endif

namespace boost { namespace property_tree {

#if defined(BOOST_PROPERTY_TREE_USE_STD_OPTIONAL)
    template <class T>
    using optional = std::optional<T>;
#else
    template <class T>
    using optional = boost::optional<T>;
#endif

}}

#endif
