//! \file

#ifndef Y_LANG_DYNAMO_GENERATOR_INCLUDED
#define Y_LANG_DYNAMO_GENERATOR_INCLUDED 1

#include "y/lang/syntax/parser.hpp"
#include "y/associative/map.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{

    namespace Lang
    {

        //! Generate a parser from a Dynamo compiled AST
        class DynamoGenerator
        {
        public:
            typedef Syntax::Node       Node;
            typedef Syntax::Aggregate  AGG;

            //! initialize
            explicit DynamoGenerator();

            //! destructor
            virtual ~DynamoGenerator() throw();

            //! from top-level
            Syntax::Parser *create( Node &dynamo );





        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);
            class MetaAgg
            {
            public:
                AGG &agg;
                inline const string &key() const throw() { return agg.name; }
                inline  MetaAgg( AGG &r ) throw() : agg(r) {}
                inline ~MetaAgg() throw() {}
                inline MetaAgg(const MetaAgg &other) throw() : agg(other.agg) {}

            private:
                Y_DISABLE_ASSIGN(MetaAgg);
            };

            auto_ptr<Syntax::Parser> parser;
            Node::SaveList           modules;
            set<string,MetaAgg>      top;


            //! extract name from dynamo.children.head->lexeeme
            string getModuleName( const Node &dynamo ) const;

            //! collect top level rules
            void collectTopLevel( const Node *node );


            void collectSubModules( Node *node ) throw();
        };


    }

}

#endif

