#include "y/program.hpp"
#include "y/string/temporary-name.hpp"
#include "y/fs/local/fs.hpp"
#include "y/ios/ocstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"
#include <cstdlib>

using namespace upsylon;

////////////////////////////////////////////////////////////////////////////////
//
// system calls to generator rsa keys with openssl
// use msys-coreutils and msys-openssl for windows
//
////////////////////////////////////////////////////////////////////////////////
namespace
{
    static inline
    void perform( const string &cmd )
    {
        std::cerr << "..executing   [" << cmd << "]" << std::endl;

        if( system( *cmd ) != 0 )
        {
            throw exception("failed <%s>", *cmd );
        }
    }

    static inline
    void try_remove( const string &filename )
    {
        vfs &fs = local_fs::instance();
        std::cerr << "..suppressing [" << filename << "]" << std::endl;
        fs.try_remove_file( filename );
    }

    static inline
    void try_remove( const array<string> &filenames )
    {
        for(size_t i=filenames.size();i>0;--i)
        {
            try_remove(filenames[i]);
        }
    }


    string generate_openssl_private_key( const size_t bits )
    {
        vector<string,memory::pooled> filenames(3,as_capacity);
        temporary_name::create(filenames,3);

        const string & randname = filenames[1];     // reference to entropy file
        const string & osslname = filenames[2];     // reference to binary file
        const string   filename = filenames.back(); // copy

        try
        {
            // get entropy
            {
                const string cmd = "dd if=/dev/random bs=1024 count=1 of=" + randname + " &> /dev/null";
                perform(cmd);
            }

            // generate binary key
            {
                const string cmd = "openssl genrsa -out " + osslname + " -rand " + randname + vformat(" %u", unsigned(bits) ) + " &> /dev/null";
                perform(cmd);
            }

            // generating test file now
            {
                const string cmd = "openssl rsa -text -noout -in " + osslname + " -out " + filename + " &> /dev/null";
                perform(cmd);
            }

        }
        catch(...)
        {
            try_remove(filenames);
            throw;
        }

        filenames.pop_back();
        try_remove(filenames);
        std::cerr << "..built in    [" << filename << "]" << std::endl;
        return filename;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// parse generated file
//
////////////////////////////////////////////////////////////////////////////////
#include "y/ios/icstream.hpp"
#include "y/mpl/rsa/keys.hpp"
#include "y/associative/set.hpp"
#include "y/string/tokenizer.hpp"
#include "y/ptr/intr.hpp"
#include "y/iterate/for-each.hpp"
#include "y/ptr/auto.hpp"

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

#if 0
        friend std::ostream & operator<<( std::ostream &os, const Entry &entry )
        {
            os << '<' << entry.name << '>' << '=' << entry.content;
            return os;
        }
#endif
        inline void compile()
        {
            if(content.size()>0)
            {
                value = mpn::parse(content);
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
#define LPAREN '('

    static inline
    RSA::Key *parse_openssl_private_key( const string &filename )
    {
        try
        {
            Entries entries;

            {
                ios::icstream                 fp( filename);
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

            //std::cerr << "Entries: " << entries << std::endl;
            for_each_meta( entries, Entry::Compile );

            auto_ptr<RSA::Key> prv = new RSA::PrivateKey(FETCH(modulus),
                                                         FETCH(publicExponent),
                                                         FETCH(privateExponent),
                                                         FETCH(prime1),
                                                         FETCH(prime2),
                                                         FETCH(exponent1),
                                                         FETCH(exponent2),
                                                         FETCH(coefficient)
                                                         );
            try_remove(filename);
            return prv.yield();
        }
        catch(...)
        {
            try_remove(filename);
            throw;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// mixed call
//
////////////////////////////////////////////////////////////////////////////////
namespace
{
    RSA::Key *generate_key( const size_t nbits )
    {
        const string       filename = generate_openssl_private_key(nbits);
        return  parse_openssl_private_key(filename);
    }
}

#include "y/string/convert.hpp"

Y_PROGRAM_START()
{
    ios::ocstream fp( ios::cstdout );
    for(int i=1;i<argc;++i)
    {
        const size_t       nbits = string_convert::to<size_t>(argv[i],"nbits");
        for(size_t j=1;j<=2;++j)
        {
            auto_ptr<RSA::Key> key   = generate_key(nbits);
            key->serialize( fp );
        }
    }

}
Y_PROGRAM_END()

