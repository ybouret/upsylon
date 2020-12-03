
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


            void Loader:: checkIncludes( XNode::Pointer &dialect )
            {

                static const char fn[] = "Dialect::Loader::checkIncludes: ";

                //--------------------------------------------------------------
                // initialize loop over top-level child(ren)
                //--------------------------------------------------------------
                if(XNode::IsInternal!=dialect->kind) throw exception("%sdialect not is not internal!",fn);
                XNode::List &children = dialect->leaves();
                XNode       *child    = children.head;
                while(child)
                {
                    XNode *next = child->next;
                    if( Include == *(child->axiom.name) )
                    {
                        //------------------------------------------------------
                        // check Include kind
                        //------------------------------------------------------
                        if(XNode::IsInternal!=child->kind) throw exception("%s'%s' directive is not internal!",fn,Include);

                        //------------------------------------------------------
                        // check Include args
                        //------------------------------------------------------
                        const XNode::List &args = child->leaves();
                        if(1!=args.size) throw exception("%sinvalid args for '%s''",fn,Include);

                        //------------------------------------------------------
                        // check Include arg
                        //------------------------------------------------------
                        const XNode       &data  = *args.head;
                        if(XNode::IsTerminal!=data.kind)    throw exception("%s'%s' argument is not a terminal",fn,Include);
                        const string      &name     = *data.axiom.name;
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

                        const Lexeme  *lexeme   = data.lexeme();
                        const Context &context  = *lexeme;
                        const string   fileName = lexeme->toString();
                        std::cerr << "loading " << (absolute?"absolute":"relative") << " '" << fileName << "'" << std::endl;
                        const string   rootName = vfs::get_file_dir(*context.tag);
                        std::cerr << "from " << rootName << std::endl;
                        const string   inclName = rootName + fileName;
                        // TODO: absolute/relative
                        XNode *included = loadFile(inclName);
                        // no throw exchange
                        XNode::Release( children.replace(child,included) );

                    }
                    child = next;
                }

            }
        }

    }

}
