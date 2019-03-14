//! \file
#ifndef Y_DYNAMO_GENERATOR_INCLUDED
#define Y_DYNAMO_GENERATOR_INCLUDED 1

#include "y/lang/dynamo/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {
        //______________________________________________________________________
        //
        //
        // class management
        //
        //______________________________________________________________________
        typedef memory::pooled                  DynamoMemory; //!< internal memory type
        typedef key_hasher<string,hashing::fnv> DynamoHasher; //!< string hasher
        template <typename T>
        struct DynamoSetOf
        {
            typedef set<string,T,DynamoHasher,DynamoMemory> Type;
        };
        //______________________________________________________________________
        //
        //
        // user's plugin management
        //
        //______________________________________________________________________
        typedef functor<Syntax::Terminal &,TL2(const string &, Syntax::Parser &)> DynamoPlugin;         //!< functor to hook a plugin
        typedef map<string,DynamoPlugin,DynamoHasher,DynamoMemory>                DynamoPluginFactory;  //!< database of plugin builder

        //______________________________________________________________________
        //
        //
        // user's lexer managamement
        //
        //______________________________________________________________________
        typedef array<const string>                                       DynamoArgs;           //!< array of arguments for lexical instructions
        typedef functor<void,TL2(Lexer &, const DynamoArgs &)>            DynamoLexical;        //!< lexer only modifier
        typedef map<string,DynamoLexical,DynamoHasher,DynamoMemory>       DynamoLexicalFactory; //!< database of lexer modifier

        //______________________________________________________________________
        //
        //
        // Generator class and auxiliary classes
        //
        //______________________________________________________________________

        //! base class for symbols
        class DynamoInfo
        {
        public:
            typedef DynamoSetOf<DynamoInfo>::Type Set; //!< database of symbols

            DynamoInfo(const DynamoInfo &) throw();    //!< no throw copy
            virtual ~DynamoInfo() throw();             //!< destructor
            const string &key() const throw();         //!< rule.name

            const Tag           from;                   //!< creator module
            const Syntax::Rule &rule;                   //!< the generic underlying rule

        protected:
            //! setup
            explicit DynamoInfo( const Tag &, const Syntax::Rule & ) throw();

        private:
            Y_DISABLE_ASSIGN(DynamoInfo);
        };

        //! generic derived symbol store for toplevel rules
        template <typename T>
        class DynamoRef : public DynamoInfo
        {
        public:
            typedef typename DynamoSetOf<DynamoRef>::Type Set; //!< specialized database

            T &derived; //!< the derived type reference

            //! setup
            inline explicit DynamoRef(const Tag &moduleID, T &d ) throw() : DynamoInfo(moduleID,d), derived(d) {}

            //! desctructor
            inline virtual ~DynamoRef() throw() {}

            //! no-throw copy
            inline DynamoRef(const DynamoRef &other) throw() : DynamoInfo(other), derived(other.derived) {}

        private:
            Y_DISABLE_ASSIGN(DynamoRef);
        };


        typedef DynamoRef<Syntax::Terminal> DynamoTerm; //!< alias for terminal
        typedef DynamoRef<Syntax::Compound> DynamoRule; //!< alias for compound



        //! generates a parser from a compiled tree
        class DynamoGenerator
        {
        public:
            typedef vector<const Tag,DynamoMemory> Modules; //!< alias

            virtual ~DynamoGenerator() throw(); //!< destructor
            explicit DynamoGenerator();         //!< setup and register plugins

            //! build the parser from a top-level dynamo node
            Syntax::Parser * build( DynamoNode &top );

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

            DynamoLexical & findLexical( const string &id );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);


            auto_ptr<Syntax::Parser> parser;    //!< currently built parser
            const hashing::mperf     declH;     //!< "dynamo",...
            Modules                  modules;   //!< stack of modules
            DynamoInfo::Set          symbols;   //!< all the symbols
            DynamoTerm::Set          terminals; //!< all the terminals
            DynamoRule::Set          internals; //!< all the internals
            DynamoPluginFactory      plugins;   //!< declared plugins
            DynamoLexicalFactory     lexicals;  //!< declared lexical
            int                      level;     //!< depth



            void declModule(  DynamoNode       &dynamo );
            void declAlias(   const DynamoNode &alias  );
            void declPlugin(  const DynamoNode &plg    );
            void declLexical( const DynamoNode &lxr    );
            void declEOL(     const DynamoNode &eol    );

            string getContent( const DynamoNode *node, const char *id, const char *context) const;
            string getRID( const DynamoNode *node, const char *context ) const;
            string getEID( const DynamoNode *node, const char *context ) const;
            string getLID( const DynamoNode *node, const char *context ) const;
            string getSTR( const DynamoNode *node, const char *context ) const;

            void storeDecl( Syntax::Terminal &t );

            Syntax::Terminal & _jstring( const string &id, Syntax::Parser &p );
            Syntax::Terminal & _rstring( const string &id, Syntax::Parser &p );

            void lexicalDrop( Lexer &, const DynamoArgs & );
            void lexicalEndl( Lexer &, const DynamoArgs & );
            void lexicalComm( Lexer &, const DynamoArgs & );

        public:
            bool verbose; //!< verbosity flag

        };
    }

}

#endif

