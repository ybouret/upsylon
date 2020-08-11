#include "y/memory/joint/section.hpp"
#include "y/code/round.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/base2.hpp"
#include "y/exceptions.hpp"
#include "y/os/run-time-log.hpp"
#include <cstring>

namespace upsylon {

    namespace memory {

        namespace joint {

            size_t  section::block:: round(const size_t bytes) throw()
            {
                if(bytes<=size)
                {
                    return size;
                }
                else
                {
                    assert(bytes>size);
                    const size_t ans = Y_ROUND_LN2(exp2,bytes);
                    assert(0==(ans%size));
                    return ans;
                }
            }

            size_t section:: holding(const size_t bytes, size_t &shift)
            {

                const size_t aligned_bytes = block::round(bytes);
                if(aligned_bytes>max_allocated) throw exception("joint::section:: too many bytes to hold");

                const size_t ans = next_power_of_two(max_of(aligned_bytes+2*block::size,min_size));
                shift            = integer_log2(ans); /* and check... */ assert(size_t(1)<<shift==ans);  assert(ans>=bytes);
                return ans;

            }


            section:: section(void        *usr_data,
                              const size_t usr_size,
                              const size_t usr_exp2) throw():
            entry( static_cast<block *>(usr_data) ),
            guard( entry ),
            greatest(entry),
            capacity(0),
            next(0),
            prev(0),
            size(usr_size),
            exp2(usr_exp2)
            {
                assert(usr_data!=NULL);
                assert(size>=min_size);
                assert(is_a_power_of_two(size));
                assert(size_t(1)<<exp2==size);

                size_t blocks = size >> block::exp2;
                assert(blocks>=min_blocks);
                guard += --blocks;

                entry->prev = 0;
                entry->next = guard;
                entry->from = 0;
                entry->bulk = --blocks;


                guard->prev = entry;
                guard->next = NULL;
                guard->bulk = 0;
                guard->from = this;

                assert(check_block(entry));
                assert(check_block(guard));
                
                capacity = (greatest->bulk << block::exp2);

            }
            

            section:: ~section() throw()
            {
                if(!is_empty())
                {
                    std::ostream &os = rtl(run_time_warning);
                    os << "[memory::section] not empty: ";
                    for(const block *blk=entry;blk;blk=blk->next)
                    {
                        if(blk->from)
                        {
                            std::cerr << (blk->bulk << block::exp2) << "/";
                        }
                    }
                    std::cerr << std::endl;
                }
                entry    = guard = greatest = 0;
                capacity = 0;
            }

