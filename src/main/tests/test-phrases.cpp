#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/counting/perm.hpp"
#include "y/counting/comb.hpp"
#include "y/ios/icstream.hpp"
#include "y/associative/suffix-tree.hpp"
#include "y/os/progress.hpp"
#include "y/core/node.hpp"
#include "y/ptr/arc.hpp"

using namespace upsylon;

namespace {

    typedef core::cpp_node_of<char>      char_type;
    typedef core::list_of_cpp<char_type> char_list_;

    class char_list : public char_list_, public counted_object
    {
    public:
        inline  char_list() throw() {}

        inline ~char_list() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(char_list);
    };

    typedef arc_ptr<char_list> shared_list;

    static inline
    void split( const shared_list &source )
    {
        for(size_t i=1;i<source->size;++i)
        {
            shared_list head = new char_list();
            shared_list tail = new char_list();
            const char_type *ch = source->head;
            for(size_t j=1;j<=i;++j,ch=ch->next)
            {
                head->push_back( new char_type( **ch ) );
            }
            
            for(ch=ch->next;ch;ch=ch->next)
            {
                tail->push_back( new char_type( **ch ) );
            }
        }
    }





}

Y_UTEST(phrases)
{
    if(argc>1)
    {
        const string      alphabet = argv[1];
        const size_t      letters  = alphabet.size(); std::cerr << "'" << alphabet << "'" << std::endl; std::cerr << "#card=" << letters << std::endl;
        permutation       perm(letters);              std::cerr << "#perm=" << perm.count << std::endl;
        suffix_tree<bool> dict(perm.count,as_capacity);
        string            motif(letters,as_capacity,true);

        progress          bar;
        bar.start();
        double old = bar.query();
        for( perm.start(); perm.valid(); perm.next() )
        {

            perm.apply(*motif,*alphabet);
            if(!dict.insert_by(motif,true))
            {
                continue;
            }

            if( bar.query() - old >= 0.5 )
            {
                bar.update(perm.index, perm.count);
                bar.display(std::cerr) << '\r';
                old = bar.query();
            }

            shared_list source = new char_list();
            for(size_t i=0;i<letters;++i)
            {
                source->push_back( new char_type(motif[i]) );
            }

            split(source);

        }
        bar.update(1.0);
        bar.display(std::cerr) << std::endl;
        std::cerr << "found [" << dict.entries() << "/" << perm.count << "]" << std::endl;




    }
}
Y_UTEST_DONE()


