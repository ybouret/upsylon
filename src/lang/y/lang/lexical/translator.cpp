
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
scanners(4,as_capacity)

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
                base = new Scanner(name);
                const Scanner::Pointer p(base);
                if(!scanners.insert(p))
                {
                    throw exception("unable to insert base scanner [%s]", **(p->label));
                }
                curr = base;
            }

            Scanner & Translator:: decl(const string &id)
            {
                assert(base);
                Scanner::Pointer p = new Scanner(id);
                if(!scanners.insert(p))
                {
                    throw exception("[%s] multiple scanner [%s]", **name, *id);
                }
                return *p;
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
                        assert(curr!=NULL);
                        //std::cerr << "with [" << curr->label << "]" << std::endl;
                        Message msg = 0;
                        Lexeme *lx  = curr->probe(source,msg);
                        if(lx)
                        {
                            //--------------------------------------------------
                            // ok, regular one (some are possibly discarded...)
                            //--------------------------------------------------
                            //std::cerr << "|_" << lx->label <<  " : " << *lx << std::endl;
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
                                    //std::cerr << "|_[" << curr->label << "]->[" << msg->label << "]" << std::endl;
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
                            // and continue...
                            //--------------------------------------------------
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

