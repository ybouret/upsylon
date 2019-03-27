
#include "y/lang/dynamo/compiler.hpp"
#include "y/exception.hpp"
#include "y/string/convert.hpp"
#include "y/string/io.hpp"
#include "y/ios/graphviz.hpp"
#include "y/ios/osstream.hpp"
#include "y/ios/null-ostream.hpp"

namespace upsylon
{
    namespace Lang
    {
        DynamoNode:: ~DynamoNode() throw()
        {
            assert(impl);
            switch(type)
            {
                case DynamoInternal:
                    delete static_cast<DynamoList *>(impl);
                    break;

                case DynamoTerminal:
                    delete static_cast<string*>(impl);
                    break;
            }
            impl=0;
        }

        DynamoNode:: DynamoNode(const string &id,
                                const Lexeme &lx,
                                const size_t  nskip,
                                const size_t  ntrim) :
        type( DynamoTerminal ),
        name(id),
        impl( NULL )
        {
            const string s = lx.to_string(nskip,ntrim);
            impl = new string(s);
        }

        const string & DynamoNode:: content() const throw()
        {
            assert(DynamoTerminal==type);
            assert(NULL!=impl);
            return *static_cast<const string *>(impl);
        }

        DynamoList   &  DynamoNode:: children() throw()
        {
            assert(DynamoInternal==type);
            assert(NULL!=impl);
            return *static_cast<DynamoList *>(impl);
        }

        const DynamoList   &  DynamoNode:: children() const throw()
        {
            assert(DynamoInternal==type);
            assert(NULL!=impl);
            return *static_cast<const DynamoList *>(impl);
        }


        DynamoNode:: DynamoNode( const string &id ) :
        type( DynamoInternal ),
        name(id),
        impl( new DynamoList() )
        {

        }

        std::ostream & DynamoNode:: display( std::ostream &os, int level ) const
        {
            switch(type)
            {
                case DynamoTerminal: {
                    const string s = string_convert::to_visible( content() );
                    Indent(os,level) << "[term] <" << name << ">";
                    if(s.size()>0)
                    {
                        os << "='" << s << "'";
                    }
                    os << std::endl;
                } break;

                case DynamoInternal: {
                    const DynamoList &self = children();
                    Indent(os,level) << "[call] <" << name << ">/" << self.size << std::endl;
                    ++level;
                    for(const DynamoNode *node = self.head; node; node=node->next )
                    {
                        node->display(os,level);
                    }
                    --level;
                } break;
            }
            return os;
        }

        std::ostream & DynamoNode:: Indent(std::ostream &os, int level)
        {
            const string s = vformat("x%02x",level);
            os << '|' << s << '|';
            for(;level>0;--level)
            {
                os << '_' << '_';
            }
            return os;
        }

        void DynamoNode:: viz( ios::ostream &fp ) const
        {
            fp.viz(this);
            const string l = string_convert::to_visible(name);
            switch (type) {
                case DynamoTerminal: {
                    string s = content();
                    s = string_convert::to_visible(s);
                    if(s.size()>0)
                    {
                        fp("[label=\"%s='%s'\"];\n",*l,*s);
                    }
                    else
                    {
                        fp("[label=\"%s\"];\n",*l);
                    }

                } break;

                case DynamoInternal:{
                    fp("[label=\"%s\"];\n",*l);
                    const bool multiple = children().size>0;
                    unsigned   idx      = 1;
                    for( const DynamoNode *node=children().head;node;node=node->next,++idx)
                    {
                        node->viz(fp);
                        fp.viz(this); fp << "->"; fp.viz(node);
                        if(multiple)
                        {
                            fp("[label=\"%u\"]",idx);
                        }
                        fp << ';' << '\n';
                    }
                } break;
            }
        }

        void DynamoNode:: graphViz(const string &filename) const
        {
            {
                ios::ocstream fp(filename);
                fp << "digraph G {\n";
                viz(fp);
                fp << "}\n";
            }
            (void)ios::GraphViz::Render(filename);

        }

        void DynamoNode:: save( ios::ostream &fp , size_t *bytes) const
        {
            const size_t nl = string_io::save_binary(fp,name);
            if(bytes) (*bytes) += nl;
            switch (type)
            {
                case DynamoTerminal: {
                    fp.emit<uint8_t>(0);        if(bytes) ++(*bytes);
                    const string s  = content();
                    const size_t sz = string_io::save_binary(fp,s);
                    if(bytes) (*bytes) += sz;
                } break;

                case DynamoInternal: {
                    fp.emit<uint8_t>(1); if(bytes) ++(*bytes);
                    const DynamoList &ch = children();
                    const size_t      nch = ch.size;
                    size_t            sz  = 0;
                    fp.emit_upack(nch,&sz); if(bytes) (*bytes) += sz;
                    for(const DynamoNode *node=ch.head;node;node=node->next)
                    {
                        node->save(fp,bytes);
                    }
                } break;
            }
        }
        
        size_t DynamoNode:: output_bytes() const
        {
            size_t            bytes = 0;
            ios::null_ostream fp;
            save(fp,&bytes);
            return bytes;
        }

        
        string DynamoNode:: to_string() const
        {
            string ans;
            ios::osstream fp(ans);
            save(fp);
            
            return ans;
        }



        DynamoNode * DynamoNode:: Load( Source &fp )
        {
            static const char fn[] = "DynamoNode::Load";

            const string   theName = string_io::load_binary(fp);
            const unsigned theType = fp.read<uint8_t>();
            switch( theType )
            {
                case 0: // DynamoTerminal
                {
                    const size_t nch = fp.read_upack<size_t>();
                    Lexeme       lex( (*fp)->origin );
                    for(size_t i=0;i<nch;++i)
                    {
                        Char *ch = fp.get();
                        if(!ch) throw exception("%s(missing chars for '%s')",fn,*theName);
                        lex.push_back(ch);
                    }
                    return new DynamoNode(theName,lex,0,0);
                }


                case 1: // DynamoInternal
                {
                    const size_t         nch  = fp.read_upack<size_t>();
                    auto_ptr<DynamoNode> node = new DynamoNode(theName);
                    DynamoList          &ch   = node->children();
                    for(size_t i=0;i<nch;++i)
                    {
                        ch.push_back( Load(fp) );
                    }
                    return node.yield();
                }

                default:
                    break;
            }
            throw exception("%s(invalid DynamoType=%u for '%s')", fn, theType, *theName);
        }

        void DynamoNode::  run( hashing::function &H ) const throw()
        {
            static const uint8_t TermHash = 0;
            static const uint8_t RuleHash = 1;
            H(name);
            switch (type)
            {
                case DynamoTerminal:
                    H.run_type(TermHash);
                    H(content()); break;

                case DynamoInternal:
                    H.run_type(RuleHash);
                    for(const DynamoNode *node=children().head;node;node=node->next)
                    {
                        node->run(H);
                    }
                    break;
            }
        }

        digest DynamoNode:: md( hashing::function &H ) const
        {
            H.set();
            run(H);
            return H.md();
        }

    }
}
