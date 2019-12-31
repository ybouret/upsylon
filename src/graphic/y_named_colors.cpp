#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

#include "y/graphic/color/rgb.hpp"

#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace Graphic;

static inline int rgbScore( const rgb &C ) throw()
{
    return int(C.r) + int(C.g) + int(C.b);
}

class Entry
{
public:
    typedef  set<string,Entry> DataBase;

    const string   name;
    const rgb      code;

    inline Entry( const string &id, const rgb &C ) :
    name(id), code(C)
    {
    }

    inline   ~Entry() throw() {}

    inline const string & key() const throw() { return name; }

    inline Entry( const Entry &other) : name( other.name ), code( other.code ) {}


    static int Compare( const Entry &lhs, const Entry &rhs ) throw()
    {
        const int L = rgbScore(lhs.code);
        const int R = rgbScore(rhs.code);
        if(L<R)
        {
            return -1;
        }
        else if(R<L)
        {
            return 1;
        }
        else
        {
            return string::compare(lhs.name, rhs.name);
        }
    }

    inline friend std::ostream & operator<< ( std::ostream &os, const Entry &E )
    {
        os << '<' << E.name << " : " << int(E.code.r) << ' ' << int(E.code.g) << ' ' << int(E.code.b) << '>';
        return os;
    }

private:
    Y_DISABLE_ASSIGN(Entry);
};

#include "y/string/convert.hpp"

static inline uint8_t getField( const string &id, const string &field, const char *label )
{
    assert(label);
    const size_t value = string_convert::to<size_t>(field,label);
    if(value>255) throw exception("invalid %s for %s", label, *id);
    return uint8_t(value);
}



Y_PROGRAM_START()
{
    if(argc>1)
    {
        const string    fileName = argv[1];
        Entry::DataBase colors(1024,as_capacity);
        size_t          maxLen = 0;

        {
            ios::icstream  fp(fileName);
            string         line;
            vector<string> words(16,as_capacity);
            while( fp.gets(line) )
            {
                tokenizer<char>::split_with(words, line," \t");
                if( words.size() < 4 ) continue;
                while( words.size() > 4 ) words.pop_back();

                string        name = words[1];
                const uint8_t r    = getField(name,words[2],"red");
                const uint8_t g    = getField(name,words[3],"green");
                const uint8_t b    = getField(name,words[4],"blue");
                const rgb     code(r,g,b);
                string_convert::to_upper(name);
                name = "Y_" + name;
                std::cerr << name << std::endl;
                const Entry   E(name,code);
                if( !colors.insert(E) )
                {
                    throw exception("multiple %s", *name);
                }
                maxLen = max_of(maxLen,name.length());
            }
        }

        std::cerr << "#entry=" << colors.size() << ", maxLen=" << maxLen << std::endl;
        colors.sort_data( Entry::Compare );
        std::cerr << colors << std::endl;
        const unsigned n = unsigned( colors.size() );

        {
            ios::ocstream fp("named.hxx");
            fp << "#define Y_NAMED_COLORS ";
            fp("%u\n", n);
            unsigned i=0;
            for( Entry::DataBase::iterator it=colors.begin(); it != colors.end(); ++it,++i )
            {
                const Entry &E = *it;
                fp << "#define " << E.name;
                fp.repeat(maxLen-E.name.length(), ' ');
                fp(" upsylon::Graphic::rgb(0x%02x,0x%02x,0x%02x)",E.code.r,E.code.g,E.code.b);
                fp << '\n';
                fp << "#define " << E.name << "_INDEX ";
                fp.repeat(maxLen-E.name.length(), ' ');
                fp("%u\n",i);
            }
        }

        {
            ios::ocstream fp("named.cxx");
            unsigned      i = 1;
            for( Entry::DataBase::iterator it=colors.begin(); it != colors.end(); ++it, ++i)
            {
                const Entry &E = *it;
                fp << "NamedColor(\"" << &E.name[2] << "\",";
                fp.repeat(maxLen-E.name.length(), ' ');
                fp("rgb(0x%02x,0x%02x,0x%02x)",E.code.r,E.code.g,E.code.b);
                fp << ")";
                if(i<n) fp << ',';
                fp << '\n';
            }
        }

    }
}
Y_PROGRAM_END()


