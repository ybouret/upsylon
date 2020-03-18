//! \file

#ifndef Y_JARGON_CHAR_INCLUDED
#define Y_JARGON_CHAR_INCLUDED 1

#include "y/jargon/context.hpp"
#include "y/core/inode.hpp"
#include "y/core/pool.hpp"

namespace upsylon {

    namespace Jargon {
        
        
        class Char : public Object, public inode<Char>, public Context
        {
        public:
            typedef core::list_of<Char>     List_;
            typedef core::list_of_cpp<Char> List;
           
            
            typedef core::pool_of_cpp<Char> PoolType;
            
            class Pool : public CountedObject, public PoolType
            {
            public:
                explicit Pool() throw();
                virtual ~Pool() throw();
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Pool);
            };
            
            typedef arc_ptr<Pool> Cache;
            
            
            virtual ~Char() throw();
            
            static Char *Make(Cache &, const Context &, const uint8_t);
            static Char *Copy(Cache &, const Char &);
            
            const uint8_t code;
            
            friend std::ostream & operator<<( std::ostream &, const Char &);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Char);
            explicit Char(const Context &context,
                          const uint8_t  content) throw();
        };
        
        
    }
    
}

#endif
