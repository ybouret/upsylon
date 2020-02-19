//! \file

#ifndef Y_INFORMATION_QBITS_INCLUDED
#define Y_INFORMATION_QBITS_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/randomized/bits.hpp"
#include "y/sequence/list.hpp"

namespace upsylon {

     //! Queue Bit
    struct qbit
    {
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

    typedef list<bool> qbits_; //!< base class for qbits

    //! Queue of Bits
    class qbits : public qbits_, public ios::ostream, public ios::istream
    {
    public:
        explicit qbits() throw(); //!< setup
        virtual ~qbits() throw(); //!< cleanup

        

        bool pop() throw();    //!< pop front bit

        //! push partial, integral unsigned type
        template <typename T>
        inline void _push(T      word,
                          size_t bits)
        {
            static const T      _1(1);
            static const T      _0(0);
            assert(bits<=sizeof(word)*8);
            while(bits-- > 0)
            {
                push_back( (word&_1) != _0 );
                word>>=1;
            }
        }

        //! push full unsigned integral type
        template <typename T>
        inline void _push(const T word)
        {
            _push<T>(word,sizeof(word)*8);
        }

        //! push any integral type
        template <typename T>
        inline void push(const T     &word,
                         const size_t bits)
        {
            typename unsigned_int< sizeof(T) >::type w(word);
            _push<typename unsigned_int< sizeof(T) >::type>(w,bits);
        }

        //! push a full integral type
        template <typename T>
        inline void push(const T &word)
        {
            push<T>(word,sizeof(T)*8);
        }

        //! pop partial, unsigned integral type
        template <typename T>
        inline T _pop(const size_t bits) throw()
        {
            T              ans(0);
            assert(bits<=size());
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
        inline T _pop() throw()
        {
            return _pop<T>(sizeof(T)*8);
        }

        //! pop any integral type
        template <typename T>
        inline T pop(const size_t bits) throw()
        {
            typename unsigned_int< sizeof(T) >::type w = _pop<typename unsigned_int< sizeof(T) >::type>(bits);
            return T(w);
        }

        //! pop a full integral type
        template <typename T>
        inline T pop() throw()
        {
            return pop<T>(sizeof(T)*8);
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

        void compile( sequence<char> & ); //!< write all possible chars
        void zfinish( sequence<char> & ); //!< zpad and write possible chars


    private:
        Y_DISABLE_COPY_AND_ASSIGN(qbits);
    };


}

#endif

