//! \file

#ifndef Y_CHEMICAL_TYPES_INCLUDED
#define Y_CHEMICAL_TYPES_INCLUDED 1

#include "y/memory/allocator/dyadic.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/vector.hpp"
#include "y/associative/hash/set.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        // global types
        //______________________________________________________________________
        extern bool                       Verbosity;   //!< to debug
        typedef memory::dyadic            Allocator;   //!< default allocator
        typedef matrix<double>            Matrix;      //!< default matrix
        typedef vector<double,Allocator>  Vector;      //!< default vector
        typedef vector<string,Allocator>  Strings;     //!< default sequence of strings
        typedef addressable<double>       Addressable; //!< default addressable
        typedef accessible<double>        Accessible;  //!< default addressable
        typedef matrix<unit_t>            iMatrix;     //!< default matrix of integers
        typedef vector<bool,Allocator>    Flags;       //!< default vector of booleans
        typedef vector<size_t,Allocator>  Indices;     //!< default vector of indices

        //! output a debug message
#define Y_CHEMICAL_PRINTLN(MSG) do { if(Verbosity) { std::cerr << MSG << std::endl; } } while(false)
        
      
     
        
    }
}

#endif

