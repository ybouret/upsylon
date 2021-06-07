
//! \file
#ifndef Y_JIVE_LANGUAGE_ANALYZER_INCLUDED
#define Y_JIVE_SYNTAX_ANALYZER_INCLUDED 1

#include "y/jive/language/node.hpp"


namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {

            //__________________________________________________________________
            //
            //
            //! base class to analyze AST
            //
            //__________________________________________________________________
            class Analyzer
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                template <typename ID> inline
                explicit Analyzer(const ID &id) :
                name( Tags::Make(id) ),
                depth(0),
                maxLength(0)
                {
                }

                //! cleanup
                virtual ~Analyzer() throw();

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________

                //! what to do when a terminal is met
                virtual int onTerminal(const string &id, const Lexeme &data);

                //! what to do when an internal is met
                virtual int onInternal(const string &id, const size_t  size);

                //______________________________________________________________
                //
                // non virtual interface
                //______________________________________________________________

                //! walk down the node
                int walk(const Node *);

                //! helper: indenting
                template <typename OSTREAM>
                OSTREAM & indent( OSTREAM &os ) const
                {
                    for(size_t i=depth;i>0;--i) os << '.';
                    os << "|_";
                    return os;
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________

                const Tag    name;      //!< should be grammar name
                const size_t depth;     //!< current depth
                size_t       maxLength; //!< max length of lexemes

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
                int walkDown(const Node *node);
                int walkDownInternal(const Node *node);
            };

        }

    }

}


#endif

