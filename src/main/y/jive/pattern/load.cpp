#include "y/jive/pattern/all.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Jive
    {

        namespace {
            static const char fn[] = "Jive::Pattern::Load: ";

            template <typename T> const Pattern * __load( ios::istream &);

            //==================================================================
            //
            // Basic
            //
            //==================================================================

            template <> const Pattern *__load<Any>(ios::istream&)
            { return Any::Create(); }

            template <>
            const Pattern *__load<Single>(ios::istream&fp)
            {
                char C = 0;
                if(!fp.query(C)) throw exception("%sSingle: missing code",fn);
                return Single::Create( uint8_t(C) );
            }

            template <>
            const Pattern *__load<Range>(ios::istream&fp)
            {
                static const char sub[] ="Range: ";
                char C = 0;
                if(!fp.query(C)) throw exception("%s%smissing lower",fn,sub);
                const uint8_t lower = uint8_t(C);
                if(!fp.query(C)) throw exception("%s%smissing upper",fn,sub);
                const uint8_t upper = uint8_t(C);
                if(upper<lower)
                {
                    throw exception("%s%scorrupted!",fn,sub);
                }
                return Range::Create(lower,upper);
            }

            template <>
            const Pattern *__load<Exclude>(ios::istream&fp)
            {
                char C = 0;
                if(!fp.query(C)) throw exception("%sExclude: missing code",fn);
                return Exclude::Create( uint8_t(C) );
            }

        }

#define Y_PATTERN_LOAD(CLASS) case CLASS::UUID: return __load<CLASS>(fp)

        const Pattern * Pattern:: Load(ios::istream &fp )
        {
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
                    Y_PATTERN_LOAD(Range);
                    Y_PATTERN_LOAD(Exclude);
                default:
                    break;
            }

            throw exception("%sunknown UUID=<%s>",fn, fourcc_(uuid));
        }
    }
}

