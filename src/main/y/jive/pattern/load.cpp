#include "y/jive/pattern/all.hpp"
#include "y/exception.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Jive
    {

        namespace {
            static const char fn[] = "Jive::Pattern::Load: ";

            template <typename T>  Pattern * __load( ios::istream &);

            //==================================================================
            //
            // Basic
            //
            //==================================================================

            template <> Pattern *__load<Any>(ios::istream&)
            { return Any::Create(); }

            template <>
            Pattern *__load<Single>(ios::istream&fp)
            {
                char C = 0;
                if(!fp.query(C)) throw exception("%sSingle: missing code",fn);
                return Single::Create( uint8_t(C) );
            }

            template <>
            Pattern *__load<Range>(ios::istream&fp)
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
            Pattern *__load<Exclude>(ios::istream&fp)
            {
                char C = 0;
                if(!fp.query(C)) throw exception("%sExclude: missing code",fn);
                return Exclude::Create( uint8_t(C) );
            }

            //==================================================================
            //
            // Logical
            //
            //==================================================================
            template <>
            Pattern *__load<And>(ios::istream &fp)
            {
                auto_ptr<Logical> p = And::Create();
                p->load(fp);
                return p.yield();
            }

            template <>
            Pattern *__load<Or>(ios::istream &fp)
            {
                auto_ptr<Logical> p = Or::Create();
                p->load(fp);
                return p.yield();
            }

            template <>
            Pattern *__load<None>(ios::istream &fp)
            {
                auto_ptr<Logical> p = None::Create();
                p->load(fp);
                return p.yield();
            }

            //==================================================================
            //
            // Joker
            //
            //==================================================================
            template <>
            Pattern * __load<Optional>(ios::istream &fp)
            {
                return Optional::Create( Pattern::Load(fp) );
            }

            template <>
            Pattern * __load<Repeating>(ios::istream &fp)
            {
                size_t shift = 0;
                size_t minCount = 0;
                if( !fp.query_upack(minCount,shift) ) throw exception("%sRepeating: missing minCount",fn);
                return Repeating::Create( Pattern::Load(fp), minCount);
            }


            template <>
            Pattern * __load<Counting>(ios::istream &fp)
            {
                static const char sub[] ="Counting: ";
                size_t shift    = 0;
                size_t minCount = 0;
                size_t maxCount = 0;
                if( !fp.query_upack(minCount,shift) ) throw exception("%s%smissing minCount",fn,sub);
                if( !fp.query_upack(maxCount,shift) ) throw exception("%s%smissing maxCount",fn,sub);
                return Counting::Create( Pattern::Load(fp), minCount, maxCount);
            }

        }

#define Y_PATTERN_LOAD(CLASS) case CLASS::UUID: return __load<CLASS>(fp)

        Pattern * Pattern:: Load(ios::istream &fp )
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

                    Y_PATTERN_LOAD(And);
                    Y_PATTERN_LOAD(Or);
                    Y_PATTERN_LOAD(None);

                    Y_PATTERN_LOAD(Optional);
                    Y_PATTERN_LOAD(Repeating);
                    Y_PATTERN_LOAD(Counting);

                case Repeating::UUID_ZOM: return Repeating::Create( Load(fp), 0 );
                case Repeating::UUID_OOM: return Repeating::Create( Load(fp), 1 );

                default:
                    break;
            }

            throw exception("%sunknown UUID=<%s>",fn, fourcc_(uuid));
        }
    }
}

