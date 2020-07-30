//! \file

#ifndef Y_JARGON_CHAR_INCLUDED
#define Y_JARGON_CHAR_INCLUDED 1

#include "y/jargon/context.hpp"
#include "y/core/inode.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    namespace Jargon {
        
        
        //------------------------------------------------------------------
        //
        //! a dynamic char for tokens, with a context
        /**
         the content of a Char is perfectly located
         */
        //------------------------------------------------------------------
        class Char : public Object, public inode<Char>, public Context
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef core::list_of_cpp<Char>     List;     //!< list base class
            
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            explicit Char(const Context &, const uint8_t) throw(); //!< setup
            explicit Char(const Char &)                   throw(); //!< copy
            virtual ~Char()                               throw(); //!< cleanup
            friend std::ostream & operator<<(std::ostream &, const Char &); //!< display

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const uint8_t code; //!< low level content
            
            
        private:
            Y_DISABLE_ASSIGN(Char);
           
        };
        
        
        
        
    }
    
}

#endif
