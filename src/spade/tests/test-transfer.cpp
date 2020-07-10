
#include "y/spade/network/transfer.hpp"
#include "y/spade/layout/tessellation.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/convert.hpp"
#include "y/spade/field/ops.hpp"
#include "y/ios/imstream.hpp"
#include "support.hpp"

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
                    const Layout<COORD> &outer = partition[rank].outer;

                    {
                        const string name = vformat("iField#%u",unsigned(rank));
                        iFieldHandle F    = new iField(name,outer);
                        iFields.push(F);
                        Ops::Ld(*F,*F,int(rank));
                        transfer.setup(*F);
                    }
                    
                    {
                        const string name = vformat("sField#%u",unsigned(rank));
                        sFieldHandle F    = new sField(name,outer);
                        sField      &f    = *F;
                        
                        sFields.push(F);
                        transfer.setup(f);
                        std::cerr << "\t" << name  << ":" << f << std::endl;
                        Ops::Ld(f,f,name);
                    }
                    
                    typename Layout<COORD>::Loop it(outer.lower,outer.upper);

                    for( it.boot(); it.good(); it.next() )
                    {
                        (*iFields.back())[ *it ] = support::get<int>();
                        (*sFields.back())[ *it ] = support::get<string>();
                    }
                }
                
                Y_ASSERT(iFields.size()==size);
                Y_ASSERT(sFields.size()==size);
                
                //--------------------------------------------------------------
                // transfer ghosts
                //--------------------------------------------------------------
                std::cerr << "\ttransfer ghosts" << std::endl;
                IOBlock block;
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
                    
                    
                    for(size_t i=0;i<L.asyncTwoWays.size();++i)
                    {
                        
                        {
                            block.free();
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
                            block.free();
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
                            block.free();
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
                
                std::cerr << "\ttransfer bulks" << std::endl;
                {
                    // tranfer bulk
                    typename iField::Loop it;
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
                            block.free();
                            transfer.asyncSetup(iF);
                            transfer.asyncBulkSave(block,iF,bulk,it);
                            transfer.asyncBulkLoad(iFull,block,bulk,it);
                        }
                        
                        {
                            block.free();
                            transfer.asyncSetup(sF);
                            transfer.asyncBulkSave(block,sF,bulk,it);
                            transfer.asyncBulkLoad(sFull,block,bulk,it);
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
    Coord::DispWidth  = 2;
    string layout     = "10:10:10";
    size_t cores      = 4;
    size_t ghosts     = 2;
    
    if(argc>1) layout = argv[1];
    if(argc>2) cores  = string_convert::to<size_t>(argv[2],"cores");
    if(argc>3) ghosts = string_convert::to<size_t>(argv[3],"ghosts");
    
    for(size_t nc=1;nc<=cores;++nc)
    {
        for(size_t ng=0;ng<=ghosts;++ng)
        {
            doTest<Coord1D>(layout,ng,nc);
            doTest<Coord2D>(layout,ng,nc);
            doTest<Coord3D>(layout,ng,nc);
        }
    }
    
}
Y_UTEST_DONE()

