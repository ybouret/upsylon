
#include "y/memory/tight/twain-allocator.hpp"
#include "y/memory/tight/zcache.hpp"
#include "y/memory/tight/arena.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include "y/memory/marker.hpp"
#include <iostream>
#include <cstring>

namespace upsylon
{
    namespace memory
    {
        namespace tight
        {

            typedef zcache<chunk> zchunks;


            void twain_allocator:: release_A(size_t na) throw()
            {
                assert(NULL!=A);
                arena *a = (arena *)A;
                while(na-- > 0 )
                {
                    self_destruct( a[na] );
                }
                A=NULL;
                aliasing::_(num_arenas) = 0;
            }

            void twain_allocator :: release_W() throw()
            {
                assert(workspace);
                assert(work_exp2);
                assert(NULL==Z);
                vein &V = Q[work_exp2];
                memset(workspace,0,V.block_size);
                V.release(workspace);
                workspace              = 0;
                aliasing::_(work_exp2) = 0;
            }

            void twain_allocator:: release_Z() throw()
            {
                assert(NULL!=Z);
                self_destruct( *(zchunks *)Z );
                Z=NULL;
            }

            twain_allocator:: ~twain_allocator() throw()
            {
                release_A(num_arenas);
                release_Z();
                release_W();
            }

            twain_allocator:: twain_allocator(lockable    &usr_access,
                                              quarry      &usr_quarry,
                                              const size_t usr_chunk_size,
                                              const size_t usr_limit_size) :
            exp2_allocator(usr_access,usr_quarry),
            limit_size(vein::min_size),
            limit_exp2(vein::min_exp2),
            num_arenas(1+limit_exp2),
            A(0),
            Z(0),
            workspace(0),
            work_exp2(0)
            {

                //--------------------------------------------------------------
                // limit size is already at its minimum
                // check usr request
                //--------------------------------------------------------------
                if(usr_limit_size>vein::max_size)
                {
                    // set to the max
                    aliasing::_(limit_size) = vein::max_size;
                    aliasing::_(limit_exp2) = vein::max_exp2;
                    aliasing::_(num_arenas) = quarry::max_veins;

                }
                else
                {
                    // check if bigger than current limit_size
                    if(usr_limit_size>limit_size)
                    {
                        aliasing::_(limit_size) = next_power_of_two(usr_limit_size);
                        aliasing::_(limit_exp2) = integer_log2(limit_size);
                        aliasing::_(num_arenas) = 1+limit_exp2;
                        assert( size_t(1) << limit_exp2 == limit_size );
                    }
                }
                std::cerr << "#arenas=" << num_arenas << std::endl;

                //--------------------------------------------------------------
                //
                // allocating memory for arenas and zchunks
                //
                //--------------------------------------------------------------
                {
                    marker am( num_arenas * sizeof(arena) );
                    marker zm( am, sizeof(zchunks) );
                    const size_t work_size  = next_power_of_two( zm.next_offset() );
                    aliasing:: _(work_exp2) = max_of<size_t>(integer_log2(work_size),vein::min_exp2);
                    std::cerr << "request: " << work_size << "<=2^" << work_exp2 << std::endl;
                    std::cerr << "|_arena: " << am.length << std::endl;
                    std::cerr << "|_zombi: " << zm.length << std::endl;
                    workspace = static_cast<char *>(Q[work_exp2].acquire());
                    A         = workspace + am.offset;
                    Z         = workspace + zm.offset;
                }

                //--------------------------------------------------------------
                //
                // create zchunks
                //
                //--------------------------------------------------------------
                zchunks *zc = (zchunks *)Z; new (zc) zchunks(usr_chunk_size,Q);

                //--------------------------------------------------------------
                //
                // create arenas
                //
                //--------------------------------------------------------------
                size_t na = 0;
                try
                {
                    size_t bs = 1;
                    arena *a  = (arena *)A;
                    while(na<num_arenas)
                    {
                        new ( &a[na] ) arena(bs,usr_chunk_size,zc,Q);
                        ++na;
                        bs <<= 1;
                    }
                }
                catch(...)
                {
                    release_A(na);
                    release_Z();
                    release_W();
                    throw;
                }

            }


            void * twain_allocator:: acquire(size_t &bytes, size_t &shift)
            {

            }

            void  twain_allocator:: release(void *&addr, size_t &bytes, size_t &shift) throw()
            {
                
            }

        }
    }
}

