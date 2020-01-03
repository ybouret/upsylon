
#include "y/graphic/image/pixtags.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {

        PixTag_:: ~PixTag_() throw()
        {
        }

        PixTag_:: PixTag_(const std::type_info &tid_,
                          const char           *tag_ ) :
        tid( tid_ ),
        tag( tag_ )
        {
        }

        const std::type_info & PixTag_:: key() const throw()
        {
            return tid;
        }


        PixTags:: ~PixTags() throw()
        {
        }

        PixTags:: PixTags() : PixTags_(8,as_capacity)
        {
            create<uint8_t>( "1" );
            create<float>  ( "F" );
            create<rgb>    ( "3" );
            create<rgba>   ( "4" );

        }

        void PixTags:: create( const PixTag &p )
        {
            if( !insert(p) )
            {
                throw exception("PixTags: multiple '%s", p->tid.name() );
            }
        }

        const string & PixTags:: of(const std::type_info &tid) const
        {
            const PixTag *pp = search(tid);
            if(!pp) throw exception("PixTags: no '%s'", tid.name() );
            return ( (**pp).tag );
        }


    }
}
