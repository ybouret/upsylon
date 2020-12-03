
#include "y/jive/dialect/loader.hpp"
#include "y/fs/vfs.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {

            static const char *AllowedStrings[] =
            {
                Loader::jStringID,
                Loader::bStringID
            };


            Loader:: Loader() :
            Dialect::Parser(),
            shash( Y_MPH_WORDS(AllowedStrings) )
            {
            }

            Loader:: ~Loader() throw()
            {

            }

            XNode * Loader:: loadBinary(Module *module)
            {
                Source         source(module);
                size_t         temp=0;
                XNode::Pointer xnode( XNode::Load(source,*this,temp) );
                return xnode.yield();
            }


            void Loader:: checkIncludes( XNode::Pointer &dialect )
            {

                static const char fn[] = "Dialect::Loader::checkIncludes: ";

                //--------------------------------------------------------------
                // initialize loop over top-level child(ren)
                //--------------------------------------------------------------
                if( !dialect->isInternal() ) throw exception("%sdialect not is not internal!",fn);
                XList  &children = dialect->leaves();
                XNode  *child    = children.head;
                while(child)
                {
                    XNode *next = child->next;
                    if( Include == *(child->axiom.name) )
                    {
                        //------------------------------------------------------
                        // check Include kind
                        //------------------------------------------------------
                        if( ! child->isInternal() ) throw exception("%s'%s' directive is not internal!",fn,Include);

                        //------------------------------------------------------
                        // check Include args
                        //------------------------------------------------------
                        const XNode::List &args = child->leaves();
                        if(1!=args.size) throw exception("%sinvalid args for '%s''",fn,Include);

                        //------------------------------------------------------
                        // check Include arg
                        //------------------------------------------------------
                        const XNode       &data  = *args.head;
                        if( !data.isTerminal() )    throw exception("%s'%s' argument is not a terminal",fn,Include);
                        const string      &name     = data.name();
                        bool               absolute = false;
                        switch( shash(name) )
                        {
                            case 0: assert( jStringID == name );
                                absolute = false;
                                break;

                            case 1: assert( bStringID == name );
                                absolute = true;
                                break;

                            default:
                                throw exception("%sunhandled %s argument '%s' ",fn,Include,*name );
                        }

                        //------------------------------------------------------
                        // create include name
                        //------------------------------------------------------
                        // TODO: absolute/relative
                        const Lexeme  *lexeme   = data.lexeme();
                        const string   fileName = lexeme->toString();
                        const string   rootName = vfs::get_file_dir(*(lexeme->tag));
                        std::cerr << "loading " << (absolute?"absolute":"relative") << " '" << fileName << "'" << std::endl;
                        std::cerr << "from " << rootName << std::endl;
                        const string   inclName = rootName + fileName;

                        //------------------------------------------------------
                        // create included node
                        //------------------------------------------------------
                        XNode *included = loadSketch(inclName);

                        //------------------------------------------------------
                        // no throw exchange/release
                        //------------------------------------------------------
                        XNode::Release( children.replace(child,included) );
                    }

                    child = next;
                }

            }
        }

    }

}
