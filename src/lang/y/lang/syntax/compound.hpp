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
                //! role in grammar
                enum Type
                {
                    Normal, //!< keep as is
                    Acting, //!< temporary, merge all
                    Design  //!< design only, merge if one child!
                };

                const Type type; //!< flag if the compound is an acting compound

                void setActing() throw(); //!< set as Acting from Normal
                void setDesign() throw(); //!< set as Design form Normal

                //! destructor
                virtual ~Compound() throw();

                //! syntax helper
                inline Compound & operator<<(const Rule &r) { add(r); return *this; }

                //! linking
                virtual void epilog( ios::ostream &fp ) const;

                //! specific style
                virtual const char * __style() const throw()
                {
                    switch(type)
                    {
                        case Acting: return "dotted";
                        case Design: return "rounded";
                        default:
                            break;
                    }
                    return "solid";
                }
            protected:
                //! initialize, data=self
                explicit Compound(const uint32_t t, const string &id, const Type flag);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
            };

            //! aggregate different rules
            class Aggregate : public Compound
            {
            public:
                //! class identifier
                static const uint32_t UUID = Y_FOURCC('A','G','G',' ');;
                
                explicit Aggregate(const string &id, const Type = Normal);  //!< initialize
                virtual ~Aggregate() throw();                      //!< destructor

                //! syntax helper
                inline Aggregate & operator += ( const Rule &r ) { add(r); return *this; }

                //! accept all or no nodes
                /**
                 if true, an empty sub-tree is added => not hollow!
                 */
                virtual bool accept( Y_LANG_SYNTAX_RULE_ARGS ) const;

                //! always create a sub-tree with its label, never hollow
                /**
                 But the tree can be empty.
                 */
                virtual bool hollow() const throw();

                //! specific shpae
                virtual const char * __shape() const throw() { return "house"; }

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

                virtual const char * __shape() const throw() { return "oval"; }

                //! true if one of the member is hollow
                virtual bool hollow() const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alternate);
            };
        }

    }
}

#endif

