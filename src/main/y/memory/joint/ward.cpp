#include "y/memory/joint/ward.hpp"
#include "y/type/utils.hpp"
#include "y/code/base2.hpp"
#include "y/exception.hpp"
#include "y/os/run-time-log.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon {

    namespace memory {

        namespace joint {

            static inline
            size_t ward_chunk_size(const size_t chunk_size)
            {
                assert(section::min_size<=tight::vein::max_size);
                return next_power_of_two( clamp(section::min_size,chunk_size,tight::vein::max_size) );
            }

            ward:: ~ward() throw()
            {
                while(S.size)
                {
                    section     *s = S.pop_back(); // remove section
                    tight::vein &v = Q[s->exp2];   // find memory provider
                    void        *p = s->entry;     // find memory address
                    self_destruct(*s);             // cleanup the segment
                    v.release(p);                  // return memory into vein of quarry
                    Z.zstore(s);                   // return zombie section
                }
            }
            
            size_t ward:: chunk_size() const throw()
            {
                return V.block_size;
            }

            const ward::quarry_type & ward:: _quarry()   const throw()
            {
                return Q;
            }

            const ward::sections    & ward:: _sections()   const throw()
            {
                return S;
            }

            

            ward:: ward(const size_t usr_chunk_size) :
            acquiring(0),
            empty_one(0),
            S(),
            Q(),
            V(Q(ward_chunk_size(usr_chunk_size))),
            Z(V.block_size,Q)
            {
                std::cerr << "W.chunk_size=" << chunk_size()   << std::endl;
                std::cerr << "Z.chunk_size=" << Z.chunk_size << std::endl;
                std::cerr << "|_nodes_rise=" << Z.nodes_rise << " section/part" << std::endl;

                acquiring = empty_one = S.push_back( section_for(0) );

            }

            tight::vein & ward:: vein_for(const size_t required)
            {
                size_t       shift=0;
                const size_t bytes = section::holding(required,shift); assert(bytes<=tight::vein::max_size);

                if(bytes<=V.block_size)
                {
                    assert(bytes<=V.block_size);
                    assert(shift<=V.block_exp2);
                    return V;
                }
                else
                {
                    assert(shift>=tight::vein::min_exp2);
                    assert(shift<=tight::vein::max_exp2);
                    return Q[shift];
                }

            }

            section     * ward:: section_for(const size_t required)
            {
                tight::vein &v = vein_for(required);
                section     *s = Z.zquery();
                try {
                    return new (s) section( v.acquire(), v.block_size, v.block_exp2 );
                }
                catch(...)
                {
                    Z.zstore(s);
                    throw;
                }
            }

#define Y_MEM_WARD_TEST(PTR,MEMBER) \
do { if( 0 != (p=PTR->acquire(n)) ) { acquiring=PTR; goto RETURN; } PTR=PTR->MEMBER; } while(false)

#define Y_MEM_WARD_TEST_LO() Y_MEM_WARD_TEST(lo,prev)
#define Y_MEM_WARD_TEST_HI() Y_MEM_WARD_TEST(hi,next)

            void * ward:: acquire_block(size_t &n)
            {
                assert(acquiring);
                void *p = acquiring->acquire(n);
                if(p)
                {
                    return p;
                }
                else
                {
                    // interleaved look-up
                    section *lo = acquiring->prev;
                    section *hi = acquiring->next;
                    while(lo&&hi)
                    {
                        Y_MEM_WARD_TEST_LO();
                        Y_MEM_WARD_TEST_HI();
                    }

                    while(lo) Y_MEM_WARD_TEST_LO();
                    while(hi) Y_MEM_WARD_TEST_HI();

                    // new section creation
                    acquiring = S.push_back(section_for(n)); assert(acquiring->capacity>=n);
                    p         = acquiring->acquire(n);
                    {
                        section *s = acquiring;
                        while(s->prev && (s->entry<s->prev->entry) )
                        {
                            S.towards_head(s);
                        }
                    }
                    return p;

                RETURN:
                    if(acquiring==empty_one)
                    {
                        empty_one = 0; 
                    }
                    return p;
                }
            }


            void ward:: release_block(void *&p, size_t &n) throw()
            {
                section *releasing = section::release(p,n);
                (void) releasing;
            }

            std::ostream & operator<<( std::ostream &os, const ward &w)
            {
                os << "<ward>" << std::endl;
                os << w.Q << std::endl;
                os << "<sections #=" << w.S.size << ">" << std::endl;
                for(const section *s = w.S.head;s;s=s->next)
                {
                    std::cerr << "\t@" << s->entry << "+" << s->size << std::endl;
                }
                os << "<sections/>" << std::endl;
                os << "<ward/>";
                return os;
            }

        }
    }

}
