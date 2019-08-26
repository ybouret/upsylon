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
        //! I/O on objects
        struct IO
        {
            //! assuming integral type
            template <typename T>  static inline
            void SaveIntegral( ios::ostream &fp, const void *addr )
            {
                assert(addr);
                fp.emit<T>(*static_cast<const T*>(addr));
            }

            //! reload from integral type
            template <typename T> static inline
            void LoadIntegral( ios::istream &fp, void *addr )
            {
                assert(addr);
                *static_cast<T *>(addr) = fp.read<T>();
            }

            //! assuming portable binary block
            template <typename T> static inline
            void SaveBlock( ios::ostream &fp, const void *addr )
            {
                assert(addr);
                fp.output((const char *)addr,sizeof(T));
            }

            //! reloading portable binary block
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

            //! base type for derived type
            typedef vector<uint8_t,memory::global> Bytes;

            //! variable length block type, usable with ios::imstream for input
            class Block : public Bytes, public ios::ostream
            {
            public:
                //! prepare with n bytes as capacity
                explicit Block( const size_t n);
                //! destruct
                virtual ~Block() throw();

                virtual void write( char C ); //!< append C and adjuste size
                virtual void flush();         //!< do nothing
                void *format(const size_t n); //!< set size to n zeroed bytes

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Block);
            };
        };

    }
}

#endif

