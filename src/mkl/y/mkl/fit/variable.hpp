//! \file

#ifndef Y_MATH_FIT_VARIABLE_INCLUDED
#define Y_MATH_FIT_VARIABLE_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/hash/set.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon {

    namespace mkl {

        namespace Fit {

            //==================================================================
            //
            //
            //! named variable interface
            //
            //
            //==================================================================
            class Variable : public object, public counted
            {
            public:
                //==============================================================
                //
                // types and definitions
                //
                //==============================================================
                typedef intr_ptr<string,Variable>               Pointer;   //!< alias
                typedef key_hasher<string,hashing::fnv>         KeyHasher; //!< alias
                typedef memory::pooled                          Allocator; //!< alias
                typedef hash_set<string,Pointer,KeyHasher>      Set;       //!< alias
                typedef vector<string,Allocator>                Names;     //!< alias

                //==============================================================
                //
                // virtual interface
                //
                //==============================================================
                virtual size_t index() const throw() = 0; //!< index for parameters
                virtual       ~Variable()    throw();     //!< cleanup

                //==============================================================
                //
                // non virtual interface
                //
                //==============================================================
                //! get item from source with bound checking
                template <typename T>
                T & get( addressable<T> &source ) const
                {
                    return source[ query(source) ];
                }

                //! get item from const source with bound checking
                template <typename T>
                const T & get( const accessible<T> &source ) const
                {
                    return source[ query(source) ];
                }


                Variable(const Variable &);                                           //!< copy
                const string & key()                     const throw();               //!< key for set
                size_t         query(const collection &) const;                       //!< check index

                //! output
                friend std::ostream & operator<<( std::ostream &, const Variable &);

                //! copy into target the matching source index
                void activate( addressable<bool> &target, const accessible<bool> &source) const;

                //==============================================================
                //
                // members
                //
                //==============================================================
                const string name; //!< unique variable name


            protected:
                explicit Variable(const string &); //!< setup
                explicit Variable(const char   *); //!< setup

            private:
                Y_DISABLE_ASSIGN(Variable);
            };
        }
    }

}

#endif

