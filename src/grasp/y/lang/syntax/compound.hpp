//! \file

#ifndef Y_LANG_SYNTAX_COMPOUND_INCLUDED
#define Y_LANG_SYNTAX_COMPOUND_INCLUDED 1

#include "y/lang/syntax/internal.hpp"

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

            //! compound behaviors for AST
            enum Behavior
            {
                SubGroup, //!< keep grouped
                MergeOne, //!< merge if only one child
                MergeAll  //!< merge with parent node for any number of children
            };

            //! holds a list of operands
            class Compound : public Internal, public Operand::List
            {
            public:
                //! policy to accept or reject hollow operands
                enum HollowPolicy
                {
                    AcceptHollowOperand, //!< would accept
                    RejectHollowOperand  //!< would reject

                };
                virtual ~Compound() throw();           //!< destructor
                void append( const Rule &r );          //!< push back a new operand
                Compound & operator<<( const Rule & ); //!< append a rule

                virtual void        graphVizEpilog(ios::ostream &) const; //!< build links

                const Behavior behavior; //!< the behavior for the AST



            protected:
                explicit Compound(const uint32_t,
                                  const string   &,
                                  const Behavior,
                                  const HollowPolicy); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
                const HollowPolicy hollowPolicy; //!< checked upon append
            };

            //! aggregation of rules
            class Aggregate : public Compound
            {
            public:
                static const uint32_t UUID = Y_FOURCC('A', 'G', 'G', 0); //!< UUID

                explicit Aggregate(const string &id);     //!< setup
                virtual ~Aggregate() throw();             //!< destructor
                Aggregate & operator += ( const Rule & ); //!< syntactic sugar

                virtual bool        isHollow() const throw();      //!< true if empty of all operands are hollow
                virtual const char *typeName() const throw();      //!< "Aggregate"
                Y_LANG_SYNTAX_ACCEPT_PROTO();                      //!< must accept all operands
                virtual const char *graphVizShape() const throw(); //!< the shape
                virtual const char *graphVizStyle() const throw(); //!< change style according to behavior


                //! change behavior
                Aggregate & will( Behavior newBehavior ) throw() { (Behavior &)behavior = newBehavior; return *this; }
                void  autoUpgrade() throw(); //!< will detect design aggregate


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

                virtual bool        isHollow() const throw();      //!< false by construction
                virtual const char *typeName() const throw();      //!< "Alternate"
                Y_LANG_SYNTAX_ACCEPT_PROTO();                      //!< accept the first possible operands
                virtual const char *graphVizShape() const throw(); //!< the shape
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alternate);
            };



        }
    }
}


#endif


