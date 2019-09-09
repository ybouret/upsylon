#include "y/oxide/workspace.hpp"
#include "y/utest/run.hpp"
#include "y/memory/pooled.hpp"
#include "y/string/tokenizer.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/ovstream.hpp"

#include "support.hpp"

using namespace upsylon;
using namespace Oxide;

namespace
{
    typedef vector<string,memory::pooled> strings;
    static inline bool isSep( const char C ) { return C == ','; }

    template <typename FIELD>
    void display_field( const FIELD &F )
    {
        std::cerr << F.name << " : " << F << std::endl;
    }

    template <typename FIELD> static inline
    void fill(FIELD &F)
    {
        for(size_t i=0;i<F.items/2;++i)
        {
            const typename FIELD::type tmp = support::get<typename FIELD::mutable_type>();

            F( F.rand(alea) ) = tmp;
        }
    }


    template <typename COORD>
    static inline void testWksp(char **argv)
    {

        typedef  typename __Field<COORD,double>::Type  dField;
        typedef  typename __Field<COORD,float>::Type   fField;
        typedef  typename __Field<COORD,string>::Type  sField;

        const COORD   length  = Coord::Parse<COORD>(  argv[1],"length");
        const COORD   pbc     = Coord::Parse<COORD>(  argv[2],"pbc");
        const Coord1D ng      = Coord::Parse<Coord1D>(argv[3],"ng");
        const COORD   mapping = Coord::Parse<COORD>(  argv[4],"mapping");

        COORD org(0); Coord::LD(org,1);
        const Layout<COORD> full(org,length);
        std::cerr << "full=" << full << std::endl;


        ios::ovstream block;

        const Coord1D size = Coord::Product(mapping);
        for(Coord1D   rank=0;rank<size;++rank)
        {
            std::cerr << "tile[" << rank << "]" << std::endl;

            Workspace<COORD> wksp(full,mapping,rank,pbc,ng);
            wksp.display(std::cerr, "\t(*) ");

            {
                dField &Fd = wksp.template create<dField>( "Fd" );
                fField &Ff = wksp.template create<fField>( "Ff" );
                sField &Fs = wksp.template create<sField>( "Fs" );

                display_field(Fd);
                display_field(Ff);
                display_field(Fs);

                fill(Fd);
                fill(Ff);
                fill(Fs);

                Y_CHECK( wksp.owns(Fd) );
                Y_CHECK( wksp.owns(Ff) );
                Y_CHECK( wksp.owns(Fs) );

                wksp.localExchange(Fd);
                wksp.localExchange(Ff);
                wksp.localExchange(Fs);

                ios::plugin_raw<double> dplg;

                size_t total = 0;
                for(size_t i=0;i<wksp.Orientations;++i)
                {
                    total += wksp.asyncSave(Connectivity::Forward,i,block,Fd, dplg.save );
                    total += wksp.asyncSave(Connectivity::Reverse,i,block,Fd, dplg.save );
                }
                std::cerr << "all_save=" << total << std::endl;


            }

            {
                display_field( wksp.template as<dField>("Fd") );
                display_field( wksp.template as<fField>("Ff") );
                display_field( wksp.template as<sField>("Fs") );
            }


        }


    }

}

Y_UTEST(oxide_wksp)
{
    // length pbc ng mapping
    if(argc<=4)
    {
        std::cerr << "Usage: " << program << " length pbc ng mapping" << std::endl;
        return 0;
    }


    strings      length(3,as_capacity);
    const size_t dimensions = tokenizer<char>::split(length, argv[1], isSep);


    switch(dimensions)
    {
        case 1: testWksp<Coord1D>(argv); break;
        case 2: testWksp<Coord2D>(argv); break;
        case 3: testWksp<Coord3D>(argv); break;
        default: break;
    }

}
Y_UTEST_DONE()
