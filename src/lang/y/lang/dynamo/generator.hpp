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

        //! hold reference to some objects with a 'name' field
        template <typename T>
        class DynMeta
        {
        public:
            T &ref;                                                           //!< the reference
            inline  DynMeta( T &r ) throw() : ref(r) {}                       //!< initialize
            inline ~DynMeta() throw() {}                                      //!< destructor
            inline  DynMeta(const DynMeta &other) throw() : ref(other.ref) {} //!< copy
            inline const string &key() const throw() { return ref.name; }     //!< return the name field

        private:
            Y_DISABLE_ASSIGN(DynMeta);
        };



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

            typedef DynMeta<Syntax::Aggregate>                   MetaAgg;
            typedef key_hasher<string,hashing::fnv>              KeyHasher;
            typedef memory::pooled                               Memory;
            typedef set<string,MetaAgg,KeyHasher,Memory>         TopRules;
            typedef set<string,Symbol::Pointer,KeyHasher,Memory> Symbols;



            auto_ptr<Syntax::Parser>  parser;    //!< currently built parser
            bool                      verbose;   //!< verbose flag
            TopRules                  top;       //!< rules to be filled
            int                       level;     //!< for tree walking
            hashing::mperf            htop;      //!< RULE, ALIAS, LXR, PLUGIN
            hashing::mperf            hstr;      //!< RX, RS, ^
            hashing::mperf            hlxr;      //!< drop,endl,comment
            int                       icom;      //!< index for comment
            lstack<Origin>            modules;   //!< stack of visited modules
            Symbols                   terminals; //!< database of terminals
            Symbols                   internals; //!< database of internals
            
            //! extract name from dynamo.children.head->lexeeme
            string getModuleName( const Node &dynamo ) const;

            //! collect top level rules, compile alias, lxr and plugins
            void collectTopLevel( Node *node );

            //! extract node name, must match label and skip chars from string
            string getNodeName( const Node &node, const char *label, const size_t nskip ) const;

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

