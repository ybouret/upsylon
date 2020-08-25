
#include "y/memory/tight/twain-allocator.hpp"
#include "y/memory/tight/zcache.hpp"
#include "y/memory/tight/arena.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include "y/memory/marker.hpp"
#include "y/exceptions.hpp"

#include <cerrno>
#include <iostream>
#include <cstring>

namespace upsylon
{
    namespace memory
    {
        namespace tight
        {

            typedef zcache<chunk> zchunks;


            void twain_allocator:: gc() throw()
            {
                ((zchunks *)Z)->gc();
            }


            void twain_allocator:: release_A(size_t na) throw()
            {
                assert(NULL!=A);
                arena *a = (arena *)A;
                while(na-- > 0 )
                {
                    self_destruct( a[lower_exp2+na] );
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
                                              const size_t usr_lower_size,
                                              const size_t usr_limit_size) :
            exp2_allocator(usr_access,usr_quarry),
            lower_size(1),
            lower_exp2(0),
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
                        assert( size_t(1) << limit_exp2 == limit_size );
                    }
                }

                {
                    aliasing::_(lower_size) = next_power_of_two( clamp<size_t>(1,usr_lower_size,limit_size) );
                    aliasing::_(lower_exp2) = integer_log2(lower_size);
                    assert(lower_size>0);
                    assert(lower_size<=limit_size);
                    assert( size_t(1) << lower_exp2 == lower_size);
                    assert(limit_exp2>=lower_exp2);
                }
                aliasing::_(num_arenas) = 1+limit_exp2-lower_exp2;


                std::cerr << "#arenas=" << num_arenas << " from " << lower_size << "=2^" << lower_exp2 << " to " << limit_size << "=2^" << limit_exp2 << std::endl;

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
                    A         = (workspace + am.offset) - lower_exp2 * sizeof(arena);
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
                    size_t bs = lower_size;
                    arena *a  = static_cast<arena*>(A) + lower_exp2;
                    while(na<num_arenas)
                    {
                        new (a) arena(bs,usr_chunk_size,zc,Q);
                        assert(a->block_size==bs);
                        ++na;
                        ++a;
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
                try
                {
                    //----------------------------------------------------------
                    // check bytes
                    //----------------------------------------------------------
                    bytes = max_of<size_t>(1,bytes);
                    if(bytes>vein::max_size)
                    {
                        throw libc::exception(EDOM,"twain_allocator overflow");
                    }

                    //----------------------------------------------------------
                    // compute parameters
                    //----------------------------------------------------------
                    {
                        size_t n = lower_size;
                        size_t s = lower_exp2;
                        while(n<bytes)
                        {
                            n <<= 1;
                            ++s;
                        }

                        bytes = n;
                        shift = s;
                    }

                    //----------------------------------------------------------
                    // acquire memory
                    //----------------------------------------------------------
                    if(shift<=limit_exp2)
                    {
                        //------------------------------------------------------
                        // use arena
                        //------------------------------------------------------
                        arena *a = static_cast<arena *>(A) + shift;
                        assert(a->block_size==bytes);
                        return a->acquire();
                    }
                    else
                    {
                        //------------------------------------------------------
                        // use quarry
                        //------------------------------------------------------
                        vein &V = Q[shift]; assert(V.block_size==bytes);
                        void *p = V.acquire();
                        memset(p,0,bytes);
                        return p;
                    }


                }
                catch(...)
                {
                    bytes=0;
                    shift=0;
                    throw;
                }
            }

            void  twain_allocator:: release(void *&addr, size_t &bytes, size_t &shift) throw()
            {
                assert(addr);
                assert(bytes>=lower_size);
                assert(size_t(1)<<shift==bytes);
                if(shift<=limit_exp2)
                {
                    //------------------------------------------------------
                    // use arena
                    //------------------------------------------------------
                    arena *a = static_cast<arena *>(A) + shift;
                    assert(a->block_size==bytes);
                    a->release(addr);
                }
                else
                {
                    vein &V = Q[shift]; assert(V.block_size==bytes);
                    V.release(addr);
                }
                addr  = 0;
                bytes = 0;
                shift = 0;

            }

        }
    }
}

