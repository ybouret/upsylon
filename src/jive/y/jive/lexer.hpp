//! \file

#ifndef Y_JIVE_LEXER_INCLUDED
#define Y_JIVE_LEXER_INCLUDED 1

#include "y/jive/lexical/plugin.hpp"
#include "y/jive/lexemes.hpp"
#include "y/jive/pattern/dictionary.hpp"
#include "y/sequence/list.hpp"

namespace upsylon {

    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //! Lexer
        /**
         transform a stream of Char(s) into a stream of Lexeme(s)
         using different lexical scanners/plugins
         */
        //______________________________________________________________________
        class Lexer : public Lexical::Queue, public Lexical::Scanner
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef Lexical::Scanner                 Scanner;      //!< alias
            typedef suffix_storage<Scanner::Pointer> ScannerTree;  //!< alias
            typedef Lexical::Plugin                  Plugin;       //!< alias
            typedef suffix_storage<Plugin::Pointer>  PluginTree;   //!< alias

            typedef Scanner                      *HScan;        //!< alias for history
            typedef list<HScan>                   History;      //!< history type

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~Lexer() throw();

            //! setup
            template <typename ID> inline
            explicit Lexer(const ID &id) :
            Lexical::Queue(), Scanner(id,AcceptEOS),
            scan(this), hist(), io(), db(), ex(),
            dictionary()
            {
                initialize();
            }

            //__________________________________________________________________
            //
            // methods for scanners
            //__________________________________________________________________

            //! declare a new scanner
            template <typename ID> inline
            Scanner &decl(const ID &id)
            {
                const Tag l = Tags::Make(id);
                return newScanner(l);
            }

            //__________________________________________________________________
            //
            // methods for plugins
            //__________________________________________________________________

            //! create plugin with no args
            template <typename PLUGIN,typename ID>
            PLUGIN &plug(const ID &id)
            {
                PLUGIN * p = new PLUGIN(id,*this);
                newPlugin( p );
                return *p;
            }

            //! create plugin with one arg
            template <typename PLUGIN,typename ID,typename RX>
            PLUGIN &plug(const ID &id, const RX &rx)
            {
                PLUGIN * p = new PLUGIN(id,rx,*this);
                newPlugin( p );
                return *p;
            }

            //! create plugin with two arg
            template <typename PLUGIN,typename ID,typename ENTER,typename LEAVE>
            PLUGIN &plug(const ID &id, const ENTER &enter, const LEAVE &leave)
            {
                PLUGIN * p = new PLUGIN(id,enter,leave,*this);
                newPlugin( p );
                return *p;
            }

            //! check is a plugin is registerer
            template <typename ID>
            bool hasPlugin(const ID &id) const throw()
            {
                return NULL != ex.search(id);
            }

            //! get a plugin by ID
            template <typename ID>
            Plugin &getPlugin(const ID &id)
            {
                const Plugin::Pointer *ppPlugin = ex.search(id);
                if(!ppPlugin) noPlugin(id);
                return (Plugin&)(**ppPlugin);
            }

          
            //__________________________________________________________________
            //
            // I/O methods
            //__________________________________________________________________


            Lexeme       *get(Source &);             //!< get new lexemes
            void          unget(Lexeme  *) throw();  //!< unget lexeme
            void          unget(Lexemes &) throw();  //!< unget lexemes
            void          reset()          throw();  //!< cleanup
            void          ready(Source &,size_t);    //!< try to grow IO cache
            const Lexeme *next(Source &);            //!< get/unget

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            Scanner    *scan;
            History     hist;
            Lexemes     io;    // I/O
            ScannerTree db;    // scanners
            PluginTree  ex;    // extensions

            void         initialize();
            Scanner &    newScanner(const Tag &t);
            void         newPlugin( Plugin *plg );
            void         jmp(const Tag &);
            virtual void push(Lexeme *) throw();
            void         noPlugin(const string &s) const;
            void         noPlugin(const char   *s) const;
        public:
            Dictionary   dictionary; //!< shared dictionary for all scanners
        };
    }

}

#endif
