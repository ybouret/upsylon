
#include "y/lang/lexical/translator.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {

#define Y_LANG_TRANS()  \
name( new string(id) ), \
curr(0),                \
base(0),                \
cache(),                \
history(),              \
scanners(4,as_capacity),\
dict()

            Translator:: Translator(const string &id) :
            Y_LANG_TRANS()
            {
                setup();
            }

            Translator:: ~Translator() throw()
            {
            }

            void Translator::setup()
            {
                base = & decl(*name);
                curr = base;
            }

            void Translator :: enroll( Scanner *s )
            {
                assert(s);
                Scanner::Pointer p = s;
                if(!scanners.insert(p))
                {
                    throw exception("[%s] multiple scanner [%s]", **name, **(p->label) );
                }
                p->userDict = &dict;
            }

            Plugin & Translator:: enroll_plugin( Plugin *plg )
            {
                assert(plg);
                Plugin::Pointer p = plg;
                if(!plugins.insert(p))
                {
                    throw exception("[%s] multiple plugin [%s]", **name, **(p->label) );
                }
                enroll(plg);
                return *plg;
            }


            void Translator:: link(Scanner &scanner,
                                   Plugin  &plugin)
            {
                scanner.call(*(plugin.label),plugin.trigger, &plugin, & Plugin::Init );
            }

            Scanner & Translator:: decl(const string &id)
            {
                Scanner *s = new Scanner(id);
                enroll(s);
                return *s;
            }


            void Translator:: reset() throw()
            {
                curr = base;
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

            Scanner & Translator:: operator *() throw()
            {
                assert(base);
                return *base;
            }

            bool Translator:: active( Source &source )
            {
                if(cache.size>0)
                {
                    return true;
                }
                else
                {
                    Lexeme *lx = get(source);
                    if(!lx)
                    {
                        return false;
                    }
                    else
                    {
                        cache.push_front(lx);
                        return true;
                    }
                }
            }

            const Lexeme * Translator:: peek(Source &source)
            {
                if(active(source))
                {
                    assert(cache.size>0);
                    return cache.head;
                }
                else
                {
                    return NULL;
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
            RESCAN:
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
                        assert(curr!=NULL);
                        //std::cerr << "with [" << curr->label << "]" << std::endl;
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
                                case ControlEvent::Call: history.append(curr); // FALLTHRU

                                case ControlEvent::Jump: {
                                    Scanner::Pointer *ppScanner = scanners.search(msg->label);
                                    if(!ppScanner) throw exception("[%s] jump/call to undeclared [%s]", **(curr->label), *(msg->label) );
                                    curr = & (**ppScanner);
                                } break;

                                case ControlEvent::Back:
                                    if(history.size<=0)
                                    {
                                        throw exception("[%s] can't go back from [%s]", **name, **(curr->label) );
                                    }
                                    curr = history.tail->addr;
                                    delete history.pop_back();
                                    break;
                            }
                            //--------------------------------------------------
                            // and continue, rescan since a new lexeme
                            // may be in the cache
                            //--------------------------------------------------
                            goto RESCAN;
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

