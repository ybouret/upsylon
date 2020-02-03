
#include "y/lang/dynamo/node.hpp"
#include "y/exception.hpp"
#include "y/string/convert.hpp"
#include "y/string/io.hpp"
#include "y/ios/tools/graphviz.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/serialized.hpp"

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
            const string s = lx.toString(nskip,ntrim);
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
        
        std::ostream & DynamoNode:: output( std::ostream &os, int level ) const
        {
            switch(type)
            {
                case DynamoTerminal: {
                    const string s = string_convert::to_visible( content() );
                    Indent(os,level) << "[term] <" << name << ">";
                    if(s.size()>0)
                    {
                        os << " = '" << s << "'";
                    }
                    os << std::endl;
                } break;
                    
                case DynamoInternal: {
                    const DynamoList &self = children();
                    Indent(os,level) << "[call] <" << name << ">/" << self.size << std::endl;
                    ++level;
                    for(const DynamoNode *node = self.head; node; node=node->next )
                    {
                        node->output(os,level);
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
        
        void DynamoNode:: vizCore( ios::ostream &fp ) const
        {
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
                        node->vizSave(fp);
                        this->vizJoin(fp,node);
                        if(multiple)
                        {
                            fp("[label=\"%u\"]",idx);
                        }
                        endl(fp);
                    }
                } break;
            }
        }
        
        

        size_t DynamoNode:: serialize( ios::ostream &fp ) const
        {
            size_t ans = name.serialize(fp);

            switch(type)
            {
                case DynamoTerminal: {
                    Y_OSTREAM_ADD_TO(ans,fp.emit_net<uint8_t>,0);
                    const string tmp = content();
                    ans += tmp.serialize(fp);
                } break;

                case DynamoInternal: {
                    Y_OSTREAM_ADD_TO(ans,fp.emit_net<uint8_t>,1);
                    const DynamoList &ch = children();
                    const size_t      nch = ch.size;
                    Y_OSTREAM_ADD_TO(ans,fp.emit_upack,nch);
                    for(const DynamoNode *node=ch.head;node;node=node->next)
                    {
                        ans += node->serialize(fp);
                    }
                } break;
            }

            return ans;
        }

        const char *DynamoNode:: className() const throw()
        {
            return "DynamoNode";
        }


        

        DynamoNode * DynamoNode:: Load_( Source &fp )
        {
            static const char fn[] = "DynamoNode::Load";
            
            const string   theName = string_io::load_binary(fp);
            const unsigned theType = fp.read_net<uint8_t>();
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
                        ch.push_back( Load_(fp) );
                    }
                    return node.yield();
                }
                    
                default:
                    break;
            }
            throw exception("%s(invalid DynamoType=%u for '%s')", fn, theType, *theName);
        }
        
        DynamoNode * DynamoNode:: Load( Module *m )
        {
            assert(m);
            Source source(m);
            return Load_(source);
        }

        std::ostream & operator<<( std::ostream &os, const DynamoNode &node )
        {
            return node.output(os,0);
        }
        
    }
}
