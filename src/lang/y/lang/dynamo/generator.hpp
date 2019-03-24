//! \file
#ifndef Y_DYNAMO_GENERATOR_INCLUDED
#define Y_DYNAMO_GENERATOR_INCLUDED 1

#include "y/lang/dynamo/compiler.hpp"
#include "y/lang/dynamo/symbols.hpp"

namespace upsylon
{
    namespace Lang
    {
       
        //______________________________________________________________________
        //
        //
        // user's plugin management
        //
        //______________________________________________________________________
        typedef functor<Syntax::Terminal &,TL2(const string &, Syntax::Parser &)> DynamoPlugin;         //!< functor to hook a plugin
        typedef DynamoMapOf<DynamoPlugin>::Type                                   DynamoPluginFactory;  //!< database of plugin builder

        //______________________________________________________________________
        //
        //
        // user's lexer managamement
        //
        //______________________________________________________________________
        typedef array<const string>                                       DynamoArgs;           //!< array of arguments for lexical instructions
        typedef functor<void,TL2(Lexer &, const DynamoArgs &)>            DynamoLexical;        //!< lexer only modifier
        typedef DynamoMapOf<DynamoLexical>::Type                          DynamoLexicalFactory; //!< database of lexer modifier


        //______________________________________________________________________
        //
        //
        //! generates a parser from a compiled tree
        //
        //______________________________________________________________________
        class DynamoGenerator : public DynamoObject
        {
        public:
            typedef vector<const Tag,DynamoMemory> Modules; //!< alias

            virtual ~DynamoGenerator() throw(); //!< destructor
            explicit DynamoGenerator();         //!< setup and register plugins

            //! build the parser from a top-level dynamo node
            Syntax::Parser * build(DynamoNode     &top,
                                   DynamoSymbols *symbols);

            //! clear maximum of data
            void clear() throw();

            //! register a plugin
            void registerPlugin(const string &id, const DynamoPlugin &dp);

            //! wrapper to register a host+method plugin
            template <typename HOST_POINTER,
            typename METHOD_POINTER>
            inline void registerPlugin(const string  &id,
                                       HOST_POINTER   host,
                                       METHOD_POINTER meth)
            {
                DynamoPlugin dp(host,meth);
                registerPlugin(id,dp);
            }

            //! wrapper to register a host+method plugin
            template <typename HOST_POINTER,
            typename METHOD_POINTER>
            inline void registerPlugin(const char    *id,
                                       HOST_POINTER   host,
                                       METHOD_POINTER meth)
            {
                const string _(id); registerPlugin(_,host,meth);
            }

            //! find a registered plugin
            DynamoPlugin &findPlugin( const string &id );

            //! register a lexical management function
            void registerLexical( const string &id, const DynamoLexical &dl );

            //! wrapper to register a host+method lexical
            template <typename HOST_POINTER,
            typename METHOD_POINTER>
            inline void registerLexical(const string  &id,
                                       HOST_POINTER   host,
                                       METHOD_POINTER meth)
            {
                DynamoLexical dp(host,meth);
                registerLexical(id,dp);
            }

            //! wrapper to register a host+method lexical
            template <typename HOST_POINTER,
            typename METHOD_POINTER>
            inline void registerLexical(const char    *id,
                                        HOST_POINTER   host,
                                        METHOD_POINTER meth)
            {
                const string _(id);
                return registerLexical(_,host,meth);
            }

            //! find a registered lexical management function
            DynamoLexical & findLexical( const string &id );

            //! find a registered symbol by its name
            const Syntax::Rule  & findSymbol( const string &id );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);


            auto_ptr<Syntax::Parser> parser;    //!< currently built parser
            const hashing::mperf     declH;     //!< "dynamo","aka","plg","lxr","eol","cmd","rule"
            const hashing::mperf     implH;     //!< "dynamo","rule"
            const hashing::mperf     operH;     //!< "op", "rx", "rs"
            const hashing::mperf     fillH;     //!< "rid",...
            Modules                  modules;   //!< stack of modules
            DynamoInfo::Set          symbols;   //!< all the reachable symbols
            DynamoTerm::Set          terminals; //!< all the reachable terminals
            DynamoRule::Set          internals; //!< all the reachable internals
            DynamoTerm::Set          literals;  //!< all the literals 
            DynamoPluginFactory      plugins;   //!< declared plugins
            DynamoLexicalFactory     lexicals;  //!< declared lexical
            int                      level;     //!< depth



            void declModule(        DynamoNode &dynamo ); //!< top level call
            void declAlias(   const DynamoNode &alias  );
            void declPlugin(  const DynamoNode &plg    );
            void declLexical( const DynamoNode &lxr    );
            void declEOL(     const DynamoNode &eol    );
            void declCommand( const DynamoNode &cmd    );
            void declRule(          DynamoNode &cmd    );

            string getContent( const DynamoNode *node, const char *id, const char *context) const;
            string getRID( const DynamoNode *node, const char *context ) const;
            string getEID( const DynamoNode *node, const char *context ) const;
            string getCID( const DynamoNode *node, const char *context ) const;
            string getLID( const DynamoNode *node, const char *context ) const;
            string getSTR( const DynamoNode *node, const char *context ) const;

            void storeDecl( Syntax::Terminal &t );
            void storeDecl( Syntax::Compound &c );
            void storeImpl( Syntax::Compound &c );

            Syntax::Terminal & _jstring( const string &id, Syntax::Parser &p );
            Syntax::Terminal & _rstring( const string &id, Syntax::Parser &p );

            void lexicalDrop( Lexer &, const DynamoArgs & );
            void lexicalEndl( Lexer &, const DynamoArgs & );
            void lexicalComm( Lexer &, const DynamoArgs & );

            void implModule( DynamoNode &dynamo ); //!< top level call to implement declared rules
            void implRule(   DynamoNode &r      ); //!< top level rule

            static string MakeSubName( const Syntax::Compound &parent, const unsigned indx);
            
            void fill(    Syntax::Compound &parent, DynamoNode *node );
            void fillRID( Syntax::Compound &parent, DynamoNode *node );
            void fillJK(  Syntax::Compound &parent, DynamoNode *node, const unsigned indx );
            void fillALT( Syntax::Compound &parent, DynamoNode *node, const unsigned indx );
            void fillGRP( Syntax::Compound &parent, DynamoNode *node, const unsigned indx );
            void fillOP(  Syntax::Compound &parent, DynamoNode *node );
            Syntax::Terminal &fillSTR( Syntax::Compound &parent, DynamoNode *node );

            void dispatchTerminals( const DynamoTerm::Set &terms, DynamoSymbols &symbols, const char *context ) const;

        public:
            bool verbose; //!< verbosity flag

        };
    }

}

#endif

