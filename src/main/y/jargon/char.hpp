//! \file

#ifndef Y_JARGON_CHAR_INCLUDED
#define Y_JARGON_CHAR_INCLUDED 1

#include "y/jargon/context.hpp"
#include "y/core/inode.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    namespace Jargon {
        
        
        
        //! a dynamic char for tokens, with a context
        /**
         the content of a Char is perfectly located
         */
        class Char : public Object, public inode<Char>, public Context
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
           
            //------------------------------------------------------------------
            // for Token
            //------------------------------------------------------------------
            typedef core::list_of<Char>     List;     //!< list base class
            typedef core::list_of_cpp<Char> PoolType; //!< pool base class
            
            //------------------------------------------------------------------
            // for Cache
            //------------------------------------------------------------------
            
            //! a pool to create a char cache
            class Pool : public CountedObject, public PoolType
            {
            public:
                explicit Pool() throw(); //!< setup
                virtual ~Pool() throw(); //!< cleanup
                void     store( Char *ch ) throw(); //!< store a used Char
                void     store( List &l  ) throw(); //!< store a used Token
                Char    *query() throw();           //!< query a used Char
                
                void     optimize();
                void     maxChars(const size_t max_chars); //!< keep no more that max_chars in cache
                void     maxBytes(const size_t max_bytes); //!< keep no more that max_bytes in cache
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Pool);
            };
            
            typedef arc_ptr<Pool> Cache; //!< alias
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            virtual     ~Char() throw();                                     //!< cleanup
            static Char *Make(Cache &, const Context &, const uint8_t);      //!< make a Char
            static Char *Copy(Cache &, const Char &);                        //!< copy a Char
            friend std::ostream & operator<<( std::ostream &, const Char &); //!< display

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const uint8_t code; //!< low level content
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Char);
            explicit Char(const Context &context,
                          const uint8_t  content) throw();
        };
        
        
    }
    
}

#endif
