//! \file

#ifndef Y_JARGON_PATTERN_EXCLUDED_INCLUDED
#define Y_JARGON_PATTERN_EXCLUDED_INCLUDED 1

#include "y/jargon/pattern/basic.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! matching any other ONE char
        //
        //----------------------------------------------------------------------
        class Excluded : public Basic
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('E','X','C','L'); //!< UUID
            static const char     CLID[];                           //!< CLID
            
            //------------------------------------------------------------------
            //
            // specific methods
            //
            //------------------------------------------------------------------
            virtual        ~Excluded() throw();        //!< cleanup
            static Pattern *Create(const uint8_t c); //!< create
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const uint8_t code; //!< the bad code
            
            
        protected:
            explicit Excluded(const uint8_t) throw(); //!< setup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Excluded);
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
