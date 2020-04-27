#include "y/jargon/dialect.hpp"
#include "y/exception.hpp"
#include "y/jargon/token/convert.hpp"
#include "y/fs/vfs.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        XNode *Dialect:: expressBlocks(XNode *root)
        {
            auto_ptr<XNode> guard(root);
            
            return guard.yield();
        }
        
        XNode *Dialect::checkIncludes(XNode        *root,
                                      const string &fileName)
        {
            
            //------------------------------------------------------------------
            // scan the top-level node
            //------------------------------------------------------------------
            auto_ptr<XNode> guard(root);
            assert(root);
            assert(root->name()=="dialect");
            Y_JAXIOM(std::cerr << "[" << title << "] checkIncludes" <<std::endl);
            
            //------------------------------------------------------------------
            // look for control nodes
            //------------------------------------------------------------------
            XList &children = root->children;
            {
                XList temp;
                while(children.size>0)
                {
                    auto_ptr<XNode> chld = children.pop_front();
                    if( chld->name() == "inc" )
                    {
                        temp.push_back( include(*chld,fileName) );
                        continue;
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
                if( kind == "rx" )
                {
                    return includeRelative;
                }
                
                if( kind == "rs" )
                {
                    return includeAbsolute;
                }
                
                throw exception("%s%sunexpected argument '%s'",gm,fn,*kind);
            }
        }
        
        XNode * Dialect:: include(const XNode  &inc,
                                  const string &fileName)
        {
            static const char fn[] = "::include: ";
            
            //------------------------------------------------------------------
            // check args
            //------------------------------------------------------------------
            const XList &args = inc.children;
            
            //------------------------------------------------------------------
            // check args
            //------------------------------------------------------------------
            if(2!=args.size)
                throw exception("%s%sinvalid #children",**title,fn);
            
            const string &which = args.head->name();
            if(which!="#include")
                throw exception("%s%s: unexpected command '%s'",**title,fn,*which);

            
            //------------------------------------------------------------------
            // extract info: kind and include name
            //------------------------------------------------------------------
            const XNode      &node = *args.tail;
            const includePath flag = includePathFrom( node.name(), **title, fn);
            string            incl; if(!node.query(incl)) throw exception("%s%sunexpected no include name",**title,fn);
            
            Y_JAXIOM(std::cerr << "[" << title << "] include=''" << incl << "''" << std::endl);

            //------------------------------------------------------------------
            // and compile again!
            //------------------------------------------------------------------
            switch(flag)
            {
                case includeRelative:
                {
                    const string subName = vfs::get_file_dir(fileName) + incl;
                    Y_JAXIOM(std::cerr << "[" << title << "]    load=''" << subName << "''" << std::endl);
                    return compileFile(subName);
                }
                case includeAbsolute:
                    break;
            }
            
            throw exception("%s%snot implemented!",**title,fn);
            
        }
        
        
    }
    
}