            bool section:: is_empty() const throw()
            {
                if( (NULL==entry->from) && (guard==entry->next) )
                {
                    assert(greatest==entry);
                    assert(capacity==entry->bulk << block::exp2);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            static inline void __zero(void *p, const size_t n) throw()
            {
                assert(n>0);
                assert(p!=NULL);
                memset(p,0,n);
            }


            void *section:: acquire(size_t &n) throw()
            {
                return acquire(n,__zero);
            }

            void  section:: look_up_greatest() throw()
            {
                block *g=0;
                len_t  n=0;

                // find a first free block
                for(block *blk=entry;blk;blk=blk->next)
                {
                    if(0==blk->from)
                    {
                        g=blk;
                        n=blk->bulk;
                        break;
                    }
                }

                // find better blocks
                if(g)
                {
                    for(block *b=g->next;b;b=b->next)
                    {
                        if(NULL!=b->from) continue;
                        const size_t tmp = b->bulk;
                        if(tmp>n)
                        {
                            g=b;
                            n=tmp;
                        }
                    }

                    // update status
                    greatest = g;
                    capacity = (n << block::exp2);
                }
                else
                {
                    // full section
                    greatest = 0;
                    capacity = 0;
                }

            }
            

            section::block * section:: guess_greatest_within(block *lo, block *hi) throw()
            {
                assert(lo);
                assert(hi);

                block *g=0;
                len_t  n=0;
                for(block *blk=lo;blk!=hi;blk=blk->next)
                {
                    if(0==blk->from)
                    {
                        g = blk;
                        n = blk->bulk;
                        break;
                    }
                }

                if(g)
                {
                    for(block *b=g->next;b!=hi;b=b->next)
                    {
                        if(NULL!=b->from) continue;
                        const size_t tmp = b->bulk;
                        if(tmp>n)
                        {
                            g=b;
                            n=tmp;
                        }
                    }
                    return g;
                }
                else
                {
                    return 0;
                }

            }

            void section:: assign_greatest(block *g) throw()
            {
                assert(g);
                assert(g->is_free());
                greatest = g;
                capacity = (g->bulk<<block::exp2);
            }

            void section:: assign_greatest(block *lhs, block *rhs) throw()
            {
                assert(lhs);
                assert(rhs);
                assert(lhs<rhs);
                if(lhs->bulk>=rhs->bulk)
                {
                    assign_greatest(lhs);
                }
                else
                {
                    assign_greatest(rhs);
                }
            }

            void  section:: post_acquire_greatest() throw()
            {
                assert(greatest);
                assert(greatest->is_used());

                static const unsigned found_none  = 0;
                static const unsigned found_left  = 0x01;
                static const unsigned found_right = 0x02;
                static const unsigned found_both  = found_left | found_right;

                unsigned found = found_none;
                block   *lhs   = 0;
                if(greatest->prev && NULL!=(lhs=guess_greatest_within(entry,greatest)) )
                {
                    found |= found_left;
                }

                block   *rhs   = 0;
                {
                    block *at_right = greatest->next;
                    if(at_right && NULL!=(rhs=guess_greatest_within(at_right,guard) ))
                    {
                        found |= found_right;
                    }
                }

                switch(found)
                {
                    case found_left:
                        assert(!rhs);
                        assert(lhs);
                        assert(lhs->is_free());
                        assign_greatest(lhs);
                        break;

                    case found_right:
                        assert(!lhs);
                        assert(rhs);
                        assert(rhs->is_free());
                        assign_greatest(rhs);
                        break;

                    case found_both:
                        assert(lhs);
                        assert(rhs);
                        assert(lhs<rhs);
                        assign_greatest(lhs,rhs);
                        break;

                    default:
                        assert(found_none==found);
                        greatest = 0;
                        capacity = 0;
                        break;
                }

#ifndef NDEBUG
                {
                    block *g = greatest;
                    size_t n = capacity;
                    look_up_greatest();
                    assert(greatest==g);
                    assert(capacity==n);
                }
#endif

            }



            void * section:: acquire(size_t &n, finalize proc) throw()
            {
                //--------------------------------------------------------------
                // parameters to trigger a new block
                //--------------------------------------------------------------
                static const size_t split_blocks = 3;
                static const size_t extra_blocks = split_blocks-1;
                static const size_t delta_blocks = extra_blocks-1;

                assert(proc);

                if(greatest)
                {
                    assert(greatest->is_free());
                    if(capacity>=n)
                    {

                        //------------------------------------------------------
                        // ok!
                        //------------------------------------------------------
                        block       *currBlock = greatest;
                        const size_t available = currBlock->bulk;
                        const size_t boundary  = block::round(n);         assert(capacity  >= boundary);
                        const size_t required  = boundary >> block::exp2; assert(available >= required);

                        if(available>=required+extra_blocks)
                        {
                            //--------------------------------------------------
                            // shall split
                            //--------------------------------------------------
                            
                            // create a new block
                            block *nextBlock = currBlock->next;
                            block *new_block = currBlock+required+1;
                            new_block->prev  = currBlock;
                            new_block->next  = nextBlock;
                            new_block->from  = 0;
                            new_block->bulk  = available-required-delta_blocks;

                            // update nextBlock
                            nextBlock->prev = new_block;

                            // update currBlock
                            currBlock->bulk = required;
                            currBlock->next = new_block;

                            assert(check_block(currBlock));
                            assert(check_block(nextBlock));
                            assert(check_block(new_block));
                            assert(currBlock->bulk==required);
                            n = boundary;
                        }
                        else
                        {
                            //--------------------------------------------------
                            // take the full currBlock
                            //--------------------------------------------------
                            n = capacity;
                        }
                        
                        assert( n == (currBlock->bulk<<block::exp2) );

                        currBlock->from = this;
                        void *p = &currBlock[1];
                        proc(p,n);
                        post_acquire_greatest();
                        return p;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // not enough space
                        //------------------------------------------------------
                        return 0;
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    // section has no blocks left
                    //----------------------------------------------------------
                    assert(0==greatest);
                    assert(0>=capacity);
                    return 0;
                }


            }




            static inline void __nope(void*,const size_t) throw()
            {

            }

            section * section:: receive(void * &addr, size_t &capa, const size_t size) throw()
            {
                assert(addr);
                assert(capa);
                assert(capa>=size);
                size_t n = size;
                void  *p = acquire(n,__nope);
                if(p)
                {
                    assert(n>=size);
                    memcpy(p,addr,size);
                    memset(static_cast<char*>(p)+size,0,n-size);
                    section *s = release(addr,capa);
                    addr = p;
                    capa = n;
                    return s;
                }
                else
                {
                    // left untouched
                    return 0;
                }
            }

            section * section:: owner_of(void *p) throw()
            {
                assert(p);
                block *b = static_cast<block *>(p)-1;
                assert(b->from);
                return b->from;
            }


            section *section:: release(void * &addr, size_t &n) throw()
            {
                assert(addr); assert(n>0);

                //--------------------------------------------------------------
                // get block
                //--------------------------------------------------------------
                block   *currBlock = static_cast<block *>(addr) - 1;
                assert(currBlock->from);
                assert(currBlock->bulk*block::size==n);

                //--------------------------------------------------------------
                // get owner
                //--------------------------------------------------------------
                section *owner     = currBlock->from;
                assert(owner->check_block(currBlock));
                assert(owner->guard!=currBlock);
                assert(currBlock!=owner->greatest);

                //--------------------------------------------------------------
                // check status
                //--------------------------------------------------------------
                static const unsigned merge_none = 0x00;
                static const unsigned merge_prev = 0x01;
                static const unsigned merge_next = 0x02;
                static const unsigned merge_both = merge_prev|merge_next;

                unsigned flag      = merge_none;
                block   *prevBlock = currBlock->prev; if(prevBlock&&(0==prevBlock->from))       flag |= merge_prev;
                block   *nextBlock = currBlock->next; assert(nextBlock); if(0==nextBlock->from) flag |= merge_next;

                //--------------------------------------------------------------
                // merge
                //--------------------------------------------------------------
                block *guess = 0;
                switch(flag)
                {
                    case merge_prev:
                        prevBlock->next = nextBlock;
                        nextBlock->prev = prevBlock;
                        prevBlock->bulk = static_cast<len_t>(nextBlock-prevBlock)-1;
                        assert(owner->check_block(prevBlock));
                        assert(owner->check_block(nextBlock));
                        guess = prevBlock;
                        break;

                    case merge_next:
                        assert(nextBlock->next!=NULL);
                        nextBlock=nextBlock->next;
                        currBlock->next = nextBlock;
                        nextBlock->prev = currBlock;
                        currBlock->bulk = static_cast<len_t>(nextBlock-currBlock)-1;
                        currBlock->from = 0;
                        assert(owner->check_block(currBlock));
                        assert(owner->check_block(nextBlock));
                        guess = currBlock;
                        break;

                    case merge_both:
                        assert(nextBlock->next!=NULL);
                        assert(nextBlock->next->from);
                        nextBlock=nextBlock->next;
                        prevBlock->next = nextBlock;
                        nextBlock->prev = prevBlock;
                        prevBlock->bulk = static_cast<len_t>(nextBlock-prevBlock)-1;
                        assert(owner->check_block(prevBlock));
                        assert(owner->check_block(nextBlock));
                        guess=prevBlock;
                        break;

                    default:
                        assert(merge_none==flag);
                        currBlock->from = 0;
                        guess = currBlock;
                        break;
                }

                assert(guess);
                assert(guess->from==0);
                owner->post_release_greatest(guess);
                addr = 0;
                n    = 0;
                return owner;
            }


            void section:: post_release_greatest(block *guess) throw()
            {
                assert(guess);
                assert(guess->is_free());

                if(!greatest)
                {
                    assign_greatest(guess);
                }
                else
                {
                    if( guess==greatest )
                    {
                        // update capacity
                        capacity = (greatest->bulk<<block::exp2);
                    }
                    else
                    {
                        // choice
                        assert(greatest!=NULL && guess!=greatest);
                        if(guess<greatest) assign_greatest(guess,greatest); else assign_greatest(greatest,guess);
                    }
                }

#ifndef NDEBUG
                {
                    block *g = greatest;
                    size_t n = capacity;
                    look_up_greatest();
                    assert(greatest==g);
                    assert(capacity==n);
                }
#endif
            }


        }
    }

}


namespace upsylon {

