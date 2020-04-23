#include "y/jargon/dialect.hpp"
#include "y/exception.hpp"
#include "y/jargon/token/convert.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        string Dialect:: readCID(const XNode &ctl) const
        {
            static const char fn[] = "::readCID: ";
            const XList &args = ctl.children;
            if(args.size<=0) throw exception("%s%sno CommandID in control node",**title,fn);
            const XNode &cid  = *args.head;
            if( "cid" != cid.name() ) throw exception("%s%scontrol node cid!=%s",**title,fn,*cid.name());
            string data;
            if(!cid.query(data,1,0)) throw exception("%s%sno control node command",**title,fn);
            return data;
            
        }
        
        XNode *Dialect::checkIncludes(XNode        *root,
                                      const string &fileName)
        {
            
            auto_ptr<XNode> guard(root);
            assert(root);
            assert(root->name()=="dialect");
            Y_JAXIOM(std::cerr << title << " checkIncludes" <<std::endl);
            
            // include
            XList &children = root->children;
            {
                XList temp;
                while(children.size>0)
                {
                    auto_ptr<XNode> chld = children.pop_front();
                    if( chld->name() == "ctl" )
                    {
                        const string cid = readCID(*chld);
                        if( "include" == cid)
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
            static const char fn[] = "::include: ";
            static const int  relative = 1;
            static const int  absolute = 2;
            
            const XList &args = ctrl.children;
            assert(args.size>=1);
            assert(args.head->name() == "cid");
            assert(args.head->lexeme.is_valid());
            
            //! check 1 arg
            if(2!=args.size) throw exception("%s%srequires a single filename",**title,fn);
            const XNode  &node = *args.tail;
            const string &kind = node.name();
            int           flag = 0;
            if( kind == "RX" )
            {
                flag = relative;
                goto INCLUDE;
            }
            
            if( kind == "RS" )
            {
                flag = absolute;
                goto INCLUDE;
            }
           
            throw exception("%s%sunexpected argument '%s'", **title,fn,*kind);
            
        INCLUDE:
            std::cerr << "using " << kind << std::endl;
            
            
            throw exception("not implemented");
        }
        
        
    }
    
}

