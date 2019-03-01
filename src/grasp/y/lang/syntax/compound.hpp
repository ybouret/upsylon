//! \file

#ifndef Y_LANG_SYNTAX_COMPOUND_INCLUDED
#define Y_LANG_SYNTAX_COMPOUND_INCLUDED 1

#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            //! holds a reference to a Rule
            class Operand : public Object, public core::inode<Operand>
            {
            public:
                typedef core::inode<Operand>       Base; //!< alias
                typedef core::list_of_cpp<Operand> List; //!< alias

                explicit Operand( const Rule &r) throw(); //!< setup
                virtual ~Operand() throw();               //!< destructor

                const Rule &rule; //!< previously declared (persistent) rule

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Operand);
            };

            //! holds a list of operands
            class Compound : public Rule, public Operand::List
            {
            public:
                virtual ~Compound() throw();           //!< destructor
                void append( const Rule &r );          //!< push back a new operand
                Compound & operator<<( const Rule & ); //!< append a rule
                const bool acceptHollow;               //!< flag to accept appended ru;e

            protected:
                explicit Compound(const uint32_t, const string &, bool accept_hollow); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
            };

            //! aggregation of rules
            class Aggregate : public Compound
            {
            public:
                static const uint32_t UUID = Y_FOURCC('A', 'G', 'G', 0); //!< UUID

                explicit Aggregate(const string &id);     //!< setup
                virtual ~Aggregate() throw();             //!< destructor
                Aggregate & operator += ( const Rule & ); //!< syntactic sugar

                virtual bool        isHollow() const throw(); //!< true if empty of all operands are hollow
                virtual const char *typeName() const throw(); //!< "Aggregate"
                Y_LANG_SYNTAX_ACCEPT_PROTO();                 //!< must accept all operands

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            };

            //! alternation of rules, reject hollow operand at build time
            class Alternate : public Compound
            {
            public:
                static const uint32_t UUID = Y_FOURCC('A', 'L', 'T', 0); //!< UUID

                explicit Alternate(const string &id); //!< setup
                virtual ~Alternate() throw();         //!< desctructor
                Alternate & operator |= ( const Rule & ); //!< syntactic sugar

                virtual bool        isHollow() const throw(); //!< false by construction
                virtual const char *typeName() const throw(); //!< "Alternate"
                Y_LANG_SYNTAX_ACCEPT_PROTO();                 //!< accept the first possible operands
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alternate);
            };



        }
    }
}


#endif


