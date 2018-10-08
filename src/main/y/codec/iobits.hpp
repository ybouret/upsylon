//! \file
#ifndef Y_IOBITS_INCLUDED
#define Y_IOBITS_INCLUDED 1

#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/object.hpp"

namespace upsylon
{

    //! dynamic bit
    struct iobit
    {
        iobit  *next; //!< for list/pool
        iobit  *prev; //!< for list
        bool    code; //!< boolean value

        typedef core::list_of<iobit> list_type; //!< alias
        typedef core::pool_of<iobit> pool_type; //!< alias

        //! compact memory allocation
        inline static iobit * create( bool flag )
        {
            iobit *b = object::acquire1<iobit>();
            b->code = flag;
            return b;
        }

        //! compact memory deallocation
        inline static void destroy( iobit *b ) throw()
        {
            assert(b);
            object::release1(b);
        }

        //! C++ list of bits
        class list : public list_type
        {
        public:
            inline explicit list() throw() : list_type() {} //!< initialize
            inline virtual ~list() throw() { clear(); }     //!< destructor
            inline void     clear() throw() { while(size) destroy( pop_back() ); } //!< special release
        private:
            Y_DISABLE_COPY_AND_ASSIGN(list);
        };

        //! C++ pool of bits
        class pool : public pool_type
        {
        public:
            inline explicit pool() throw() : pool_type() {} //!< initialize
            inline virtual ~pool() throw() { clear(); }     //!< destructor

            inline void clear() throw() { while(size) destroy( query() ); }                    //!< special release
            inline void keep(const size_t nmax) throw() { while(size>nmax) destroy(query()); } //!< keep at most some bits
            inline void reserve(size_t n) { while(n-->0) store(object::acquire1<iobit>()); }   //!< reserve extra bits
        private:
            Y_DISABLE_COPY_AND_ASSIGN(pool);
        };

    };

    //! low level I/O bits FIFO
    class iobits : public iobit::list
    {
    public:
        explicit iobits() throw(); //!< initialize
        virtual ~iobits() throw(); //!< destructor
        
        iobit::pool pool; //!< internal pool of bits

        //! query a new bit
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

        //! push a new bit at the back of the list
        inline void push(const bool flag)   { push_back( query(flag) ); }

        //! peek the bit at the front of the list
        inline bool peek() const throw()    { assert(size>0); return head->code; }

        //! pop the bit at the front of the list
        inline bool pop() throw()
        {
            assert(size>0);
            return pool.store(pop_front())->code;
        }
        
        //! free the current content
        inline void free() throw() { while(size) pool.store( pop_back() ); }


        //! pushing unsigned integrals
        void push(const uint64_t value,
                  const size_t   nbits);

        //! pushing full integral types
        template <typename T>
        inline void push_full( const T value ) { push(value,8*sizeof(T)); }

        //! pop unsigned integral
        template <typename T>
        inline T pop( size_t nbits ) throw()
        {
            assert(nbits<=8*sizeof(T)); assert(nbits<=size);
            T ans = 0;
            while(nbits-->0)
            {
                { (ans <<= 1); }
                if( pop() ) { ans |= 1;}
            }
            return ans;
        }

        //! pop full integral
        template <typename T>
        inline  T pop_full() throw() { return pop<T>(sizeof(T)*8); }

        //! table of individual bits
        static const uint64_t ibits[64];

        static std::ostream & display(std::ostream & os,
                                      const uint64_t value,
                                      const size_t   nbits);

    private:
        Y_DISABLE_COPY_AND_ASSIGN(iobits);
    };

}

#endif

