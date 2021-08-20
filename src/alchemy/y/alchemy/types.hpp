

//! \file

#ifndef Y_ALCHEMY_TYPES_INCLUDED
#define Y_ALCHEMY_TYPES_INCLUDED 1

#include "y/string.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        typedef memory::dyadic                 Allocator;    //!< alias
        typedef vector<double,Allocator>       Vector;       //!< alias
        typedef vector<unit_t,Allocator>       iVector;      //!< alias
        typedef vector<string,Allocator>       Strings;      //!< alias
        typedef vector<bool,Allocator>         Flags;        //!< alias
        typedef addressable<double>            Addressable;  //!< alias
        typedef accessible<double>             Accessible;   //!< alias
        typedef vector<size_t,Allocator>       Indices;      //!< alias


    }

}

#endif
