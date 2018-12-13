
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/joker.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {

        static inline Pattern *__load( Logical *l, ios::istream &fp )
        {
            auto_ptr<Logical> q = l;
            const size_t      n = fp.read_upack<size_t>();
            for(size_t i=1;i<=n;++i)
            {
                q->add( Pattern::Load(fp) );
            }
            return q.yield();
        }


        Pattern * Pattern::Load( ios::istream &fp )
        {
            const uint32_t which = fp.read<uint32_t>();
            switch(which)
            {
                    //__________________________________________________________
                    //
                    // basics
                    //__________________________________________________________
                case Any1  ::UUID : return new Any1();
                case Single::UUID : return new Single( fp.read<uint8_t>() );
                case Range ::UUID : {
                    const uint8_t lo = fp.read<uint8_t>();
                    const uint8_t hi = fp.read<uint8_t>();
                    return new Range(lo,hi); }

                    //__________________________________________________________
                    //
                    // logicals
                    //__________________________________________________________
                case AND::  UUID: return __load( new AND(),  fp);
                case OR ::  UUID: return __load( new OR(),   fp);
                case NONE:: UUID: return __load( new NONE(), fp );

                    //__________________________________________________________
                    //
                    // jokers
                    //__________________________________________________________
                case Optional :: UUID: return Optional::Create( Pattern::Load(fp) );
                case Repeating:: UUID: {
                    const size_t nmin = fp.read_upack<size_t>();
                    return Repeating::Create(Pattern::Load(fp),nmin); }
                case Counting:: UUID: {
                    const size_t nmin = fp.read_upack<size_t>();
                    const size_t nmax = fp.read_upack<size_t>();
                    return Counting::Create(Pattern::Load(fp),nmin,nmax); }

                default:
                    break;
            }
            throw exception("Pattern::Load(unknown UUID=0x%08x)", unsigned(which) );
        }

    }

}

