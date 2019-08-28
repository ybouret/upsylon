//! \file
#ifndef Y_IOS_OSTREAM_INCLUDED
#define Y_IOS_OSTREAM_INCLUDED 1

#include "y/strfwd.hpp"
#include "y/ios/stream.hpp"
#include "y/os/endian.hpp"
#include "y/os/static-check.hpp"
#include "y/os/printf-check.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon
{
    namespace ios
    {
        //! add value to addr if addr is valid
        template <typename T, typename U>
        inline void __add_to( T *addr, const U value ) throw()
        {
            if(addr) (*addr) += value;
        }

        //! right shift 8 bits sizeof(T)>1
        template <typename T>
        inline void __shr8( T &x, int2type<true> ) throw()
        {
            assert(sizeof(T)>1);
            x >>= 8;
        }

        //! right shift 8 bits sizeof(T)<=1
        template <typename T>
        inline void __shr8( T &x, int2type<false> ) throw()
        {
            assert(sizeof(T)<=1);
            x=0;
        }

        //! interface for output streams
        class ostream : public stream
        {
        public:
            //! destructor
            virtual ~ostream() throw();

            //! write one char
            virtual void write( char C ) = 0;

            //! flush
            virtual void flush()         = 0;

            //! printf style, may be superseeded
            virtual ostream & operator()(const char *fmt,...) Y_PRINTF_CHECK(2,3);


            //! output large buffer
            inline void output(const char *buffer, const size_t buflen)
            {
                assert(!(buffer==0&&buflen>0));
                for(size_t i=0;i<buflen;++i) write(buffer[i]);
            }
            
            //! flux for one char
            inline ostream & operator<<( const char C )
            {
                write(C);
                return *this;
            }
            
            //! output text
            ostream & operator<<( const char *buffer );


            //! output buffer
            inline ostream & operator<<(const memory::ro_buffer &buffer)
            {
                output( static_cast<const char *>(buffer.ro()), buffer.length() );
                return *this;
            }

            //! output binary address
            ostream & viz( const void *addr );

            //! emit integral types
            template <typename T>
            ostream & emit(T x)
            {
                x = swap_be_as<T>(x);
                output((const char*)&x,sizeof(T));
                return *this;
            }

            //! emit compact unsigned
            template <typename T>
            ostream & emit_upack(T x, size_t *shift=NULL)
            {
                //______________________________________________________________
                //
                // check value metrics
                //______________________________________________________________
                Y_STATIC_CHECK(sizeof(T)<=8,T_is_too_large);
                const size_t  num_bits     = bits_for(x); assert(num_bits<=64);
                const uint8_t last4shifted = uint8_t(x&T(0x0f)) << 4;
                if(num_bits<=4)
                {
                    //__________________________________________________________
                    //
                    // 0 extra bytes!
                    //__________________________________________________________
                    write(last4shifted);
                    if(shift) *shift = 1;
                }
                else
                {
                    //__________________________________________________________
                    //
                    // some extra bits/byte(s)
                    //__________________________________________________________
                    const size_t extra_bits  = num_bits - 4;
                    size_t       extra_bytes = Y_ROUND8(extra_bits)>>3; assert(extra_bytes<=8);
                    write( char(last4shifted | extra_bytes) );
                    if(shift) *shift = 1;
                    x >>= 4;
                    while(extra_bytes-->0)
                    {
                        const uint8_t B = uint8_t(x&T(0xff));
                        write(B);
                        __shr8<T>(x, int2type< (sizeof(T)>1) >() );
                        if(shift) ++(*shift);
                    }
                    assert(0==x);
                }

                return *this;
            }

            //! repeat a char
            inline ostream & repeat( size_t sz, const char C)
            {
                while(sz-->0) write(C);
                return *this;
            }

            //! pad something for alignement
            inline ostream & align(const char  *buffer,
                                   const size_t buflen,
                                   const size_t aligned, const char C=' ')
            {
                assert(!(0==buffer&&buflen>0));
                for(size_t i=0;i<buflen;++i)       write(buffer[i]);
                for(size_t i=buflen;i<aligned;++i) write(C);
                return *this;
            }

            //! pad text
            ostream & align(const char *text,const size_t aligned,const char C=' ');

            //! pad text
            ostream & align(const string &s,const size_t aligned,const char C=' ');

            //! emit a binary block, return written bytes
            size_t emit_block( const void *data, const size_t size );

            //! emit a binary block, return written bytes
            size_t emit_block( const memory::ro_buffer  &buff );

            //! emit a binary text
            size_t emit_block( const char *text );

        protected:
            //! constructor
            explicit ostream() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ostream);
        };
    }
}

#endif
