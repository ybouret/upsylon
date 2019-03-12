//! \file
#ifndef Y_DYNAMO_COMPILER_INCLUDED
#define Y_DYNAMO_COMPILER_INCLUDED 1

#include "y/lang/dynamo/loader.hpp"

namespace upsylon
{
    namespace Lang
    {

        
        template <typename T>
        class DynamoRef
        {
        public:
            Y_DECL_ARGS(T,type); //!< alias
            typedef set<string,DynamoRef> Set;

            type &rule;

            inline  DynamoRef( type &r ) throw() : rule(r) {}
            inline ~DynamoRef() throw() {}
            inline  DynamoRef( const DynamoRef &other ) throw() : rule(other.rule) {}
            const string & key() const throw() { return rule.name; }

        private:
            Y_DISABLE_ASSIGN(DynamoRef);
        };

        typedef DynamoRef<Syntax::Compound> DynamoRule;
        typedef DynamoRef<Syntax::Terminal> DynamoTerm;


        //! build an intermediate tree
        class DynamoCompiler : public DynamoLoader
        {
        public:
            explicit DynamoCompiler();
            virtual ~DynamoCompiler() throw();
            

            Syntax::Parser *compile( XNode &top );

            DynamoRule::Set rules;
            DynamoRule::Set terms;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoCompiler);
            
        };
        
    }
}

#endif

