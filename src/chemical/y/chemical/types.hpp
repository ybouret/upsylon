//! \file

#ifndef Y_CHEMICAL_TYPES_INCLUDED
#define Y_CHEMICAL_TYPES_INCLUDED 1

#include "y/memory/allocator/dyadic.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/vector.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        // global types
        //______________________________________________________________________
        extern bool                      Verbosity; //!< to debug
        typedef memory::dyadic           Allocator; //!< default allocator
        typedef matrix<double>           Matrix;    //!< default matrix
        typedef vector<double,Allocator> Vector;    //!< default vector
        
        //! output a debug message
#define Y_CHEMICAL_PRINTLN(MSG) do { if(Verbosity) { std::cerr << MSG << std::endl; } } while(false)
        
        //______________________________________________________________________
        //
        //
        //! base type for dynamic classes
        //
        //______________________________________________________________________
        class Object : public object, public counted
        {
        public:
            virtual ~Object() throw(); //!< cleanup
            
        protected:
            explicit Object() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Object);
        };
        
        //______________________________________________________________________
        //
        //
        //! base type for indexed class
        //
        //______________________________________________________________________
        class Indexed : public Object
        {
        public:
            virtual ~Indexed() throw();
            
            const size_t indx; //!< the [1...] index
            
        protected:
            explicit Indexed() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Indexed);
        };
        
        
    }
}

#endif

