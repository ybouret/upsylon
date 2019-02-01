//! \file
#ifndef Y_LANG_LEXICAL_TRANSLATOR_INCLUDED
#define Y_LANG_LEXICAL_TRANSLATOR_INCLUDED 1

#include "y/lang/lexical/plugin.hpp"
#include "y/associative/set.hpp"
#include "y/core/addr-list.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            //! transform a source of Char into a source of Lexeme
            class Translator : public Object
            {
            public:
                typedef set<string,Scanner::Pointer> DataBase; //!< database of scanners
                typedef set<string,Plugin::Pointer>  Plugins;  //!< database of plugins
                const Tag  label;                              //!< shared label
                
                //! destructor
                virtual ~Translator() throw();

                //! initialize
                explicit Translator(const string &id);

                //! return root scanner
                Scanner & operator *() throw();

                //! create, register and return a new scanner
                Scanner & decl(const string &id);

                //! create, register and return a new scanner
                inline
                Scanner & decl(const char *id) { const string _(id); return decl(_); }

                //! reset curr to base and clean history
                void    reset() throw();

                //! unget an extracted lexeme
                void    unget( Lexeme *lx ) throw();

                //! unget a chain of extracted lexemes
                void    unget( Lexeme::List &lxm ) throw();

                //! get the next lexeme
                Lexeme *get( Source &source );

                //! check if there is a next lexeme
                bool          active(Source &source);

                //! check if there is a next lexeme
                const Lexeme *peek(Source &source);
                
                //! get the last matched lexeme
                const Lexeme *last() const throw() { return cache.tail; }


                //! no args PLUGIN constructor
                template <typename PLUGIN>
                inline void hook( Scanner &scanner, const string &pluginName )
                {
                    Plugin::Pointer *p = plugins.search( pluginName );
                    if(p)
                        link(scanner,**p);
                    else
                        link(scanner,enroll_plugin( new PLUGIN(*this,pluginName) ) );
                }

                //! no args PLUGIN constructor
                template <typename PLUGIN>
                inline void hook( Scanner &scanner, const char *pluginName )
                {
                    const string _(pluginName); hook<PLUGIN>(scanner,_);
                }

                //! PLUGIN with one argument
                template <typename PLUGIN>
                inline void hook( Scanner &scanner, const string &pluginName, const char *expr)
                {
                    Plugin::Pointer *p = plugins.search( pluginName );
                    if(p)
                        link(scanner,**p);
                    else
                        link(scanner,enroll_plugin( new PLUGIN(*this,pluginName,expr) ) );
                }

                //! PLUGIN with one argument
                template <typename PLUGIN>
                inline void hook( Scanner &scanner, const char *pluginName, const char *expr)
                {
                    const string _(pluginName); hook<PLUGIN>(scanner,_,expr);
                }


                //! PLUGIN with two arguments
                template <typename PLUGIN>
                inline void hook( Scanner &scanner, const string &pluginName, const char *rxInit, const char *rxQuit)
                {
                    Plugin::Pointer *p = plugins.search( pluginName );
                    if(p)
                        link(scanner,**p);
                    else
                        link(scanner,enroll_plugin( new PLUGIN(*this,pluginName,rxInit,rxQuit) ) );
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Translator);
                typedef core::addr_node<Scanner> sNode;
                typedef core::addr_list<Scanner> History;

                Scanner     *curr;     //!< current scanner
                Scanner     *base;     //!< root scanner
                Lexeme::List cache;    //!< cache of lexemes
                History      history;  //!< for call/back
                DataBase     scanners; //!< database of scanners
                Plugins      plugins;  //!< database of plugins

                void    enroll( Scanner *s );         //!< insert into scanners
                Plugin &enroll_plugin( Plugin *plg ); //!< insert into plugins AND scanners
                void    link(Scanner &, Plugin & );   //!< scanner calls plugin upon trigger



            public:
                Dictionary dict; //!< shared dictionary, set as userDict for registers scanners
            };
        }

        typedef Lexical::Translator Lexer; //!< alias
    }
}

#endif

