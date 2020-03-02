//! \file

#ifndef Y_INFORMATION_QBITS_INCLUDED
#define Y_INFORMATION_QBITS_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/randomized/bits.hpp"
#include "y/sequence/list.hpp"

namespace upsylon {

    namespace Information {

        template <typename T> struct TableOf;

        //! declare table of bits
#define Y_INFO_TABLE(BITS) template <> struct TableOf<uint##BITS##_t> {\
static const uint##BITS##_t Bit[BITS];\
}

        Y_INFO_TABLE(8);  //!< for 8 bits
        Y_INFO_TABLE(16); //!< for 16 bits
        Y_INFO_TABLE(32); //!< for 32 bits
        Y_INFO_TABLE(64); //!< for 64 bits

        typedef list<bool> IOBits_;//!< base class for IOBits


        //! Queue of Bits
        class IOBits : public IOBits_, public ios::ostream, public ios::istream
        {
        public:
            explicit IOBits() throw();             //!< setup
            explicit IOBits(const size_t) throw(); //!< setup with capacity
            virtual ~IOBits() throw();             //!< cleanup

            bool pop() throw();    //!< pop front bit

            //! push partial, integral unsigned type
            template <typename T>
            inline void _push(T word,size_t bits)
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
                        ans |= TableOf<T>::Bit[i];
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
            friend std::ostream & operator<<( std::ostream &os, const IOBits & );  //!< display

            virtual void write( char C ); //!< write byte
            virtual void flush();         //!< zpad

            virtual bool query( char &C ); //!< query next byte if enough bits
            virtual void store( char  C ); //!< store a byte

            void compile( sequence<char> & ); //!< write all possible chars
            void zfinish( sequence<char> & ); //!< zpad and write possible chars

            //! base class for I/O on bits
            class Broker : public virtual counted_object
            {
            public:
                virtual ~Broker() throw();                     //!< cleanup
                virtual void        startBits()   throw() = 0; //!< start all up
                virtual const char *label() const throw() = 0; //!< label

            protected:
                explicit Broker() throw(); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Broker);
            };

        private:
            Y_DISABLE_COPY_AND_ASSIGN(IOBits);
        };

        //! helper to make labels
#define Y_INFORMATION_BROKER(NAME) \
const char * NAME :: label() const throw() { return CLID; }\
const char   NAME :: CLID[] = #NAME

    }
}

#endif

