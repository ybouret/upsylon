
//! \file

#ifndef Y_JARGON_TOKEN_INCLUDED
#define Y_JARGON_TOKEN_INCLUDED 1

#include "y/jargon/char.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        class Token : public Char::List
        {
        public:
            explicit Token(const Char::Cache &) throw();
            Token( const Token &);
            virtual ~Token() throw();
            virtual void release() throw();
            
            //!
            void append(const Context &, const uint8_t);
            void erase() throw();
            
            friend std::ostream & operator<<( std::ostream &, const Token &);
           
            Char::Cache cache;

            void skip(size_t n) throw();
            void trim(size_t n) throw();
            
        private:
            Y_DISABLE_ASSIGN(Token);
        };
        
    }
    
}

#endif


