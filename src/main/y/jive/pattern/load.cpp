#include "y/jive/pattern/all.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Jive
    {

        namespace {
            template <typename T> const Pattern * __load( ios::istream &);

            template <> const Pattern *__load<Any>(ios::istream&) { return Any::Create(); }

            template <>
            const Pattern *__load<Single>(ios::istream&fp)
            {
                char C = 0;
                if(!fp.query(C)) throw exception("Jive::Pattern::Load.Single: missing code");
                return Single::Create( uint8_t(C) );
            }

        }

#define Y_PATTERN_LOAD(CLASS) case CLASS::UUID: return __load<CLASS>(fp)

        const Pattern * Pattern:: Load(ios::istream &fp )
        {
            static const char fn[] = "Jive::Pattern::Load: ";
            uint32_t uuid  = 0;
            size_t   shift = 0;
            if( !fp.query_nbo(uuid,shift) )
            {
                throw exception("%scouldn't read uuid",fn);
            }

            switch(uuid)
            {
                    Y_PATTERN_LOAD(Any);
                    Y_PATTERN_LOAD(Single);
                default:
                    break;
            }

            throw exception("%sunknown UUID=<%s>",fn, fourcc_(uuid));
        }
    }
}

