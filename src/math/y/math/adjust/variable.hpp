//! \file

#ifndef Y_MATH_ADJUST_VARIABLE_INCLUDED
#define Y_MATH_ADJUST_VARIABLE_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/intr.hpp"
#include "y/sequence/addressable.hpp"
#include "y/associative/set.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon {
    
    namespace Adjust
    {
        //! variable interface
        class Variable : public counted_object
        {
        public:
            typedef intr_ptr<string,Variable>       Pointer;
            typedef key_hasher<string,hashing::fnv> KeyHasher;
            typedef memory::pooled                  Allocator;
            
            typedef set<string,Pointer,KeyHasher,Allocator> Set;
            
            
            const string name;
            
            virtual size_t index() const throw() = 0;
            virtual       ~Variable()    throw();
            
            template <typename T>
            T & get( addressable<T> &source )
            {
                return source[ query(source) ];
            }
            
            template <typename T>
            const T & get( const accessible<T> &source )
            {
                return source[ query(source) ];
            }
            
            
            
            Variable(const Variable &);
            const string & key() const throw();
            size_t query(const collection &source) const;
            
        protected:
            explicit Variable(const string &);
            explicit Variable(const char   *);
            
        private:
            Y_DISABLE_ASSIGN(Variable);
        };
    }
}

#endif

