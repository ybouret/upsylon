//! \file

#ifndef Y_JIVE_CONTEXT_INCLUDED
#define Y_JIVE_CONTEXT_INCLUDED 1

#include "y/jive/tags.hpp"

namespace upsylon {

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
            
            //! generic constructor
            template <typename T>
            inline explicit Context(const T &args) :
            tag(Tags::Make(args)),
            line(init_line),
            column(init_column)
            {
            }

            explicit Context(const Context &) throw();  //!< copy constructor
            virtual ~Context() throw();                 //!< cleanup

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
