//! \file
#ifndef Y_OXIDE_FIELD_IO_INCLUDED
#define Y_OXIDE_FIELD_IO_INCLUDED 1

#include "y/oxide/types.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/serializable.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace Oxide
    {
        struct IO
        {
            template <typename T>
            static void SaveIntegral( ios::ostream &fp, const void *addr )
            {
                assert(addr);
                fp.emit<T>(*static_cast<const T*>(addr));
            }

            template <typename T>
            static void SaveBlock( ios::ostream &fp, const void *addr )
            {
                const T *ptr = static_cast<const T *>(addr);
                for(size_t i=sizeof(T);i>0;--i,++ptr)
                {
                    fp.write(*ptr);
                }
            }

            template <typename T>
            static void SaveSerializable( ios::ostream &fp, const void *addr )
            {
                assert(addr);
                const T                 *pObj = static_cast<const T*>(addr);
                const ios::serializable *pSrz = pObj;
                pSrz->serialize(fp);
            }

            typedef vector<uint8_t,memory::global> Bytes;

            class Block : public Bytes, public ios::ostream
            {
            public:
                explicit Block( const size_t n);
                virtual ~Block() throw();

                virtual void write( char C );
                virtual void flush();
                


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Block);
            };
        };

    }
}

#endif

