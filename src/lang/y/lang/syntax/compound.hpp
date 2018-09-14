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
            //! node holding a rule
            class Operand : public Object
            {
            public:
                typedef core::list_of_cpp<Operand> ListType; //!< base class for list

                //! list of operands
                class List : public ListType
                {
                public:
                    explicit List() throw();      //!< initialize
                    virtual ~List() throw();      //!< destructor
                    void     add( const Rule &r); //!< push back an operand
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(List);
                };

                const Rule &rule; //!< existing rule reference
                Operand    *next; //!< for list
                Operand    *prev; //!< for list

                virtual ~Operand() throw();              //!< destructor
                explicit Operand(const Rule &r) throw(); //!< initialize

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Operand);
            };

            //! base class for compound rule
            class Compound : public Rule, public Operand::List
            {
            public:
                virtual ~Compound() throw(); //!< desctructor
                //! syntax helper
                inline Compound & operator<<(const Rule &r) { add(r); return *this; }

            protected:
                //! initialize
                explicit Compound(const uint32_t t, const string &id);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
            };

            //! aggregate different rules
            class Aggregate : public Compound
            {
            public:
                //! class identifier
                static const uint32_t UUID = Y_FOURCC('A','G','G',' ');;
                explicit Aggregate(const string &id); //!< initialize
                virtual ~Aggregate() throw();         //!< destructor

                //! syntax helper
                inline Aggregate & operator += ( const Rule &r ) { add(r); return *this; }

                //! accept all or no nodes
                /**
                 if true, an empty sub-tree is added => not weak
                 */
                virtual bool accept( Y_LANG_SYNTAX_RULE_ARGS ) const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            };

            //! choice of different rules
            class Alternate : public Compound
            {
            public:
                //! class identifier
                static const uint32_t UUID = Y_FOURCC('A','L','T',' ');;
                explicit Alternate(const string &id); //!< initialize
                virtual ~Alternate() throw();         //!< destructor

                //! syntax helper
                inline Alternate & operator |= ( const Rule &r ) { add(r); return *this; }

                //! true if one of the operand accepts
                virtual bool accept( Y_LANG_SYNTAX_RULE_ARGS ) const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alternate);
            };
        }

    }
}

#endif

