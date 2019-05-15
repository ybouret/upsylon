
#include "y/program.hpp"
#include "y/ptr/intr.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/associative/set.hpp"
#include "y/hashing/sha1.hpp"

using namespace upsylon;

class Entry : public counted_object
{
public:
    typedef hashing::sha1                 Hasher;
    typedef key_hasher<string,Hasher>     KeyHasher;
    typedef intr_ptr<string,Entry>        Pointer;
    typedef set<string,Pointer,KeyHasher> DataBase;

    const string content;

    explicit Entry( const string &line ) : content(line){}
    virtual ~Entry() throw(){}
    const string & key() const throw() { return content; }

private:
    Y_DISABLE_COPY_AND_ASSIGN(Entry);
};

Y_PROGRAM_START()
{

    Entry::DataBase edb(128 * 1024, as_capacity );
    ios::icstream   inp( ios::cstdin  );
    ios::ocstream   out( ios::cstdout );
    string line;
    while( inp.gets(line) )
    {
        const Entry::Pointer p = new Entry( line );
        if(edb.insert(p))
        {
            out << p->content << '\n';
        }
    }

}
Y_PROGRAM_END();
