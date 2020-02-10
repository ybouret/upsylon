//! \file

#ifndef Y_INFORMATION_QBITS_INCLUDED
#define Y_INFORMATION_QBITS_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/randomized/bits.hpp"

#include "y/core/pool.hpp"
#include "y/container/container.hpp"
#include <iosfwd>

namespace upsylon {

    struct qbit
    {
        qbit *next;
        qbit *prev;
        bool  code;
        bool       & operator*()       throw();
        const bool & operator*() const throw();

        static qbit *acquire(const bool);
        static void  release(qbit * &) throw();

        friend std::ostream & operator<<( std::ostream &os, const qbit & );

        class list  : public core::list_of<qbit>
        {
        public:
            explicit  list() throw();
            virtual  ~list() throw();
            void      clear() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(list);
        };


        class pool : public core::pool_of<qbit>
        {
        public:
            explicit pool() throw();
            virtual ~pool() throw();
            void     clear() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pool);
        };

        template <typename T> struct table;

    };

#define Y_QBIT_TABLE(BITS) \
template <> struct qbit::table<uint##BITS##_t> {\
static const uint##BITS##_t bit[BITS];\
}

    Y_QBIT_TABLE(8);
    Y_QBIT_TABLE(16);
    Y_QBIT_TABLE(32);
    Y_QBIT_TABLE(64);

    class qbits :
    public qbit::list,
    public releasable,
    public ios::ostream,
    public ios::istream
    {
    public:
        explicit qbits() throw();
        virtual ~qbits() throw();

        qbit::pool qpool;

        void         free()    throw();
        virtual void release() throw();


        void push( const bool flag );
        bool peek() const throw();
        bool pop()        throw();
        void reserve(size_t);

        template <typename T>
        inline void push_as(T      word,
                            size_t bits)
        {
            static const T      _1(1);
            static const T      _0(0);
            assert(bits<=sizeof(word)*8);
            while(bits-- > 0)
            {
                push( (word&_1) != _0 );
                word>>=1;
            }
        }

        template <typename T>
        inline void push_as(const T word)
        {
            push_as<T>(word,sizeof(word)*8);
        }

        template <typename T>
        inline T pop_as(const size_t bits) throw()
        {
            T              ans(0);
            assert(bits<=size);
            assert(bits<=sizeof(T)*8);
            for(size_t i=0;i<bits;++i)
            {
                if( pop() )
                {
                    ans |= qbit::table<T>::bit[i];
                }
            }
            return ans;
        }

        template <typename T>
        inline T pop_as() throw()
        {
            return pop_as<T>( sizeof(T)*8 );
        }

        void zpad(); //!< fill to byte boundary with zero
        void zpad( randomized::bits &); //!< fill to byte boundary with random
        void drop() throw(); //!< drop to byte boundary

        std::ostream & display( std::ostream &os, const size_t nmax=0) const;

        friend std::ostream & operator<<( std::ostream &os, const qbits & );

        virtual void write( char C ); //!< write byte
        virtual void flush();         //!< zpad

        virtual bool query( char &C ); //!< query next byte if enough bits
        virtual void store( char  C ); //!< store a byte

    private:
        qbit *get(const bool flag);
        Y_DISABLE_COPY_AND_ASSIGN(qbits);
    };


}

#endif

