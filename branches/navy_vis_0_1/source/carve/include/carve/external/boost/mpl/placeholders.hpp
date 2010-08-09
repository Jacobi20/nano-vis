
#if !defined(BOOST_PP_IS_ITERATING)

///// header body

#ifndef BOOST_MPL_PLACEHOLDERS_HPP_INCLUDED
#define BOOST_MPL_PLACEHOLDERS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
// Copyright Peter Dimov 2001-2003
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /Users/sargeant/projects/PERSONAL/CARVE-SVN/include/carve/external/boost/mpl/placeholders.hpp,v $
// $Date: 2008/09/25 04:35:41 $
// $Revision: a8b879c8853c $


#if !defined(BOOST_MPL_PREPROCESSING_MODE)
#   include <carve/external/boost/mpl/arg.hpp>
#   include <carve/external/boost/mpl/aux_/adl_barrier.hpp>

#   if !defined(BOOST_MPL_CFG_NO_ADL_BARRIER_NAMESPACE)
#       define BOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(type) \
        using ::BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::type; \
        /**/
#   else
#       define BOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(type) /**/
#   endif

#endif

#include <carve/external/boost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
 && !defined(BOOST_MPL_PREPROCESSING_MODE)

#   define BOOST_MPL_PREPROCESSED_HEADER placeholders.hpp
#   include <carve/external/boost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <carve/external/boost/mpl/aux_/nttp_decl.hpp>
#   include <carve/external/boost/mpl/limits/arity.hpp>
#   include <carve/external/boost/preprocessor/iterate.hpp>
#   include <carve/external/boost/preprocessor/cat.hpp>

// watch out for GNU gettext users, who #define _(x)
#if !defined(_) || defined(BOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT)
BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN
typedef arg<-1> _;
BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

namespace boost { namespace mpl { 

BOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(_)

namespace placeholders {
using BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::_;
}

}}
#endif

/// agurt, 17/mar/02: one more placeholder for the last 'apply#' 
/// specialization
#define BOOST_PP_ITERATION_PARAMS_1 \
    (3,(1, BOOST_MPL_LIMIT_METAFUNCTION_ARITY + 1, <boost/mpl/placeholders.hpp>))
#include BOOST_PP_ITERATE()

#endif // BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // BOOST_MPL_PLACEHOLDERS_HPP_INCLUDED

///// iteration

#else
#define i_ BOOST_PP_FRAME_ITERATION(1)

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

typedef arg<i_> BOOST_PP_CAT(_,i_);

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

namespace boost { namespace mpl { 

BOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(BOOST_PP_CAT(_,i_))

namespace placeholders {
using BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::BOOST_PP_CAT(_,i_);
}

}}

#undef i_
#endif // BOOST_PP_IS_ITERATING
