#include "y/jargon/dialect.hpp"
#include "y/exception.hpp"
#include "y/jargon/token/convert.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        string Dialect:: readCID(const XNode &ctl) const
        {
            const XList &cmd = ctl.children;
            if(cmd.size<=0)
            {
                throw exception("%s invalid control", **title);
            }
            const XNode  &cidNode = *cmd.head;
            const string &cid     = *(cidNode.dogma->label);
            if( cid != "cid" )
            {
                throw exception("%s invalid control identifier label '%s'", **title, *cid);
            }
            const Lexeme *lexeme = cidNode.lexeme.content();
            if(!lexeme) throw exception("%s empty control identifier",**title);
            string res( lexeme->size, as_capacity, false);
            TokenConvert::To(res,*lexeme);
            return res;
        }
        
        XNode *Dialect::checkIncludes(XNode        *root,
                                      const string &fileName)
        {
            
            auto_ptr<XNode> guard(root);
            assert(root);
            assert(*root=="dialect");
            Y_JAXIOM(std::cerr << title << " checkIncludes" <<std::endl);
            
            // include
            XList &children = root->children;
            {
                XList temp;
                while(children.size>0)
                {
                    auto_ptr<XNode> chld = children.pop_front();
                    if( *chld == "ctl" )
                    {
                        const string cid = readCID(*chld);
                        if( "#include" == cid)
                        {
                            temp.push_back( include(*chld, fileName) );
                        }
                    }
                    temp.push_back( chld.yield() );
                }
                children.swap_with(temp);
            }
            
            
            return guard.yield();
        }
        
        XNode * Dialect:: include(const XNode &ctrl, const string &fileName)
        {
            const XList &args = ctrl.children;
            assert(args.size>=1);
            assert(*args.head == "cid");
            assert(args.head->lexeme.is_valid());
            
            throw exception("not implemented");
        }
        
        
    }
    
}

