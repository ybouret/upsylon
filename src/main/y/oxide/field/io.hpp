//! \file
#ifndef Y_OXIDE_FIELD_IO_INCLUDED
#define Y_OXIDE_FIELD_IO_INCLUDED 1

#include "y/oxide/types.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/serializable.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace Oxide
    {
        struct IO
        {
            template <typename T>  static inline
            void SaveIntegral( ios::ostream &fp, const void *addr )
            {
                assert(addr);
                fp.emit<T>(*static_cast<const T*>(addr));
            }

            template <typename T> static inline
            void LoadIntegral( ios::istream &fp, void *addr )
            {
                assert(addr);
                *static_cast<T *>(addr) = fp.read<T>();
            }


            template <typename T> static inline
            void SaveBlock( ios::ostream &fp, const void *addr )
            {
                assert(addr);
                fp.output((const char *)addr,sizeof(T));
            }

            template <typename T> static inline
            void LoadBlock( ios::istream &fp, void *addr )
            {
                assert(addr);
                fp.input(addr,sizeof(T));
            }

            //! assuming serializable conversion exists...
            template <typename T> static inline
            void SaveSerializable( ios::ostream &fp, const void *addr )
            {
                assert(addr);
                const T                 *pObj = static_cast<const T*>(addr);
                const ios::serializable *pSrz = pObj;
                pSrz->serialize(fp);
            }

            //! based on a T::read function
            template <typename T> static inline
            void LoadSerialized( ios::istream &fp, void *addr )
            {
                assert(addr);
                const T tmp = T::read(fp);
                *static_cast<T *>(addr) = tmp;
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

