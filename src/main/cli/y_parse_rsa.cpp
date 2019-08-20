#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/mpl/rsa/keys.hpp"
#include "y/associative/set.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"
#include "y/string/tokenizer.hpp"
#include "y/ptr/intr.hpp"
#include "y/iterate/for-each.hpp"

using namespace upsylon;

namespace
{
    static inline bool is_sep( const int C ) throw()
    {
        //return C == ' ' || C == '\t';
        return C == ':';
    }

    static inline bool is_blank( const int C ) throw()
    {
        return C == ' ' || C == '\t';
    }

    class Entry : public counted_object
    {
    public:
        typedef intr_ptr<string,Entry> Pointer;

        const string name;
        string       content;
        mpn          value;

        inline explicit Entry( const string &id ) : name(id), content(), value() {}
        inline virtual ~Entry() throw() {}

        const string & key() const throw() { return name; }

        friend std::ostream & operator<<( std::ostream &os, const Entry &entry )
        {
            os << '<' << entry.name << '>' << '=' << entry.content;
            return os;
        }

        inline void compile()
        {
            if(content.size()>0)
            {
                value = mpn::parse(content);
                //std::cerr << name << " : " << value << std::endl;
            }
        }

        static inline void Compile( Entry &entry )
        {
            entry.compile();
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Entry);
    };

    typedef set<string,Entry::Pointer,key_hasher<string>,memory::pooled> Entries;

    static inline
    Entry *NewEntryIn( Entries &entries, const string &id )
    {
        Entry               *entry = new Entry(id);
        const Entry::Pointer ep    = entry;
        if(!entries.insert(ep))
        {
            throw exception("Multiple Entry '%s", *id );
        }

        return entry;
    }

    static inline
    const mpn & Fetch( const char *id, const Entries &entries )
    {
        const string _(id);
        const Entry::Pointer *epp = entries.search(_);
        if(!epp) throw exception("Missing '%s'", *_);
        return (**epp).value;
    }

#define FETCH(ID) Fetch(#ID,entries)

}

#define LPAREN '('

Y_PROGRAM_START()
{

    Entries entries;

    {
        ios::icstream                 fp( ios::cstdin );
        string                        line;
        vector<string,memory::pooled> strings;
        Entry                        *curr = NULL;
        while( fp.gets(line) )
        {
            const size_t ns = tokenizer<char>::split(strings, line, is_sep);
            if(ns<=0) continue;

            const string id = strings.front();
            if( isblank(id[0]) )
            {
                // append to current entry
                if(!curr)
                {
                    throw exception("No Current Entry for content!");
                }
                for(size_t i=1;i<=ns;++i)
                {
                    tokenizer<char> tkn(strings[i]);
                    while( tkn.next(is_blank) )
                    {
                        const string data( tkn.token(), tkn.units() );
                        curr->content += data;
                    }
                }
            }
            else
            {
                // create new entry
                curr = NewEntryIn(entries,id);
                if( strings.size() > 1 )
                {
                    // single line -> dec
                    line           = strings[2];
                    const size_t n = tokenizer<char>::split(strings,line,is_blank);
                    if(n<=0) throw exception("No Valid content for %s", *id );
                    const string &head = strings.front();
                    if(head[0]!=LPAREN)
                    {
                        curr->content = strings[1];
                    }
                }
                else
                {
                    // mutliple lines -> hexa
                    curr->content = "0x";
                }
            }
        }
    }

    std::cerr << "Entries: " << entries << std::endl;
    for_each_meta( entries, Entry::Compile );

    RSA::SharedKey prv = new RSA::PrivateKey(FETCH(modulus),
                                             FETCH(publicExponent),
                                             FETCH(privateExponent),
                                             FETCH(prime1),
                                             FETCH(prime2),
                                             FETCH(exponent1),
                                             FETCH(exponent2),
                                             FETCH(coefficient)
                                             );
    //prv->print(std::cerr);

}
Y_PROGRAM_END()


