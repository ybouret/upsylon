//! \file
#ifndef Y_IMSTREAM_INCLUDED
#define Y_IMSTREAM_INCLUDED 1

#include "y/ios/istream.hpp"
#include "y/memory/buffer.hpp"
namespace upsylon
{
    namespace ios
    {
        //! input memory stream
        class imstream : public istream
        {
        public:
            inline virtual ~imstream() throw() {}

            inline imstream( const void *buf, const size_t len ) throw() :
            curr( static_cast<const char *>(buf) ),
            last( curr+len ),
            init( curr )
            {
                assert(!(0==buf&&len>0));
            }

            inline imstream( const memory::ro_buffer &buf ) throw() :
            curr( static_cast<const char *>(buf.ro()) ),
            last( curr + buf.length() ),
            init( curr )
            {
            }

            inline virtual bool query( char &C )
            {
                if(curr<last)
                {
                    C = *(curr++);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            virtual void store( const char C );

        private:
            const char *curr;
            const char *last;
            const char *init;
            Y_DISABLE_COPY_AND_ASSIGN(imstream);
        };
    }
}

#endif

