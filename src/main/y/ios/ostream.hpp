//! \file
#ifndef Y_IOS_OSTREAM_INCLUDED
#define Y_IOS_OSTREAM_INCLUDED 1

#include "y/string.hpp"
#include "y/ios/stream.hpp"
#include "y/os/endian.hpp"
#include "y/os/static-check.hpp"

namespace upsylon
{
    namespace ios
    {
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
            virtual void operator()(const char *fmt,...) Y_PRINTF_CHECK(2,3);


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
            inline ostream & operator<<( const char *buffer )
            {
                output(buffer,length_of(buffer));
                return *this;
            }

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
            ostream & emit_upack(T x)
            {
                Y_STATIC_CHECK(sizeof(T)<=8,T_is_too_large);
                const size_t  num_bits     = bits_for(x); assert(num_bits<=64);
                const uint8_t last4shifted = uint8_t(x&T(0x0f)) << 4;
                std::cerr << "emit_upack " << x << ", #bits=" << num_bits << "|";
                if(num_bits<=4)
                {
                    // 0 extra bytes!
                    std::cerr << "prolog=" << int(last4shifted) << std::endl;
                    write(last4shifted);
                }
                else
                {
                    const size_t extra_bits  = num_bits - 4;
                    size_t       extra_bytes = Y_ROUND8(extra_bits)>>3; assert(extra_bytes<=8);
                    std::cerr << "+" << extra_bytes << ",prolog=" << int(last4shifted | extra_bytes);
                    write( last4shifted | extra_bytes );
                    x >>= 4;
                    while(extra_bytes-->0)
                    {
                        const uint8_t B = uint8_t(x&T(0xff)); std::cerr << ":" << int(B);
                        write(B);
                        x >>= 8;
                    }
                    assert(0==x);
                    std::cerr << std::endl;
                }

                return *this;
            }

        protected:
            //! constructor
            explicit ostream() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ostream);
        };
    }
}

#endif
