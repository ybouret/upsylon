#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/convert.hpp"
#include "y/sort/heap.hpp"
#include "y/type/utils.hpp"

using namespace upsylon;



namespace
{
    static inline bool is_sep( const int C ) { return C == ' ' || C == '\t'; }

    class Color
    {
    public:
        const string name;
        const unsigned r,g,b;
        inline Color( const string &id, const size_t R, const size_t G, const size_t B ) :
        name(id), r(R), g(G), b(B)
        {
        }
        inline ~Color() throw() {}

        inline Color(const Color &C) : name(C.name), r(C.r), g(C.g), b(C.b) {}

        static inline
        unsigned GetComponent( const string &s, const string &id, const char *field)
        {
            const string full = id + '.' + field;
            const size_t value = unsigned(string_convert::to<size_t>(s, *full ));
            if(value>255)
            {
                throw exception("%s is too big",*full);
            }
            return value;
        }

        static int Compare( const Color &lhs, const Color &rhs )
        {
            return string::compare(lhs.name,rhs.name);
        }

    private:
        Y_DISABLE_ASSIGN(Color);
    };


}

Y_PROGRAM_START()
{
    if(argc<=1) throw exception("usage: %s colors.txt", program);

    vector<Color> colors;

    // load
    {
        const string  fn = argv[1];
        ios::icstream fp(fn);
        string         line;
        vector<string> words;
        while( fp.gets(line) )
        {
            const size_t ns = tokenizer<char>::split(words,line,is_sep);
            if(ns>=4)
            {
                string_convert::to_upper(words[1]);
                const string name = "Y_" + words[1];
                const Color C(name,
                              Color::GetComponent(words[2], name, "r"),
                              Color::GetComponent(words[3], name, "g"),
                              Color::GetComponent(words[4], name, "b"));
                //std::cerr << C.name << " " << C.r << " " << C.g << " " << C.b << std::endl;
                for(size_t i=1;i<=colors.size();++i)
                {
                    if(name==colors[i].name)
                    {
                        throw exception("multiple '%s'", *name);
                    }
                }
                colors.push_back(C);
            }
        }
    }

    // prepare
    hsort(colors,Color::Compare);
    size_t maxlen = 0;
    for(size_t i=1;i<=colors.size();++i)
    {
        maxlen = max_of(maxlen,colors[i].name.size());
    }

    // save
    {
        for(size_t i=1;i<=colors.size();++i)
        {
            const Color &C = colors[i];
            std::cerr << C.name;
            for(size_t j=C.name.size();j<=maxlen;++j)
            {
                std::cerr << ' ';
            }
            const string code = vformat("0x%02x,0x%02x,0x%02x",C.r,C.g,C.b);
            std::cerr << code;
            std::cerr << std::endl;
        }
    }


}
Y_PROGRAM_END()

