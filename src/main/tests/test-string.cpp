
#include "y/strfwd.hpp"
#include "support.hpp"
#include "y/utest/run.hpp"
#include "y/core/node.hpp"
#include "y/core/list.hpp"
#include "y/sort/merge.hpp"

using namespace upsylon;

#define __DISP(X) std::cerr << #X "='" << X << "'" << std::endl

namespace
{
    class s_node : public core::inode<s_node>
    {
    public:
        string s;
        explicit s_node(const string &_) : core::inode<s_node>(), s(_) {}
        virtual ~s_node() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(s_node);
    };

    static inline int compare_strings_by_length( const s_node *lhs, const s_node *rhs, void *)
    {
        return comparison::increasing(lhs->s.size(),rhs->s.size());
    }

}

Y_UTEST(string)
{
    //! test length
    std::cerr << std::endl;
    std::cerr << length_of( ""    ) << std::endl;
    std::cerr << length_of("Hello") << std::endl;

    // constructor
    std::cerr << std::endl;
    string s1; Y_CHECK(s1.size()==0);__DISP(s1);
    string s2(12,as_capacity,false);Y_CHECK(s2.size()==0);Y_CHECK(s2.capacity()>=12);
    string s3( "Hello" );  Y_CHECK(s3.size()==5); __DISP(s3);
    string s4 = "World";   Y_CHECK(s4.size()==5); __DISP(s4);
    string s5 = (char *)0; Y_CHECK(s5.size()==0);
    string s6 = 'a';       Y_CHECK(s6.size()==1); __DISP(s6);

    // addition
    std::cerr << std::endl;
    string s7 = "Initial";
    s7 += s3;
    s7 += "And";
    s7 += 'C';
    __DISP(s7);

    string s8  = "hello" + s4;      __DISP(s8);
    string s9  = s3      + "world"; __DISP(s9);
    string s10 = 'H'     + s4;      __DISP(s10);
    string s11 = s3      + 'H';     __DISP(s11);

    std::cerr << std::endl;
    for(size_t i=0;i<10;++i)
    {
        const string lhs = support::get<string>();
        const string rhs = support::get<string>();
        const int    cmp = string::compare(lhs,rhs);
        std::cerr << "compare(" << lhs << "," << rhs <<")=" << cmp << std::endl;
        Y_CHECK(string::compare(lhs,lhs)==0);
    }

    std::cerr << std::endl;
    const char   *l  = "a";
    const char   *r  = "b";
    const char    L  = 'a';
    const char    R  = 'b';
    const string lhs = l;
    const string rhs = r;
    Y_CHECK(lhs<rhs);  Y_CHECK(lhs<r);  Y_CHECK(lhs<R);
    Y_CHECK(lhs<=rhs); Y_CHECK(lhs<=r); Y_CHECK(lhs<=R);
    Y_CHECK(lhs==lhs); Y_CHECK(lhs==l); Y_CHECK(lhs==L);
    Y_CHECK(lhs!=rhs); Y_CHECK(lhs!=r); Y_CHECK(lhs!=R);
    Y_CHECK(rhs>=lhs); Y_CHECK(rhs>=l); Y_CHECK(rhs>=L);
    Y_CHECK(rhs>lhs);  Y_CHECK(rhs>l);  Y_CHECK(rhs>L);

    std::cerr << "skip/trim" << std::endl;
    {
        string s;
        for(size_t i=0;i<80;++i)
        {
            s += alea.range<char>('!','~');
        }
        std::cerr << s << std::endl;
        while(s.size())
        {
            if( alea.choice() )
            {
                s.trim( alea.leq(10) );
            }
            else
            {
                s.skip( alea.leq(10) );
            }
            if(s.size())
            std::cerr << s << std::endl;
        }
    }

    std::cerr << "compacting..." << std::endl;
    {
        core::list_of_cpp<s_node> s_list;

        while( s_list.size <= 4000 )
        {
            const string tmp = support::get<string>();
            s_list.push_back( new s_node(tmp) );
        }
        alea.shuffle(s_list);
        {
            const size_t nh = s_list.size/2;
            while(s_list.size>nh)
            {
                delete s_list.pop_back();
            }
        }
        merging<s_node>::sort(s_list, compare_strings_by_length, NULL);
        size_t j = 0;
        for(s_node *node = s_list.head; node; node=node->next )
        {
            const uint32_t str32 = crc32( *(node->s), node->s.size()+1 );
            if( node->s.compact() )
            {
                Y_ASSERT( crc32( *(node->s), node->s.size()+1 ) == str32 );
                std::cerr << '+';
            }
            else
            {
                std::cerr << '-';
            }
            if( 0 == (++j&63) ) std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }

}
Y_UTEST_DONE()

