
#include "y/oxide/layouts.hpp"
#include "y/utest/run.hpp"
#include "y/memory/pooled.hpp"
#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace Oxide;

typedef vector<string,memory::pooled> strings;

namespace
{
    static inline
    Coord1D strings1D( const strings &s, const char *id)
    {
        assert(1==s.size());
        return string_convert::to<unit_t>(s[1],id);
    }

    static inline
    Coord2D strings2D( const strings &s, const char *id)
    {
        assert(2==s.size());
        return Coord2D(string_convert::to<unit_t>(s[1],id), string_convert::to<unit_t>(s[2],id) );
    }

    static inline
    Coord3D strings3D( const strings &s, const char *id)
    {
        assert(2==s.size());
        return Coord3D(string_convert::to<unit_t>(s[1],id),
                       string_convert::to<unit_t>(s[2],id),
                       string_convert::to<unit_t>(s[2],id) );
    }

    template <typename COORD>
    static inline void do_wkps(const COORD  length,
                               const COORD  pbc,
                               const size_t ng,
                               const COORD  mapping)
    {
        COORD               org(0); Coord::LD(org,1);
        const Layout<COORD> full(org,length);

        const Coord1D size = Coord::Product(mapping);
        for(Coord1D   rank = 0; rank<size; ++rank)
        {
            Layouts<COORD>(full,mapping,rank,pbc,ng);
        }
    }

}

static inline bool isSep( const char C ) { return C == ','; }

Y_UTEST(oxide_layouts)
{
    // length pbc ng mapping
    if(argc<=4)
    {
        std::cerr << "Usage: " << program << " length pbc ng mapping" << std::endl;
        return 0;
    }


    strings      length(3,as_capacity);
    const size_t dimensions = tokenizer<char>::split(length, argv[1], isSep);

    strings      pbc(3,as_capacity);
    if( dimensions != tokenizer<char>::split(pbc,argv[2],isSep) )
    {
        throw exception("#pbc mismatch #length");
    }

    const size_t ng = string_convert::to<size_t>(argv[3],"ng");

    strings     mapping(3,as_capacity);
    if( dimensions != tokenizer<char>::split(mapping,argv[4],isSep) )
    {
        throw exception("#mapping mismatch #length");
    }

    switch( dimensions )
    {
        case 1: do_wkps( strings1D(length,"length"), strings1D(pbc, "pbc"), ng, strings1D(mapping,"mapping") ); break;
        case 2: do_wkps( strings2D(length,"length"), strings2D(pbc, "pbc"), ng, strings2D(mapping,"mapping") ); break;
        case 3: do_wkps( strings3D(length,"length"), strings3D(pbc, "pbc"), ng, strings3D(mapping,"mapping") ); break;
        default:
        break;
    }




}
Y_UTEST_DONE()

