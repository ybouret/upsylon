
#include "y/lang/lexical/lexer.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Lang {

        namespace Lexical {


#define Y_LANG_TRANS()   \
label( new string(id) ), \
curr(0),                 \
base(0),                 \
treated(),               \
history(),               \
scanners(4,as_capacity), \
plugins(4,as_capacity),  \
dict()

            void Translator:: attach( Scanner *scanner ) throw()
            {
                assert(scanner);
                scanner->dict = &dict;
            }

            void Translator:: setup()
            {
                base = new Scanner(label);
                {
                    const Scanner::Pointer p(base);
                    if(!scanners.insert(p)) throw exception("[%s] cannot initialize scanners!!!", **label );
                }
                curr = base;
                attach(base);
            }

            Translator:: Translator(const string &id) :
            Y_LANG_TRANS()
            {
                setup();
            }

            Translator:: Translator(const char *id) :
            Y_LANG_TRANS()
            {
                setup();
            }


            Translator:: ~Translator() throw()
            {
            }

            Scanner *Translator :: enroll( Scanner *s )
            {
                assert(s);
                Scanner::Pointer p = s;
                if(!scanners.insert(p))
                {
                    throw exception("[%s] multiple scanner [%s]", **label, **(p->label) );
                }
                attach( s );
                return s;
            }

            Plugin & Translator:: enroll_plugin( Plugin *plg )
            {
                assert(plg);
                Plugin::Pointer p = plg;
                if(!plugins.insert(p))
                {
                    throw exception("[%s] multiple plugin [%s]", **label, **(p->label) );
                }
                try
                {
                    (void) enroll(plg);
                }
                catch(...)
                {
                    plugins.no(*(p->label));
                    throw;
                }
                return *plg;
            }


            void Translator:: link(Scanner &scanner,
                                   Plugin  &plugin)
            {
                assert( owns(scanner) );
                assert( owns(plugin)  );
                
                if(scanner.verbose) { std::cerr << "@plug[" << scanner.label << "-->" << plugin.label << "] on '" << plugin.trigger << "'" << std::endl; }
                scanner.call(*(plugin.label),plugin.trigger, &plugin, & Plugin::Init );
            }

            Scanner & Translator:: decl(const string &id)
            {
                Scanner *s = enroll( new Scanner(id) );
                s->verbose = (**this).verbose;
                return *s;
            }

            Scanner & Translator:: decl(const char *id)
            { const string _(id); return decl(_); }


            bool Translator:: owns( const Scanner &s ) const throw()
            {
                const Scanner::Pointer *pp = scanners.search(s.key());
                return pp && (&s == & **pp );
            }

            void Translator:: echo(const bool flag) throw()
            {
                for( DataBase::iterator it = scanners.begin(); it != scanners.end(); ++it )
                {
                    (**it).echo = flag;
                }
            }


            void Translator:: reset() throw()
            {
                curr = base;
                history.release();
                treated.release();
            }

            void   Translator:: unget( Lexeme *lx ) throw()
            {
                assert(lx);
                treated.push_front(lx);
            }

            void   Translator:: unget( Lexeme::List &lxm ) throw()
            {
                while(lxm.size)
                {
                    treated.push_front(lxm.pop_back());
                }
            }

            Scanner & Translator:: operator *() throw()
            {
                assert(base);
                return *base;
            }

            bool Translator:: active( Source &source )
            {
                if(treated.has_nodes())
                {
                    return true;
                }
                else
                {
                    assert( treated.is_vacant() );
                    Lexeme *lx = get(source);
                    if(!lx)
                    {
                        return false;
                    }
                    else
                    {
                        treated.push_front(lx);
                        return true;
                    }
                }
            }

            const Lexeme * Translator:: peek(Source &source)
            {
                if(active(source))
                {
                    assert( treated.has_nodes() );
                    return treated.head;
                }
                else
                {
                    assert( treated.is_vacant() );
                    return NULL;
                }
            }

            const Lexeme * Translator:: last() const throw() { return treated.tail; }


        }
    }
}

namespace upsylon {

    namespace Lang {

        namespace Lexical {

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
                if(treated.has_nodes())
                {
                    //__________________________________________________________
                    //
                    // cached!
                    //__________________________________________________________
                    return treated.pop_front();
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
                        Directive result = 0;
                        Lexeme   *lexeme = curr->probe(source,result);
                        if(NULL!=lexeme)
                        {
                            //--------------------------------------------------
                            // ok, regular one (some are possibly discarded...)
                            //--------------------------------------------------
                            return lexeme;
                        }
                        else if(0!=result)
                        {
                            //--------------------------------------------------
                            // control lexeme, action was performed
                            //--------------------------------------------------
                            assert(NULL==lexeme);
                            switch(result->type)
                            {
                                case ControlEvent::Call: history.append(curr); // FALLTHRU
                                case ControlEvent::Jump: {
                                    const string      resultID  = result->label;
                                    Scanner::Pointer *ppScanner = scanners.search(resultID);
                                    if(!ppScanner) throw exception("[%s] jump/call to undeclared [%s]", **(curr->label), *resultID );
                                    curr = & (**ppScanner);
                                } break;

                                case ControlEvent::Back:
                                    if(history.size<=0)
                                    {
                                        throw exception("[%s] can't go back from [%s]", **label, **(curr->label) );
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

