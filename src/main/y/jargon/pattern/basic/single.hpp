
//! \file
#ifndef Y_JARGON_PATTERN_SINGLE_INCLUDED
#define Y_JARGON_PATTERN_SINGLE_INCLUDED 1

#include "y/jargon/pattern/basic.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! matching a single char
        //
        //----------------------------------------------------------------------
        class Single : public Basic
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('S','N','G','L'); //!< UUID
            static const char     CLID[];                           //!< CLID
            
            //------------------------------------------------------------------
            //
            // specific methods
            //
            //------------------------------------------------------------------
            virtual        ~Single() throw();        //!< cleanup
            static Pattern *Create(const uint8_t c); //!< create
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const uint8_t code; //!< the matching code
            
            
        protected:
            explicit Single(const uint8_t) throw(); //!< setup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Single);
            virtual Pattern    *clone()                    const;
            virtual bool        alike(const Pattern *)     const throw();
            virtual bool        good(const uint8_t)        const throw();
            virtual bool        univocal()                 const throw();
            virtual void        adjoin( FirstChars &)      const;
            virtual void        express(ios::ostream &)    const;

            virtual size_t      serialize( ios::ostream &) const;
            virtual const char *className()                const throw();
            virtual void        vizCore(ios::ostream &fp)  const;

        };
        
    }
    
}

#endif

