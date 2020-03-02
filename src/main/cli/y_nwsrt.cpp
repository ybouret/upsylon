#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/arc.hpp"
#include "y/associative/map.hpp"
#include "y/fs/local/fs.hpp"

using namespace upsylon;

namespace {
    
    
    class Swap
    {
    public:
        const size_t I,J;
        
        inline Swap(const size_t i, const size_t j) throw() :
        I(i),J(j)
        {
            assert(I!=J);
        }
        
        inline ~Swap() throw()
        {
            
        }
        
        inline Swap( const Swap &swp ) throw() :
        I(swp.I), J(swp.J)
        {
        }
#if 0
        inline friend std::ostream & operator<<( std::ostream &os, const Swap &swp )
        {
            os << '[' << swp.I << "," << swp.J << ']';
            return os;
        }
#endif
        
    private:
        
    };
    
    typedef vector<Swap>      Swaps_;
    typedef arc_ptr<Swaps_>   Swaps;
    typedef map<size_t,Swaps> SwapsDB;
    
    static inline
    bool HasNextLine(string       &line,
                     ios::istream &fp )
    {
        while( fp.gets(line) )
        {
            if( line.clean_with(" \t").size() > 0 ) return true;
        }
        return false;
    }
    
    static inline
    size_t GetSwapsDimension(string &line)
    {
        tokenizer<char> tkn( line );
        if( !tkn.next_with('-') )
        {
            throw exception("can't find dimension in swap header");
        }
        const string s = tkn.to_string();
        const size_t n = string_convert::to<size_t>(s,"swap dimension");
        line.skip( s.size() + 1);
        return n;
    }
    
    
    static inline const char *NextChar( const char C, const char *input, const char *end) throw()
    {
        assert(input);
        assert(end>=input);
        while(input<end)
        {
            if( C == *input ) return input;
            ++input;
        }
        return NULL;
    }
    
    static inline Swap ParseSwap( const string &data )
    {
        const char *sep = strchr(*data, ',');
        if(!sep) throw exception("missing separator in '%s'", *data);
        const string Istr(*data,sep-*data);
        const string Jstr(sep+1);
        const size_t I = string_convert::to<size_t>(Istr,"I");
        const size_t J = string_convert::to<size_t>(Jstr,"J");
        return Swap(I,J);
    }
    
    static inline
    void ParseSwaps( Swaps_ &swaps, const string &line )
    {
        assert(swaps.size()==0);
        const char *input = *line;
        const char *end   = input + line.size();
        while( NULL != (input=NextChar('[',input,end) ) )
        {
            const char *head = input+1;
            const char *tail = NextChar(']',head,end);
            if( !tail ) throw exception("missing right bracket");
            input = tail+1;
            const string data(head,tail-head);
            swaps << ParseSwap(data);
        }
    }
    
    static inline
    bool LoadSwaps( SwapsDB &db, ios::istream &fp )
    {
    FIND_SWAP:
        // look up for next not empty line
        string line;
        if( !HasNextLine(line,fp) ) return false; // Done
        
        // extract dimension
        const size_t dim  = GetSwapsDimension(line);
        const string name = line;
        // extract content
        if( !fp.gets(line) )
        {
            throw exception("no data for swap dimension=%u", unsigned(dim) );
        }
        
        {
            static const char errFmt[] = "bad swaps format level-%d";
            int               errLvl = 0;
            if( line.size()  <=   2 ) throw exception(errFmt,++errLvl);
            if( line.front() != '[' ) throw exception(errFmt,++errLvl);
            if( line.back()  != ']' ) throw exception(errFmt,++errLvl);
            line.trim(1).skip(1);
        }
        //std::cerr << "swaps=" << line << std::endl;
        
        Swaps swp = new Swaps_(dim*dim,as_capacity);
        if(!db.insert(dim,swp))
        {
            //std::cerr << "@" << dim << " -" << name << std::endl;
            goto FIND_SWAP;
        }
        
        // parse content
        std::cerr << "@" << dim << " " << name << " : ";
        ParseSwaps(*swp,line);
        std::cerr << swp->size() << std::endl;
        return true;
        
    }
    
    static inline
    void loadAndCheck( SwapsDB &db, const string &dbName )
    {
        // parse file
        {
            ios::icstream fp( dbName );
            while( LoadSwaps(db,fp) )
            {
                
            }
        }
        
        // check swaps
        size_t i=2;
        for(SwapsDB::iterator it = db.begin();it!=db.end();++it,++i)
        {
            if(it.key() != i ) throw exception("invalid key/dim");
            const Swaps_ &swaps = **it;
            for(size_t j=swaps.size(); j>0; --j)
            {
                const Swap &swp = swaps[j];
                if(swp.I>=i) throw exception("I overflow");
                if(swp.J>=i) throw exception("J overflow");
            }
            
            
        }
    }
    
    
    
}


