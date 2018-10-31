#include "y/program.hpp"
#include "y/lang/dynamo.hpp"
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

    static inline void format_def(ios::ostream          &fp,
                                  const array<string>   &def,
                                  const array<int32_t>  &hid)
    {
        const size_t n = def.size();
        size_t max_len = 0;
        for(size_t i=n;i>0;--i)
        {
            max_len = max_of<size_t>(max_len,def[i].size());
        }

        for(size_t i=1;i<=n;++i)
        {
            fp("#define ").align(def[i],max_len)(" 0x%08x\n", hid[i]);
        }
    }

    static inline
    string compile_mph(const array<string>  &sym,
                       const array<int32_t> &hid)
    {
        string           ans;
        ios::osstream    fp(ans);
        const size_t     n = sym.size();

        fp.emit_upack(n);
        for(size_t i=1;i<=n;++i)
        {
            string_io::save_binary(fp,sym[i]);
            fp.emit_upack( unsigned(hid[i]) );
        }

        return ans;
    }

    static inline
    void emit_table(ios::ostream &fp,
                    const string &name,
                    const string &data,
                    ios::bin2dat &b2d)
    {
        fp << "static const unsigned char " << name << "[";
        fp("%u",unsigned(data.size())) << "]={\n";
        b2d.reset();
        b2d.write(fp,data);
        fp << "};\n\n";
    }

}

Y_PROGRAM_START()
{
    vfs            &fs = local_fs::instance();
    DynamoCompiler  dynCompiler;
    DynamoGenerator dynGenerator;


    if(argc>1)
    {
        fs.try_remove_file("grammar.dot");
        fs.try_remove_file("grammar.png");
        fs.try_remove_file("parser.dot");
        fs.try_remove_file("parser.png");

        //______________________________________________________________________
        //
        // load the full processed AST
        //______________________________________________________________________
        const string             grammarFile = argv[1];
        auto_ptr<Syntax::Node>   grammarAST  = dynCompiler.process( Module::OpenFile(grammarFile) );
        const string             grammarBin  = grammarAST->to_binary();
        //grammarAST->GraphViz("grammar.dot");

        //______________________________________________________________________
        //
        // generate a valid parser
        //______________________________________________________________________
        auto_ptr<Syntax::Parser> parser = dynGenerator.create(*grammarAST);
        //parser->GraphViz("parser.dot");

        //______________________________________________________________________
        //
        // get definitions for terminals
        //______________________________________________________________________
        Dynamo::Hash31 h31;

        vector<string>   terminal_def;
        vector<string>   terminal_sym;
        vector<int32_t>  terminal_hid;
        hashing::mperf   hTerminal;

        for( DynamoGenerator::Terminals::iterator i=dynGenerator.terminals.begin(); i != dynGenerator.terminals.end(); ++i)
        {
            const DynamoGenerator::_Terminal &t = **i;
            if(t.visible)
            {
                const string &tName = t.rule.name;               // that will appear in analyzer
                string        sName = *(t.module) + '_' + tName; // that will be used for definitions
                make_cpp(sName);
                hTerminal.insert(tName,h31[tName]);

                terminal_def << sName;
                terminal_sym << tName;
                terminal_hid << hTerminal(tName);
            }
        }

        for(size_t i=terminal_sym.size();i>0;--i)
        {
            if( hTerminal( terminal_sym[i] ) != terminal_hid[i] ) throw exception("mismatch terminal '%s' level-1", *terminal_sym[i]);
        }

        const string terminal_mph = compile_mph(terminal_sym,terminal_hid);
        {
            const hashing::mperf table(terminal_mph.ro(),terminal_mph.length());
            for(size_t i=terminal_sym.size();i>0;--i)
            {
                if( table( terminal_sym[i] ) != terminal_hid[i] ) throw exception("mismatch terminal '%s' level-2", *terminal_sym[i]);
            }
        }
        //______________________________________________________________________
        //
        // get definitions for internals
        //______________________________________________________________________
        vector<string>  internal_def;
        vector<string>  internal_sym;
        vector<int32_t> internal_hid;
        hashing::mperf  hInternal;
        for( DynamoGenerator::Internals::iterator i=dynGenerator.internals.begin(); i != dynGenerator.internals.end(); ++i)
        {
            const DynamoGenerator::_Internal &c = **i;
            const string &iName = c.rule.name;
            string        sName = *(c.module) + '_' + iName; //
            make_cpp(sName);
            hInternal.insert(iName,h31[iName]);

            internal_def << sName;
            internal_sym << iName;
            internal_hid << hInternal(iName);
        }

        for(size_t i=internal_sym.size();i>0;--i)
        {
            if( hInternal( internal_sym[i] ) != internal_hid[i] ) throw exception("mismatch internal '%s' level-1", *internal_sym[i]);
        }

        const string internal_mph = compile_mph(internal_sym,internal_hid);
        {
            const hashing::mperf table(internal_mph.ro(),internal_mph.length());
            for(size_t i=internal_sym.size();i>0;--i)
            {
                if( table( internal_sym[i] ) != internal_hid[i] ) throw exception("mismatch internal '%s' level-2", *internal_sym[i]);
            }
        }


        //______________________________________________________________________
        //
        // emit definitions
        //______________________________________________________________________
        const string  name = *(parser->name);
        {
            string       grammarDef = grammarFile; vfs::change_extension(grammarDef, "def");
            std::cerr << "-- saving into '" << grammarDef << "'" << std::endl;
            ios::ocstream fp(grammarDef);
            const size_t  nsep = 17 + name.size();
            (fp << '/' << '/').repeat(nsep, '-')<<'\n';
            fp << "// TERMINALS for {" << name << "}\n";
            (fp << '/' << '/').repeat(nsep, '-')<<'\n';
            format_def(fp,terminal_def,terminal_hid);
            fp << '\n';

            (fp << '/' << '/').repeat(nsep, '-')<<'\n';
            fp << "// INTERNALS for {" << name << "}\n";
            (fp << '/' << '/').repeat(nsep, '-')<<'\n';
            format_def(fp,internal_def,internal_hid);
            fp << '\n';
        }

        //______________________________________________________________________
        //
        // emit code generator
        //______________________________________________________________________
        {
            ios::bin2dat b2d(16);
            string       grammarInc = grammarFile; vfs::change_extension(grammarInc, "inc");
            std::cerr << "-- saving into '" << grammarInc << "'" << std::endl;
            ios::ocstream fp(grammarInc);

            // save the grammar
            {
                const string table_name = name + "_grammar";
                emit_table(fp, table_name, grammarBin, b2d);
            }

            // save the terminals table data
            {
                const string table_name = name + "_terminals";
                emit_table(fp, table_name, terminal_mph, b2d);
            }

            // save the internals table data
            {
                const string table_name = name + "_internals";
                emit_table(fp, table_name, internal_mph, b2d);
            }
        }
    }


}
Y_PROGRAM_END()

