#include "y/lang/dynamo/hash31.hpp"
#include "y/utest/run.hpp"

#include "y/ios/icstream.hpp"
#include "y/ptr/auto.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/set.hpp"

using namespace upsylon;
using namespace Lang;

class Word : public counted_object
{
public:
    const int32_t code;
    const string  data;
    inline const int32_t & key() const throw() { return code; }

    inline explicit Word( const int32_t c, const string &d ) : code(c), data(d) {}
    inline virtual ~Word() throw() {}

    typedef intr_ptr<int32_t,Word>   Ptr;
    typedef set<int32_t,Ptr>         Set;


private:
    Y_DISABLE_COPY_AND_ASSIGN(Word);
};

Y_UTEST(h31)
{
    DynamoHash31 H;
    Word::Set    words(60000,as_capacity);

    if(argc>1)
    {
        {
            const string           filename = argv[1];
            auto_ptr<ios::istream> input    = (filename=="STDIN") ? new ios::icstream(ios::cstdin) : new ios::icstream(filename);
            string line;
            while( input->gets(line) )
            {
                const Word::Ptr  w  = new Word( H(line), line );
                //std::cerr << w->data << ":" << w->code << std::endl;
                const Word::Ptr *pw = words.search(w->code);
                if( pw )
                {
                    throw exception("collision of '%s' with '%s'", *((**pw).data), *(w->data) );
                }
                if(!words.insert(w))
                {
                    throw exception("unexpected failure to insert '%s'", *(w->data));
                }
            }
        }
        std::cerr << "#words=" << words.size() << std::endl;

    }
}
Y_UTEST_DONE()