Y_PROGRAM_START()
{
    if( argc <= 1 ) throw exception("usage: %s nwsrt.db", program);
    
    const string  dbName = argv[1];
    SwapsDB       db(32,as_capacity);
    loadAndCheck(db,dbName);
    const string dirName = vfs::get_file_dir(dbName);
    const string subName = dirName + "network/";
    std::cerr << "Will write in " << dirName << " and " << subName << std::endl;
    
    {
        const string networkFileName = dirName + "network.hpp";
        ios::ocstream header(networkFileName);
        
        header << "//! \\file\n";
        header << "#ifndef Y_SORT_NETWORK_INCLUDED\n";
        header << "#define Y_SORT_NETWORK_INCLUDED 1\n";
        header << "#include \"y/type/bswap.hpp\"\n";
        header << "namespace upsylon {\n";
        header << "\ttemplate <size_t> struct network_sort;\n\n";
        header << "}\n";
        header << "#endif\n";
        
    }
    
    vfs &fs = local_fs::instance();
    fs.create_sub_dir(subName);
    {
        const string  allName = subName + "all.hpp";
        ios::ocstream all(allName);
        
        all << "//! \\file\n";
        all << "#ifndef Y_SORT_NETWORK_ALL_INCLUDED\n";
        all << "#define Y_SORT_NETWORK_ALL_INCLUDED 1\n";
        
        
        
        for(SwapsDB::iterator it=db.begin();it!=db.end();++it)
        {
            const unsigned dim        = unsigned(it.key());
            const Swaps_  &swaps      = **it;
            const unsigned num        = unsigned(swaps.size());
            const string   rootName   = vformat("sort%u",dim);
            const string   headerName = rootName+".hpp";
            const string   sourceName = rootName+".cpp";
            const string   headerPath = subName + headerName;
            const string   sourcePath = subName + sourceName;
            std::cerr << "creating " << headerPath << " and " << sourcePath << std::endl;
            
            all << "#include \"" << headerName << "\"\n";
            {
                ios::ocstream header( headerPath );
                
                header << "//! \\file\n";
                header << "#ifndef Y_NW_" << rootName << "_INCLUDED\n";
                header << "#define Y_NW_" << rootName << "_INCLUDED\n";
                header << "#include \"../network.hpp\"\n";
                header << "namespace upsylon {\n";
                
                {
                    header("\t\t//! network sort for %u items\n",dim);
                    header("\ttemplate <> struct network_sort<%u> {\n",dim);
                    header("\t\tstatic const size_t I[%u]; //!< I swap indices\n",num);
                    header("\t\tstatic const size_t J[%u]; //!< J swap indices\n",num);
                    
                    header("\t\t//! sort...\n");
                    header("\t\ttemplate <typename T,typename COMPARE> static inline\n");
                    header("\t\tvoid on(T *a, COMPARE &compare) throw() {\n");
                    
                    header("\t\t\tassert(NULL!=a);\n");
                    header("\t\t\tfor(size_t k=0;k<%u;++k){\n",num);
                    header("\t\t\t\tconst T &aI=a[I[k]], &aJ=a[J[k]];\n");
                    header("\t\t\t\tif(compare(aJ,aI)<0) bswap(aI,aJ);\n");
                    
                    header("\t\t\t}\n");
                    header("\t\t}\n");
                    header("\t};\n");
                }
                header << "}\n";
                header << "#endif";
                
            }
            
            {
                ios::ocstream source( sourcePath );
                source << "#include \"" << headerName << "\"\n";
                source << "namespace upsylon {\n";
                
                source("\tconst size_t network_sort<%u>::I[%u] = {\n",dim,num);
                for(size_t k=1;k<=num;++k)
                {
                    source(" %u", unsigned( swaps[k].I) );
                    if(k<num) source << ',';
                }
                source << "};\n\n";
                
                source("\tconst size_t network_sort<%u>::J[%u] = {\n",dim,num);
                for(size_t k=1;k<=num;++k)
                {
                    source(" %u", unsigned( swaps[k].J) );
                    if(k<num) source << ',';
                }
                source << "};\n\n";
                
                source << "}\n";
            }
            
            //if(dim>=5) break;
        }
        
        all << "#endif\n";

    }
    
    
    
    
    
    
}
Y_PROGRAM_END()

