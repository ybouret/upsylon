//! \file

#ifndef Y_JIVE_CONTEXT_INCLUDED
#define Y_JIVE_CONTEXT_INCLUDED 1

#include "y/jive/tags.hpp"
#include "y/string.hpp"

namespace upsylon {

    class exception; //!< forward declaration

    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //! a fully qualified contex: tag+line+column
        //
        //______________________________________________________________________
        class Context
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const int init_line   = 1; //!< default line
            static const int init_column = 1; //!< default column

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            explicit Context(const char    *);          //!< setup
            explicit Context(const string  &);          //!< setup
            explicit Context(const Tag     &) throw();  //!< setup
            explicit Context(const Context &) throw();  //!< copy constructor
            virtual ~Context() throw();                 //!< cleanup

            //! mark as "tag:line:column: "
            exception & cat(exception &excp) const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Tag tag;    //!< identifier
            const int line;   //!< current line
            const int column; //!< current column

        private:
            Y_DISABLE_ASSIGN(Context);
        };

    }

}

#endif
