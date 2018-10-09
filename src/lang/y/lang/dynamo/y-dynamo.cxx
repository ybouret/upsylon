#include "y/program.hpp"
#include "y/lang/dynamo/generator.hpp"
#include "y/lang/dynamo/compiler.hpp"
#include "y/ios/bin2dat.hpp"
#include "y/fs/local.hpp"
#include "y/ios/osstream.hpp"
#include "y/string/io.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{
    static inline void make_cpp( string &name )
    {
        for(size_t i=0;i<name.size();++i)
        {
            const char C = name[i];
            if(isalnum(C)) continue;
            name[i] = '_';
        }
    }

    static inline void format_def(ios::ostream        &fp,
                                  const array<string> &def,
                                  const array<int>    &hid,
                                  const size_t         terms)
    {
        const size_t n = def.size();
        assert(terms<=n);
        size_t max_len = 0;
        for(size_t i=n;i>0;--i)
        {
            max_len = max_of<size_t>(max_len,def[i].size());
        }
        fp << "// TERMINALS\n";
        for(size_t i=1;i<=terms;++i)
        {
            const string &s = def[i];
            fp("#define ");
            fp << s;
            for(size_t j=s.size();j<=max_len;++j) fp << ' ';
            fp("0x%04x\n", hid[i]);
        }
        fp << '\n';
        fp << "// INTERNALS\n";
        for(size_t i=terms+1;i<=n;++i)
        {
            const string &s = def[i];
            fp("#define ");
            fp << s;
            for(size_t j=s.size();j<=max_len;++j) fp << ' ';
            fp("0x%04x\n", hid[i]);
        }

    }

}

Y_PROGRAM_START()
{
    vfs            &fs = local_fs::instance();
    DynamoCompiler  dynCompiler;
    DynamoGenerator dynGenerator;


    if(argc>1)
    {
        //______________________________________________________________________
        //
        // creating the corresponding parser
        //______________________________________________________________________
        const string             grammarFile = argv[1];
        auto_ptr<Syntax::Node>   grammarAST  = dynCompiler.process( Module::OpenFile(grammarFile) );
        grammarAST->GraphViz("dynast.dot");

        //______________________________________________________________________
        //
        // saving the binary data
        //______________________________________________________________________
        {
            const string grammarBin = grammarAST->to_binary();
            string       grammarInc = grammarFile; vfs::change_extension(grammarInc, "inc");
            std::cerr << "-- saving into '" << grammarInc << "'" << std::endl;
            fs.try_remove_file(grammarInc);
            {
                ios::ocstream fp(grammarInc);
                ios::bin2dat  b2d(16);
                b2d.write(fp,grammarBin);
            }
        }

        auto_ptr<Syntax::Parser> parser = dynGenerator.create(*grammarAST);
        parser->GraphViz("dynout.dot");
        std::cerr << "Terminals: " << dynGenerator.terminals << std::endl;
        std::cerr << "Internals: " << dynGenerator.internals << std::endl;

        //______________________________________________________________________
        //
        // create the definitions
        //______________________________________________________________________
        {
            string       grammarDef = grammarFile; vfs::change_extension(grammarDef, "def");
            std::cerr << "-- saving into '" << grammarDef << "'" << std::endl;
            hashing::mperf table;
            int            index=0;

            vector<string> def;
            vector<string> sym;
            vector<int>    hid;
            size_t         terms = 0;
            for( DynamoGenerator::Terminals::iterator i=dynGenerator.terminals.begin(); i != dynGenerator.terminals.end(); ++i)
            {
                const DynamoGenerator::_Terminal &t = **i;
                if(t.visible)
                {
                    const string &tName = t.rule.name;               // that will appear in analyzer
                    string        sName = *(t.module) + '_' + tName; //
                    make_cpp(sName);
                    table.insert(tName,index++);
                    def << sName;
                    sym << tName;
                    hid << table(tName);
                    ++terms;
                }
            }


            for( DynamoGenerator::Internals::iterator i=dynGenerator.internals.begin(); i != dynGenerator.internals.end(); ++i)
            {
                const DynamoGenerator::_Internal &c = **i;
                const string &iName = c.rule.name;
                string        sName = *(c.module) + '_' + iName; //
                make_cpp(sName);
                table.insert(iName,(index++) );
                def << sName;
                sym << iName;
                hid << table(iName);
            }

            {
                ios::ocstream fp( ios::cstderr );
                format_def(fp,def,hid,terms);
            }

            string mph_data;
            {
                ios::osstream fp(mph_data);
                const size_t n = sym.size();
                fp.emit_upack(n);
                for(size_t i=1;i<=n;++i)
                {
                    string_io::save_binary(fp,sym[i]);
                }
            }

            {
                const hashing::mperf mph( mph_data.ro(), mph_data.length() );
                for(size_t i=sym.size();i>0;--i)
                {
                    const string &s = sym[i];
                    if( mph(s) != hid[i] ) throw exception("invalid rehashing of '%s'", *s);
                }
            }
        }


    }


}
Y_PROGRAM_END()

