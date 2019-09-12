#include "y/oxide/workspace.hpp"
#include "y/utest/run.hpp"
#include "y/memory/pooled.hpp"
#include "y/string/tokenizer.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"

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

                Y_ASSERT( wksp.owns(Fd) );
                Y_ASSERT( wksp.owns(Ff) );
                Y_ASSERT( wksp.owns(Fs) );

                wksp.localExchange(Fd);
                wksp.localExchange(Ff);
                wksp.localExchange(Fs);

                ActiveFields pick;
                pick( wksp, "Fd" );
                pick( wksp, "Fs;Ff" );
                pick( wksp, "Fs;Ff" );

                std::cerr << "<LocalExchanges>" << std::endl;
                wksp.localExchange(pick);
                std::cerr << "<LocalExchanges/>" << std::endl;

                std::cerr << "<AsyncExchanges>" << std::endl;
                std::cerr << "\tcom: ";
                for(size_t i=0;i<wksp.Orientations;++i)
                {
                    typename Workspace<COORD>::asyncIO aio;
                    std::cerr << "<@" << i;
                    if( wksp.asyncProlog(aio, pick, Conn::Forward, i) )
                    {
                        std::cerr << "+" << GhostsComm::ToText(aio.comm);
                        wksp.recvBlock.copy(wksp.sendBlock);
                        wksp.asyncEpilog(aio,pick);
                    }

                    if( wksp.asyncProlog(aio, pick, Conn::Reverse, i) )
                    {
                        std::cerr << "-" << GhostsComm::ToText(aio.comm);
                        wksp.recvBlock.copy(wksp.sendBlock);
                        wksp.asyncEpilog(aio,pick);
                    }
                    std::cerr << ">";
                } std::cerr << std::endl;
                std::cerr << "<AsyncExchanges/>" << std::endl;


            }


            {
                display_field( wksp.template as<dField>("Fd") );
                display_field( wksp.template as<fField>("Ff") );
                display_field( wksp.template as<sField>("Fs") );
            }


        }


    }

}

#include "y/oxide/field/io.hpp"

Y_UTEST(oxide_wksp)
{

    IO &OxideIO = IO::instance();

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
    std::cerr << "OxideIO.size()   =" << OxideIO.size()    << std::endl;

    if(false)
    {
        for(IO::iterator it = OxideIO.begin(); it != OxideIO.end(); ++it )
        {
            std::cerr << "io for <" << it->key().name() << ">" << std::endl;
        }
    }


}
Y_UTEST_DONE()
