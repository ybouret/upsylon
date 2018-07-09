#include "y/memory/vein.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    class block : public object
    {
    public:
        block *next;
        block *prev;
        void  *addr;
        size_t size;
        inline explicit block() throw() : next(0), prev(0), addr(0), size(0) {}
        inline virtual ~block() throw() {}
    private:
        Y_DISABLE_COPY_AND_ASSIGN(block);
    };

    static inline
    void add_block(core::list_of<block> &blocks,
                   memory::vein         &v,
                   const size_t          nreq)
    {
        size_t n = nreq;
        void  *p = v.acquire(n);
        assert(p);
        assert(n>0);
        assert(is_a_power_of_two(n));
        block *b = new block();
        b->addr = p;
        b->size = n;
        blocks.push_back(b);
        std::cerr << "(+) " << nreq << " -> " << n << std::endl;
    }

    static inline
    void clear_blocks(core::list_of<block> &blocks,
                      memory::vein         &v)
    {
        while(blocks.size)
        {
            block *b = blocks.pop_back();
            assert(b->addr);
            assert(is_a_power_of_two(b->size));
            //std::cerr << "(-) " << b->size << std::endl;
            v.release(b->addr,b->size);
            delete b;
        }
    }

}


Y_UTEST(vein)
{
    std::cerr << "-- Building vein..." << std::endl;
    memory::vein v;

    std::cerr << "-- Checking sizes..." << std::endl;
    for(size_t i=0;i<=v.max_size;++i)
    {
        size_t       ibit = 0;
        const size_t alen = memory::vein::bytes_for(i,ibit);
        if(!is_a_power_of_two(alen)) throw exception("invalid aligned length");
        if(alen<i) throw exception("invalid length for input=%lu", (unsigned long)i);
    }

    


    core::list_of_cpp<block> blocks;
    for(size_t iter=0;iter<800;++iter)
    {
        add_block(blocks,v,0);
        for(size_t i=0;i<=v.max_bits;++i)
        {
            add_block(blocks,v,1<<i);
        }
        std::cerr << "#blocks=" << blocks.size << std::endl;
    }
    alea.shuffle(blocks);
    clear_blocks(blocks,v);

    return 0;

    for(size_t iter=0;iter<128;++iter)
    {
        assert(0==blocks.size);
        for(size_t i=0;i<16000;++i)
        {
            size_t n = alea.leq( memory::vein::max_size );
            void  *p = v.acquire(n);
            block *b = new block();
            b->addr = p;
            b->size = n;
            blocks.push_back(b);
        }
        alea.shuffle(blocks);
        while(blocks.size)
        {
            block *b = blocks.pop_back();
            v.release(b->addr,b->size);
            delete b;
        }
    }

}
Y_UTEST_DONE()

