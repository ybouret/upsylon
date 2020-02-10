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

    //! Queue Bit
    struct qbit
    {
        qbit *next; //!< for list
        qbit *prev; //!< for list
        bool  code; //!< boolean value

        bool       & operator*()       throw(); //!< access
        const bool & operator*() const throw(); //!< const access

        static qbit *acquire(const bool);         //!< small object
        static void  release(qbit * &) throw();   //!< small object


        friend std::ostream & operator<<( std::ostream &os, const qbit & ); //!< display

        //! list management
        class list  : public core::list_of<qbit>
        {
        public:
            explicit  list() throw();  //!< setup
            virtual  ~list() throw();  //!< cleanup
            void      clear() throw(); //!< release all

        private:
            Y_DISABLE_COPY_AND_ASSIGN(list);
        };

        //! pool managerment
        class pool : public core::pool_of<qbit>
        {
        public:
            explicit pool() throw();  //!< setup
            virtual ~pool() throw();  //!< cleanup
            void     clear() throw(); //!< release all

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pool);
        };

        template <typename T> struct table; //!< table of bits declaration

    };

    //! declare table for specific bits
#define Y_QBIT_TABLE(BITS) \
template <> struct qbit::table<uint##BITS##_t> {\
static const uint##BITS##_t bit[BITS];\
}

    Y_QBIT_TABLE(8);  //!< for 8 bits
    Y_QBIT_TABLE(16); //!< for 16 bits
    Y_QBIT_TABLE(32); //!< for 32 bits
    Y_QBIT_TABLE(64); //!< for 64 bits

    //! Queue of Bits
    class qbits : public qbit::list, public ios::ostream, public ios::istream
    {
    public:
        explicit qbits() throw(); //!< setup
        virtual ~qbits() throw(); //!< cleanup

        qbit::pool qpool;         //!< pool of bits

        void free()    throw();   //!< empty list into pool
        void release() throw();   //!< release all memory


        void push( const bool flag ); //!< push a new bit
        bool peek() const throw();    //!< peek front bit
        bool pop()        throw();    //!< pop front bit
        void reserve(size_t);         //!< store bits into pool

        //! push partial, integral unsigned type
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

        //! push full unsigned integral type
        template <typename T>
        inline void push_as(const T word)
        {
            push_as<T>(word,sizeof(word)*8);
        }

        //! pop partial, unsigned integral type
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

        //! pop full unsigned integral type
        template <typename T>
        inline T pop_as() throw()
        {
            return pop_as<T>( sizeof(T)*8 );
        }

        void zpad();                    //!< fill to byte boundary with zero
        void zpad( randomized::bits &); //!< fill to byte boundary with random
        void drop() throw();            //!< drop to byte boundary

        std::ostream & display( std::ostream &os, const size_t nmax=0) const; //!< display
        friend std::ostream & operator<<( std::ostream &os, const qbits & );  //!< display

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

