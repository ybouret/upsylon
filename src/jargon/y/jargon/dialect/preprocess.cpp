#include "y/jargon/dialect.hpp"
#include "y/exception.hpp"
#include "y/jargon/token/convert.hpp"
#include "y/fs/vfs.hpp"

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
        
        namespace {
            
            enum includePath
            {
                includeRelative,
                includeAbsolute
            };
            
            static inline
            includePath includePathFrom(const string &kind,
                                        const char   *gm,
                                        const char   *fn)
            {
                if( kind == "RX" )
                {
                    return includeRelative;
                }
                
                if( kind == "RS" )
                {
                    return includeAbsolute;
                }
                
                throw exception("%s%sunexpected argument '%s'",gm,fn,*kind);
            }
        }
        
        XNode * Dialect:: include(const XNode  &ctrl,
                                  const string &fileName)
        {
            static const char fn[] = "::include: ";
            
            //------------------------------------------------------------------
            // check args
            //------------------------------------------------------------------
            const XList &args = ctrl.children;
            
            assert(args.size>=1);
            assert(args.head->name() == "cid");
            assert(args.head->ties("#include"));
            
            //------------------------------------------------------------------
            // check 1 arg
            //------------------------------------------------------------------
            if(2!=args.size) throw exception("%s%srequires a single filename",**title,fn);
            
            //------------------------------------------------------------------
            // extract info: kind and include name
            //------------------------------------------------------------------
            const XNode      &node = *args.tail;
            const includePath flag = includePathFrom( node.name(), **title, fn);
            string            include;
            if(!node.query(include)) throw exception("%s%sunexpected no include name",**title,fn);
            std::cerr << "include=''" << include << "''" << std::endl;

            //------------------------------------------------------------------
            // and compile again!
            //------------------------------------------------------------------
            switch(flag)
            {
                case includeRelative:
                {
                    const string subName = vfs::get_file_dir(fileName) + include;
                    std::cerr << "load ''" << subName << "''" << std::endl;
                    return compileFile(subName);
                }
                case includeAbsolute:
                    break;
            }
            
            throw exception("%s%snot implemented!",**title,fn);
            
        }
        
        
    }
    
}

