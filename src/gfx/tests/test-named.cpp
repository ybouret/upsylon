#include "y/gfx/color/rgb.hpp"
#include "y/utest/run.hpp"

#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/sequence/vector.hpp"
#include "y/associative/hash/set.hpp"

#include "y/type/utils.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    class Color
    {
    public:
        const string   n;
        const rgb      c;
        const unsigned I;

        inline Color(const string &id, const rgb &C) :
        n(id), c(C), I( unsigned(c.r) + unsigned(c.g) + unsigned(c.b) )
        {
        }

        inline ~Color() throw()
        {
        }

        inline const string &key() const throw() { return n; }

        inline Color(const Color &C) : n(C.n), c(C.c), I(C.I) {}


    private:
        Y_DISABLE_ASSIGN(Color);
    };

    typedef hash_set<string,Color> ColorDB;

}

Y_UTEST(named)
{
    static const char *fields[] = { "red", "green", "blue" };

    if(argc>1)
    {
        const string filename = argv[1];
        ColorDB      db;
        size_t       maxlen = 0;
        {
            string        line;
            ios::icstream fp(filename);
            vector<string,memory::pooled> words(16,as_capacity);
            while(fp.gets(line))
            {
                line.clean_with(" \t");
                if(line.size()<=0) continue;
                tokenizer<char>::split_with(" \t",words,line);
                if(words.size()<4) continue;
                while(words.size()>4) words.pop_back();
                
                string     id = "Y_" + words[1];
                string_convert::make_upper(id);
                rgb           C;
                uint8_t      *f = &C.r;
                for(size_t i=0;i<3;++i)
                {
                    const char  *field = fields[i];
                    const size_t value = string_convert::to<size_t>(words[2+i],field);
                    if(value>255) throw exception("%s is too big for %s",field,*id);
                    f[i] = uint8_t(value);
                }
                std::cerr << id << " => " << C << std::endl;
                const Color cc(id,C);
                if(!db.insert(cc))
                {
                    throw exception("multiple '%s'", *cc.n);
                }
                maxlen = max_of(maxlen,id.size());
            }
        }

        std::cerr << "#colors=" << db.size() << std::endl;
        const unsigned nc = unsigned(db.size());

        {
            ios::ocstream fp("named-inc.hpp");

            fp << "#define Y_GFX_NAMED_COLORS ";
            fp("%u\n",nc);

            unsigned indx = 0;
            for(ColorDB::iterator it=db.begin();it!=db.end();++it,++indx)
            {
                const Color &C = *it;
                fp << "#define " << C.n;
                for(size_t i=C.n.size();i<=maxlen+6;++i) fp << ' ';
                fp << "graphic::rgb";
                fp("(%3u,%3u,%3u)",C.c.r,C.c.g,C.c.b);
                fp << "\n";
                fp << "#define " << C.n << "_INDEX";
                for(size_t i=C.n.size();i<=maxlen;++i) fp << ' ';
                fp("%-3u\n",indx);

            }
        }

        {
            ios::ocstream fp("named-tab.hxx");
            size_t indx=1;
            for(ColorDB::iterator it=db.begin();it!=db.end();++it,++indx)
            {
                const Color &C = *it;
                fp << C.n;
                if(indx<db.size()) fp << ',';
                fp << '\n';

            }
        }


    }

}
Y_UTEST_DONE()

