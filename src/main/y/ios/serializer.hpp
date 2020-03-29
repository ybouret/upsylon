//! \file
#ifndef Y_IOS_SERIALIZER_INCLUDED
#define Y_IOS_SERIALIZER_INCLUDED 1

#include "y/ios/serializable.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/exception.hpp"

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
                size_t total = fp.write_upack(n);
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
                size_t total = fp.write_upack(n);
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
            void load( SEQUENCE &seq, ios::istream &fp, LOADER &loader, size_t &shift, const char *which)
            {
                assert(which);
                size_t n     = 0;
                if(!fp.query_upack(n,shift) )
                {
                    throw exception("serializer::load(missing #entries for '%s')",which);
                }
                
                for(unsigned i=1;n>0;--n,++i)
                {
                    size_t       nl     = 0;
                    typename SEQUENCE::const_type tmp = loader(fp,nl,which);
                    seq.push_back(tmp);
                    shift += nl;
                }
            }
            
        };
    }
}

#endif

