//! \file

#ifndef Y_LANG_DYNAMO_GENERATOR_INCLUDED
#define Y_LANG_DYNAMO_GENERATOR_INCLUDED 1

#include "y/lang/syntax/parser.hpp"
#include "y/associative/set.hpp"
#include "y/hashing/mph.hpp"
#include "y/sequence/pipe.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{

    namespace Lang
    {


        //! Generate a parser from a Dynamo compiled AST
        class DynamoGenerator
        {
        public:
            typedef Syntax::Node       Node; //!< alias
            typedef Syntax::Aggregate  Agg;  //!< alias

            //! initialize
            explicit DynamoGenerator();

            //! destructor
            virtual ~DynamoGenerator() throw();

            //! from top-level
            Syntax::Parser *create( Node &dynamo, const bool flag=false );




        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);

            //! symbols to collect
            class Symbol : public CountedObject
            {
            public:
                typedef intr_ptr<string,Symbol> Pointer;
                const string name;
                const Origin from;
                inline explicit Symbol(const string &n, const Origin &m ) : name(n), from(m) {}
                inline virtual ~Symbol() throw() {}
                inline const string & key() const throw() { return name; }
                inline friend std::ostream & operator<<( std::ostream &os, const Symbol &sym )
                {
                    os << sym.from << '_' << sym.name;
                    return os;
                }
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Symbol);
            };

            
            typedef key_hasher<string,hashing::fnv>              KeyHasher;
            typedef memory::pooled                               Memory;
            typedef set<string,Symbol::Pointer,KeyHasher,Memory> Symbols;



            auto_ptr<Syntax::Parser>  parser;    //!< currently built parser
            bool                      verbose;   //!< verbose flag
            int                       level;     //!< for tree walking
            hashing::mperf            htop;      //!< RULE, ALIAS, LXR, PLUGIN
            hashing::mperf            hsyn;      //!< AGG, ALT, OOM, ZOM, OPT
            hashing::mperf            hstr;      //!< RX, RS, ^
            hashing::mperf            hlxr;      //!< drop,endl,comment
            int                       icom;      //!< index for comment
            lstack<Origin>            modules;   //!< stack of visited modules
            Symbols                   terminals; //!< database of terminals
            Symbols                   internals; //!< database of internals

            static const int isAGG = 0;
            static const int isALT = 1;
            static const int isZOM = 2;
            static const int isOOM = 3;
            static const int isOPT = 4;


            //! extract name from dynamo.children.head->lexeme
            string getModuleName( const Node &dynamo ) const;

            //! collect top level rules, compile alias, lxr and plugins
            void collectTopLevel( Node *node );

            //! extract node name, must match label and skip chars from string
            string getNodeName( const Node &node, const char *label, const size_t nskip ) const;

            //! extract the kind of rule
            int   getRuleKind( const Node &node ) const;

            //! create and register a new symbol
            void  newSymbol(Symbols &target, const string &name);

            //! new internal
            inline void newInternal(const string &name) { newSymbol(internals,name); }

            //! new terminal
            inline void newTerminal(const string &name) { newSymbol(terminals,name); }

            //! create a rule
            void onRule( const Node &node );

            //! create an alias
            void onAlias( const Node &node );

            //! create a lexical rule
            void onLxr( const Node &node );

            //! parse RX, RS, OP
            string nodeToRegExp(const Node &node, int &h) const;

            //! without trace
            inline string getNodeRegExp(const Node &node) const
            {
                int h=-1;
                return nodeToRegExp(node,h);
            }

            //! convert a string node to a terminal/marker/operator
            void createSpecificTerminal(const string &label, const Node &node) ;

            //! create a plugin based on name
            void onPlugin( const Node &node );

            //! indent for verbose
            std::ostream & indent() const;

            //! build a comment label from icom and parser name
            string getCommentLabel();
        };


    }

}

#endif

