
#include "y/spade/network/transfer.hpp"
#include "y/spade/layout/tessellation.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/convert.hpp"
#include "y/spade/field/ops.hpp"
#include "y/ios/imstream.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
    
    template <typename COORD> static inline
    void doTest( const string &layout, const size_t ghosts, const size_t cores )
    {
        
        Transfer transfer(comms::homogeneous);
        (void) transfer.query<string>();
        
        typedef typename FieldFor<COORD>::template Of<int>::Type    iField;
        typedef typename iField::Handle                             iFieldHandle;
        typedef typename FieldFor<COORD>::template Of<string>::Type sField;
        typedef typename sField::Handle                             sFieldHandle;
        
        
        
        const COORD   lower = Coord::Ones<COORD>();
        const COORD   upper = Coord::Parse<COORD>(layout);
        const Coord1D ng    = Coord1D(ghosts);
        const Layout<COORD> fullLayout(lower,upper);
        vector<COORD>       mappings;
        fullLayout.findMappings(mappings,cores);
        
        iField iFull("iFull",fullLayout);
        sField sFull("sFull",fullLayout);
        transfer.setup(iFull);
        transfer.setup(sFull);
        
        std::cerr << "mappings=" << mappings << std::endl;
        for(size_t i=1;i<=mappings.size();++i)
        {
            const COORD          &mapping = mappings[i];
            
            std::cerr << "using " << mapping << std::endl;
            typename Layout<COORD>::Loop loop( Coord::Zero<COORD>(), Coord::Ones<COORD>() );
            for(loop.boot();loop.good();loop.next())
            {
                //--------------------------------------------------------------
                // ENTER: PBC loop
                //--------------------------------------------------------------
                
                std::cerr << "\tpbcs=" << loop.value << std::endl;
                std::cerr << "\tcreating partition..." << std::endl;
                const Tessellation<COORD> partition(fullLayout,mapping,loop.value,ng);
                const size_t              size = partition.size;
                vector<_Field>            fields;
                
                slots<iFieldHandle> iFields(size);
                slots<sFieldHandle> sFields(size);
                
                //--------------------------------------------------------------
                // filling
                //--------------------------------------------------------------
                for(size_t rank=0;rank<size;++rank)
                {
                    {
                        const string name = vformat("iField#%u",unsigned(rank));
                        iFieldHandle F    = new iField(name,partition[rank].outer);
                        iFields.push(F);
                        Ops::Ld(*F,*F,int(rank));
                        transfer.setup(*F);
                    }
                    
                    {
                        const string name = vformat("sField#%u",unsigned(rank));
                        sFieldHandle F    = new sField(name,partition[rank].outer);
                        sField      &f    = *F;
                        
                        sFields.push(F);
                        transfer.setup(f);
                        typename Layout<COORD>::Loop it(f.lower,f.upper);
                        std::cerr << "\t" << name  << ":" << f << "/" << fullLayout << std::endl;
                        //Ops::Ld(*F,*F,name);
                        for( it.boot(); it.good(); it.next() )
                        {
                            Y_ASSERT(F->has(*it));
                            //string &tmp = (*F)[ *loop ]; tmp = name;
                        }
                    }
                }
                
                Y_ASSERT(iFields.size()==size);
                Y_ASSERT(sFields.size()==size);
                continue;
                
                // transfer ghosts
                for(size_t rank=0;rank<size;++rank)
                {
                    iField                &iF = *iFields[rank];
                    sField                &sF = *sFields[rank];
                    fields.free();
                    fields << &iF;
                    fields << &sF;
                    
                    const Fragment<COORD> &L  = partition[rank];
                    transfer.localSwap(iF,L);
                    transfer.localSwap(sF,L);
                    transfer.localSwap(fields,L);
                    
                    
                    IOBlock block;
                    for(size_t i=0;i<L.asyncTwoWays.size();++i)
                    {
                        {
                            transfer.asyncSetup(iF);
                            Y_ASSERT(transfer.style==comms::computed_block_size);
                            const AsyncTwoWaysSwaps<COORD> &xch = L.asyncTwoWays[i];
                            transfer.asyncSave(block,iF,xch.forward->innerGhost);
                            transfer.asyncSave(block,iF,xch.reverse->innerGhost);
                            {
                                ios::imstream source(block);
                                transfer.asyncLoad(iF,source,xch.reverse->outerGhost);
                                transfer.asyncLoad(iF,source,xch.forward->outerGhost);
                            }
                            
                        }
                        
                        
                        
                        {
                            transfer.asyncSetup(sF);
                            Y_ASSERT(transfer.style==comms::flexible_block_size);
                            const AsyncTwoWaysSwaps<COORD> &xch = L.asyncTwoWays[i];
                            transfer.asyncSave(block,sF,xch.forward->innerGhost);
                            transfer.asyncSave(block,sF,xch.reverse->innerGhost);
                            {
                                ios::imstream source(block);
                                transfer.asyncLoad(sF,source,xch.reverse->outerGhost);
                                transfer.asyncLoad(sF,source,xch.forward->outerGhost);
                            }
                        }
                        
                        {
                            transfer.asyncSetup(fields);
                            Y_ASSERT(transfer.style==comms::flexible_block_size);
                            const AsyncTwoWaysSwaps<COORD> &xch = L.asyncTwoWays[i];
                            transfer.asyncSave(block, fields, xch.forward->innerGhost );
                            transfer.asyncSave(block, fields, xch.reverse->innerGhost );
                            {
                                ios::imstream source(block);
                                transfer.asyncLoad(fields,source,xch.reverse->outerGhost);
                                transfer.asyncLoad(fields,source,xch.forward->outerGhost);
                            }
                        }
                        
                    }
                }
                
                if(false)
                {
                    // tranfer bulk
                    typename iField::Loop loop;
                    for(size_t rank=0;rank<size;++rank)
                    {
                        iField                &iF = *iFields[rank];
                        sField                &sF = *sFields[rank];
                        fields.free();
                        fields << &iF;
                        fields << &sF;
                        
                        const Fragment<COORD> &frag  = partition[rank];
                        const Layout<COORD>   &bulk  = frag.inner;
                        {
                            IOBlock block;
                            transfer.asyncSetup(iF);
                            transfer.asyncBulkSave(block,iF,bulk,loop);
                            transfer.asyncBulkLoad(iFull,block,bulk,loop);
                        }
                        
                        {
                            IOBlock block;
                            transfer.asyncSetup(sF);
                            transfer.asyncBulkSave(block,sF,bulk,loop);
                            transfer.asyncBulkLoad(sFull,block,bulk,loop);
                        }
                    }
                }
                
                //--------------------------------------------------------------
                // LEAVE: PBC loop
                //--------------------------------------------------------------
                
            }
            
        }
        
    }
    
    
    
}


Y_UTEST(transfer)
{
    string layout     = "10:10:10";
    size_t cores      = 2;
    size_t ghosts     = 1;
    
    if(argc>1) layout = argv[1];
    if(argc>2) cores  = string_convert::to<size_t>(argv[2],"cores");
    if(argc>3) ghosts = string_convert::to<size_t>(argv[3],"ghosts");
    
    doTest<Coord1D>(layout,ghosts,cores);
    //doTest<Coord2D>(layout,ghosts,cores);
    //doTest<Coord3D>(layout,ghosts,cores);
    
}
Y_UTEST_DONE()

