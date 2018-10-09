#include "y/program.hpp"
#include "y/lang/dynamo.hpp"h
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


    static inline
    void emit_def(ios::ostream   &fp,
                  const string   &s,
                  const uint32_t  v,
                  const size_t    max_len)
    {
        fp("#define ");
        fp << s;
        for(size_t j=s.size();j<=max_len;++j) fp << ' ';
        fp("0x%08x\n", v);
    }

    static inline void format_def(ios::ostream          &fp,
                                  const string          &name,
                                  const array<string>   &def,
                                  const array<int32_t> &hid,
                                  const size_t          terms)
    {
        const size_t n = def.size();
        assert(terms<=n);
        fp << '\n';
        size_t max_len = 0;
        for(size_t i=n;i>0;--i)
        {
            max_len = max_of<size_t>(max_len,def[i].size());
        }

        fp << "// TERMINALS for <" << name << ">\n";
        for(size_t i=1;i<=terms;++i)
        {
            emit_def(fp,def[i],hid[i],max_len);
        }
        fp << '\n';
        fp << "// INTERNALS for <" << name << ">\n";
        for(size_t i=terms+1;i<=n;++i)
        {
            emit_def(fp,def[i],hid[i],max_len);
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
        grammarAST->GraphViz("grammar.dot");

        //______________________________________________________________________
        //
        // generate a valid parser
        //______________________________________________________________________
        auto_ptr<Syntax::Parser> parser = dynGenerator.create(*grammarAST);
        parser->GraphViz("parser.dot");

        //______________________________________________________________________
        //
        // get definitions for terminals and internals
        //______________________________________________________________________
        vector<string>   def;
        vector<string>   sym;
        vector<int32_t>  hid;
        
        size_t         terms = 0;
        hashing::mperf table;
        Dynamo::Hash31 h31;

        for( DynamoGenerator::Terminals::iterator i=dynGenerator.terminals.begin(); i != dynGenerator.terminals.end(); ++i)
        {
            const DynamoGenerator::_Terminal &t = **i;
            if(t.visible)
            {
                const string &tName = t.rule.name;               // that will appear in analyzer
                string        sName = *(t.module) + '_' + tName; // that will be used for definitions
                make_cpp(sName);
                table.insert(tName,h31(tName));

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
            table.insert(iName,h31(iName));
            def << sName;
            sym << iName;
            hid << table(iName);
        }

        table.optimize();
        for(size_t i=sym.size();i>0;--i)
        {
            const string &s = sym[i];
            if( table(s) != int(hid[i]) ) throw exception("mismatch code for '%s', level-1", *s);
        }

        //______________________________________________________________________
        //
        // create symbol hash table data
        //______________________________________________________________________
        string tdata;
        {
            ios::osstream fp(tdata);
            const size_t  ns = sym.size();
            fp.emit_upack(ns);
            for(size_t i=1;i<=ns;++i)
            {
                string_io::save_binary(fp,sym[i]);
                fp.emit_upack(hid[i]);
            }
        }

        //______________________________________________________________________
        //
        // checking table
        //______________________________________________________________________
        {
            const hashing::mperf mph( tdata.ro(), tdata.length() );
            for(size_t i=sym.size();i>0;--i)
            {
                const string &s = sym[i];
                if( mph(s) != int(hid[i]) ) throw exception("mismatch code for '%s', level-2", *s);
            }
        }


        //______________________________________________________________________
        //
        // emit definitions
        //______________________________________________________________________
        {
            string       grammarDef = grammarFile; vfs::change_extension(grammarDef, "def");
            std::cerr << "-- saving into '" << grammarDef << "'" << std::endl;
            ios::ocstream fp(grammarDef);
            format_def(fp,*(parser->name),def,hid,terms);
        }

    }


}
Y_PROGRAM_END()

