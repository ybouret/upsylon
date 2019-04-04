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

        class _pfd : public counted_object
        {
        public:
            typedef intr_ptr<natural,_pfd> pointer;

            const natural p; //!< a prime
            natural       q; //!< its power

            explicit _pfd(const natural &_p, const natural &_q);
            virtual ~_pfd() throw();
            _pfd( const _pfd &t );

            const natural & key() const throw() { return p; }

            typedef hashing::fnv                                   block_hasher;
            typedef key_hasher<natural,block_hasher>               prime_hasher;
            typedef memory::pooled                                 table_memory;
            typedef set<natural,pointer,prime_hasher,table_memory> table;

            inline friend std::ostream & operator<<( std::ostream &os, const _pfd &entry )
            {
                os << entry.p << '^' << entry.q;
                return os;
            }

        private:
            Y_DISABLE_ASSIGN(_pfd);
        };


        class pfd : public counted_object
        {
        public:
            const _pfd::table  table;
            virtual ~pfd() throw();
            pfd( const natural &n );
            pfd( const word_t   n );

            void mul_by( const pfd &other );


        private:
            Y_DISABLE_COPY_AND_ASSIGN(pfd);
            void setup(const natural &value);
        };

    }
}

#endif

