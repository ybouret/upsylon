//! \file

#ifndef Y_LANG_DYNAMO_GENERATOR_INCLUDED
#define Y_LANG_DYNAMO_GENERATOR_INCLUDED 1

#include "y/lang/syntax/parser.hpp"
#include "y/associative/map.hpp"
#include "y/associative/set.hpp"
#include "y/hashing/mph.hpp"

namespace upsylon
{

    namespace Lang
    {

        template <typename T>
        class DynMeta
        {
        public:
            T &ref;
            inline  DynMeta( T &r ) throw() : ref(r) {}
            inline ~DynMeta() throw() {}
            inline  DynMeta(const DynMeta &other) throw() : ref(other.ref) {}
            inline const string &key() const throw() { return ref.name; }

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
            typedef DynMeta<Syntax::Aggregate> MetaAgg;
            
            auto_ptr<Syntax::Parser>  parser;
            bool                      verbose;
            set<string,MetaAgg>       top;
            int                       level; //!< for tree walking
            hashing::mperf            htop;
            hashing::mperf            hstr;
            hashing::mperf            hlxr;
            int                       icom; //!< index for comment
            
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


            void collectSubModules( Node *node ) throw();

            std::ostream & indent() const; //!< indent for verbose
            string getCommentLabel();
        };


    }

}

#endif

