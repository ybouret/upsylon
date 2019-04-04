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

        class pfd_entry : public counted_object
        {
        public:
            typedef intr_ptr<natural,pfd_entry> pointer;

            const natural p; //!< a prime
            natural       q; //!< its power

            explicit pfd_entry(const natural &_p, const natural &_q);
            virtual ~pfd_entry() throw();
            pfd_entry( const pfd_entry &t );

            const natural & key() const throw() { return p; }

            typedef hashing::fnv                                   block_hasher;
            typedef key_hasher<natural,block_hasher>               prime_hasher;
            typedef memory::pooled                                 table_memory;
            typedef set<natural,pointer,prime_hasher,table_memory> table;

            inline friend std::ostream & operator<<( std::ostream &os, const pfd_entry &entry )
            {
                os << entry.p << '^' << entry.q;
                return os;
            }

        private:
            Y_DISABLE_ASSIGN(pfd_entry);
        };

        class pfd_table : public pfd_entry::table
        {
        public:
            void add( const natural &p, const natural &q );
            void sub( const natural &p, const natural &q );

            explicit pfd_table() throw();
            virtual ~pfd_table() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pfd_table);
        };

        class pfd : public counted_object
        {
        public:
            const natural    value;
            const pfd_table  table;
            virtual ~pfd() throw();
            pfd( const natural &n );
            pfd( const word_t   n );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pfd);
            void setup();
        };

    }
}

#endif

