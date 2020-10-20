
#include "y/associative/affix/tree.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/string.hpp"
#include "y/hashing/sha1.hpp"
#include "y/associative/be-key.hpp"

using namespace  upsylon;


namespace {

    class Affix : public affix
    {
    public:

        explicit Affix() :  affix()
        {
        }

        template <typename ITERATOR>
        bool add(ITERATOR curr,const size_t size, void *addr)
        {
            return grow(curr,size,addr);
        }

        template <typename ITERATOR>
        const tree_node * get(ITERATOR curr,const size_t size)
        {
            return find(curr,size);
        }


        void rm(const tree_node *node) throw()
        {
            cut((tree_node*)node);
        }

        void clr() throw()
        {
            reset();
        }

        virtual ~Affix() throw()
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Affix);
    };

    static inline void build(Affix &atree, const accessible<string> &strings)
    {
        atree.clr();
        std::cerr << "in_pool: " << std::setw(6) << atree.in_pool() << " ";
        atree.limit(1024);
        size_t inserted = 0;
        for(size_t i=1;i<=strings.size();++i)
        {
            const string &data = strings[i];
            const bool    res  = atree.add(*data, data.length(), (void*)&data);
            if(res) ++inserted;
        }
        std::cerr << "inserted=" << inserted;
        Y_ASSERT(atree.entries()==inserted);
        for(size_t i=strings.size();i>0;--i)
        {
            const string           &data = strings[i];
            const affix::tree_node *node = atree.get(*data,data.length());
            Y_ASSERT(node);
            Y_ASSERT(&data==node->addr);
        }

    }

    static inline void low_level( vector<string> &strings )
    {
        Affix          atree;
        hashing::sha1  H;


        alea.shuffle(*strings, strings.size() );
        build(atree,strings);
        atree.hash_with(H);
        const digest md1 = H.md();
        std::cerr << " md1=" << md1 << std::endl;

        for(size_t iter=0;iter<8;++iter)
        {
            alea.shuffle(*strings, strings.size() );
            build(atree,strings);
            atree.hash_with(H);
            const digest md2 = H.md();
            std::cerr << " md2=" << md2 << std::endl;
            Y_ASSERT(md1==md2);
        }

        const bool save = atree.entries() <= 30;
        if(save) atree.graphViz("atree.dot");


        Y_ASSERT(strings.size()==atree.entries());
        vector<size_t> indx(strings.size(),0);
        for(size_t i=strings.size();i>0;--i) indx[i] = i;

        for(size_t iter=0;iter<8;++iter)
        {
            alea.shuffle(*strings, strings.size() );
            build(atree,strings);

            alea.shuffle(*indx,indx.size());
            const size_t half = indx.size()/2;
            for(size_t i=half;i>0;--i)
            {
                const string           &data = strings[indx[i]];
                const affix::tree_node *node = atree.get(*data,data.length());
                Y_ASSERT(node);
                Y_ASSERT(node->addr);
                Y_ASSERT(node->addr == &data );
                atree.rm(node);
            }


            for(size_t i=indx.size();i>half;--i)
            {
                const string           &data = strings[indx[i]];
                const affix::tree_node *node = atree.get(*data,data.length());
                Y_ASSERT(node);
                Y_ASSERT(node->addr);
                Y_ASSERT(node->addr == &data );
                atree.rm( (affix::tree_node *)node );
            }

            std::cerr << std::endl;
        }

        // using addresses
        atree.clr();
        for(size_t i=strings.size();i>0;--i)
        {
            const string &data = strings[i];
            const be_key key( data );
            Y_ASSERT(atree.add((const char *)(key.ro()),key.length(),(void*)&data));
        }
        if(atree.entries()<=30) atree.graphViz("addresses.dot");
    }

    static inline void high_level( vector<string> &strings )
    {
        affix_tree<string> tree;
        for(size_t iter=0;iter<8;++iter)
        {
            alea.shuffle(*strings, strings.size());
            size_t inserted = 0;
            tree.erase();
            for(size_t i=strings.size();i>0;--i)
            {
                const string &data = strings[i];
                if(tree.insert_by(data,data)) ++inserted;
            }
            std::cerr << "#entries=" << tree.entries() << std::endl;
            Y_ASSERT(inserted==tree.entries());
        }
        for(size_t i=strings.size();i>0;--i)
        {
            const string &data = strings[i];
            Y_ASSERT(tree.has(*data,data.size()));
        }

        if(tree.entries()<=30)
        {
            tree.graphViz("affix_tree.dot");
        }


        for(size_t i=strings.size();i>0;--i)
        {
            const string &data = strings[i];
            if( tree.has( *data, data.size() ))
            {
                Y_ASSERT( tree.remove_by(data) );
            }
        }
        Y_ASSERT(0==tree.entries());



    }

}

Y_UTEST(affix)
{
    Y_UTEST_SIZEOF(affix::tree_node);


    if(argc>1)
    {
        vector<string> strings;
        {
            ios::icstream fp(argv[1]);
            string        line;
            while( fp.gets(line) )
            {
                strings << line;
            }
        }
        low_level(strings);
        high_level(strings);

    }







}
Y_UTEST_DONE()

