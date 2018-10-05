//! \file
#ifndef Y_IOBITS_INCLUDED
#define Y_IOBITS_INCLUDED 1

#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/object.hpp"

namespace upsylon
{


    struct iobit
    {
        iobit  *next;
        iobit  *prev;
        bool    code;

        typedef core::list_of<iobit> list_type;
        typedef core::pool_of<iobit> pool_type;

        inline static iobit * create( bool flag )
        {
            iobit *b = object::acquire1<iobit>();
            b->code = flag;
            return b;
        }

        inline static void destroy( iobit *b ) throw()
        {
            assert(b);
            object::release1(b);
        }

        class list : public list_type
        {
        public:
            inline explicit list() throw() : list_type() {}
            inline virtual ~list() throw() { clear(); }
            inline void     clear() throw() { while(size) destroy( pop_back() ); }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(list);
        };

        class pool : public pool_type
        {
        public:
            inline explicit pool() throw() : pool_type() {}
            inline virtual ~pool() throw() { clear(); }
            inline void clear() throw() { while(size) destroy( query() ); }
            inline void keep(const size_t nmax) throw() { while(size>nmax) destroy(query()); }
            inline void reserve(size_t n) { while(n-->0) store(object::acquire1<iobit>()); }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(pool);
        };

    };

    //! low level I/O bits FIFO
    class iobits : public iobit::list
    {
    public:
        explicit iobits() throw();
        virtual ~iobits() throw();
        
        iobit::pool pool;

        inline iobit *query( const bool flag )
        {
            if( pool.size )
            {
                iobit *b = pool.query();
                b->code  = flag;
                return b;
            }
            else
            {
                return iobit::create(flag);
            }
        }

        inline void push( const bool flag ) { push_back( query(flag) ); }
        inline bool peek() const throw()    { assert(size>0); return head->code; }
        inline void free() throw()
        {
            while(size) pool.store( pop_back() );
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(iobits);
    };

}

#endif