    namespace memory {

        namespace joint {

            void section:: display() const
            {
                assert(entry);
                assert(guard);
                const block *blk = entry;
                std::cerr << '|';
                while(blk)
                {
                    std::cerr << "(" << block::size << ")";
                    if(blk->next==NULL)
                    {
                        std::cerr << '|';
                    }
                    else
                    {
                        if(blk->from)
                        {
                            std::cerr << "<" << blk->bulk * block::size << ">";
                        }
                        else
                        {
                            std::cerr << "[" << blk->bulk * block::size << "]";
                        }
                    }
                    blk = blk->next;
                }
                std::cerr << std::endl;

            }

#define Y_MEM_SEC_BLK(EXPR) do { if(!(EXPR)) { std::cerr << pfx << #EXPR << sfx << std::endl; return false; } } while(false)

            bool section:: check_block(const block *blk) const
            {
                static const char pfx[] = "memory.section.check_block: ! '";
                static const char sfx[] = "'";
                assert(blk);
                if(blk->next)
                {
                    const block *nxt = blk->next;
                    Y_MEM_SEC_BLK(nxt->prev==blk);
                    Y_MEM_SEC_BLK(blk->bulk==static_cast<len_t>(nxt-blk)-1);
                }

                if(blk->from)
                {
                    Y_MEM_SEC_BLK(blk->from==this);
                }

                if(blk->prev)
                {
                    const block *prv = blk->prev;
                    Y_MEM_SEC_BLK(prv->next==blk);
                }

                return true;
            }

            bool section:: check() const
            {
                for(const block *blk=entry; blk; blk=blk->next)
                {
                    if(!check_block(blk)) return false;
                }
                return true;
            }
        }
    }

}

