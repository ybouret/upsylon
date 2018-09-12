
#include "y/lang/lexical/translator.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            Translator:: Translator(const string &id) :
            name( new string(id) ),
            curr(0),
            root(0),
            cache(),
            history(),
            scanners(4,as_capacity)
            {
                setup();
            }

            Translator:: ~Translator() throw()
            {
            }

            void Translator::setup()
            {
                root = new Scanner(name);
                const Scanner::Pointer p(root);
                if(!scanners.insert(p))
                {
                    throw exception("unable to insert base scanner [%s]", **(p->label));
                }
            }

            Scanner & Translator:: decl(const string &id)
            {
                assert(root);
                Scanner::Pointer p = new Scanner(id);
                if(!scanners.insert(p))
                {
                    throw exception("[%s] multiple scanner [%s]", **name, *id);
                }
                return *p;
            }


            void Translator:: reset() throw()
            {
                curr = root;
                history.clear();
                cache.clear();
            }

            void   Translator:: unget( Lexeme *lx ) throw()
            {
                assert(lx);
                cache.push_front(lx);
            }

            void   Translator:: unget( Lexeme::List &lxm ) throw()
            {
                while(lxm.size)
                {
                    cache.push_front(lxm.pop_back());
                }
            }


        }
    }
}

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

            Lexeme * Translator:: get( Source &source )
            {
                assert(curr!=NULL);
                //______________________________________________________________
                //
                //
                // get the next lexeme
                //
                //______________________________________________________________
                if(cache.size)
                {
                    //__________________________________________________________
                    //
                    // cached!
                    //__________________________________________________________
                    return cache.pop_front();
                }
                else
                {
                    //__________________________________________________________
                    //
                    // need to get a new one
                    //__________________________________________________________
                    while(true)
                    {
                        Message msg = 0;
                        Lexeme *lx  = curr->probe(source,msg);
                        if(lx)
                        {
                            //--------------------------------------------------
                            // ok, regular one (some are possibly discarded...)
                            //--------------------------------------------------
                            return lx;
                        }
                        else if(msg!=0)
                        {
                            //--------------------------------------------------
                            // control lexeme, action was performed
                            //--------------------------------------------------
                            assert(NULL==lx);
                            switch(msg->type)
                            {
                                case ControlEvent::Call: 
                                case ControlEvent::Jump:
                                    break;
                                case ControlEvent::Back: break;

                            }
                        }
                        else
                        {
                            //--------------------------------------------------
                            // no msg, no lexeme => EOF
                            //--------------------------------------------------
                            assert( !source.active() );
                            return NULL;
                        }
                    }
                }
            }

        }

    }

}

