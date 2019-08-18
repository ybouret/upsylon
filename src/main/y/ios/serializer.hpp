//! \file
#ifndef Y_IOS_SERIALIZER_INCLUDED
#define Y_IOS_SERIALIZER_INCLUDED 1

#include "y/ios/serializable.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace ios
    {
        struct serializer
        {
            template <typename ITERATOR> static inline
            size_t write( ios::ostream &fp, size_t n, ITERATOR i )
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

            template <typename SEQUENCE> static inline
            size_t write( ios::ostream &fp, const SEQUENCE &seq )
            {
                return write(fp, seq.size(), seq.begin() );
            }

            template <typename ITERATOR> static inline
            size_t write_meta( ios::ostream &fp, size_t n, ITERATOR i )
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

            template <typename SEQUENCE> static inline
            size_t write_meta( ios::ostream &fp, const SEQUENCE &seq )
            {
                return write_meta(fp, seq.size(), seq.begin() );
            }

        };
    }
}

#endif

