//! \file

#ifndef Y_MPL_PFD_INCLUDED
#define Y_MPL_PFD_INCLUDED

#include "y/mpl/natural.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace mpl
    {

        //! prime factor
        class _pfd : public counted_object
        {
        public:
            typedef intr_ptr<natural,_pfd>                         pointer;      //!< alias
            typedef hashing::fnv                                   block_hasher; //!< hash bytes
            typedef key_hasher<natural,block_hasher>               prime_hasher; //!< hash key
            typedef memory::pooled                                 table_memory; //!< medium sized tables
            typedef set<natural,pointer,prime_hasher,table_memory> table;        //!< table

            const natural p; //!< a prime
            natural       q; //!< its power

            explicit _pfd(const natural &_p, const natural &_q); //!< setup
            virtual ~_pfd() throw();                             //!< cleanup
            _pfd( const _pfd &t );                               //!< copy
            const natural & key() const throw();                 //!< key for table


            //! output
            friend std::ostream & operator<<( std::ostream &os, const _pfd & );

            static int compare_data( const pointer &lhs, const pointer &rhs ) throw()
            {
                return natural::compare(lhs->p,rhs->p);
            }

        private:
            Y_DISABLE_ASSIGN(_pfd);
        };


        //! prime factor decomposition
        class pfd : public counted_object
        {
        public:
            typedef arc_ptr<pfd> pointer; //!< alias

            const _pfd::table  table; //!< table that holds the decomposition
            virtual ~pfd() throw();   //!< destructor
            pfd( const natural &n );  //!< setup from a natural
            pfd( const word_t   n );  //!< setup from an integral

            void mul_by( const pfd &other ); //!< mul by another or self

            friend std::ostream & operator<<( std::ostream &os, const pfd &F );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pfd);
            void setup(const natural &value);
        };

    }
}

#endif

