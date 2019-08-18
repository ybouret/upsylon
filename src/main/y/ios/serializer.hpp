//! \file
#ifndef Y_IOS_SERIALIZER_INCLUDED
#define Y_IOS_SERIALIZER_INCLUDED 1

#include "y/ios/serializable.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"

namespace upsylon
{
    namespace ios
    {
        //! routines to serialise containers...
        struct serializer
        {
            //! save a range
            template <typename ITERATOR> static inline
            size_t save( ios::ostream &fp, size_t n, ITERATOR i )
            {
                size_t total = 0;
                fp.emit_upack(n, &total);
                while(n-->0)
                {
                    total += (*i).serialize(fp);
                    ++i;
                }
                return total;
            }

            //! save a full sequence
            template <typename SEQUENCE> static inline
            size_t save( ios::ostream &fp, const SEQUENCE &seq )
            {
                return save(fp, seq.size(), seq.begin() );
            }

            //! save a range of pointers
            template <typename ITERATOR> static inline
            size_t save_meta( ios::ostream &fp, size_t n, ITERATOR i )
            {
                size_t total = 0;
                fp.emit_upack(n, &total);
                while(n-->0)
                {
                    total += (**i).serialize(fp);
                    ++i;
                }
                return total;
            }

            //! save a sequence of pointers
            template <typename SEQUENCE> static inline
            size_t save_meta( ios::ostream &fp, const SEQUENCE &seq )
            {
                return save_meta(fp, seq.size(), seq.begin() );
            }

            //!load a sequence
            template <typename SEQUENCE, typename LOADER> static inline
            void load( SEQUENCE &seq, ios::istream &fp, LOADER &loader)
            {
                const size_t n = fp.read_upack<size_t>();
                for(size_t i=1;i<=n;++i)
                {
                    typename SEQUENCE::const_type tmp = loader(fp);
                    seq.push_back(tmp);
                }
            }
            
        };
    }
}

#endif

