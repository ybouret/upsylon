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
            Syntax::Parser *create( Node &dynamo );




        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);
            typedef DynMeta<Syntax::Aggregate> MetaAgg;
            //typedef DynMeta<Syntax::Rule>      MetaRule;

            auto_ptr<Syntax::Parser>  parser;
            //Node::SaveList            modules;
            set<string,MetaAgg>       top;
            hashing::mperf htop;


            //! extract name from dynamo.children.head->lexeeme
            string getModuleName( const Node &dynamo ) const;

            //! collect top level rules and aliases
            void collectTopLevel( Node *node );

            //! extract node name
            string getNodeName( const Node &node, const char *label, const size_t nskip ) const;


            void collectSubModules( Node *node ) throw();
        };


    }

}

#endif

