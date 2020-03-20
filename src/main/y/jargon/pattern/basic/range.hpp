
//! \file
#ifndef Y_JARGON_PATTERN_RANGE_INCLUDED
#define Y_JARGON_PATTERN_RANGE_INCLUDED 1

#include "y/jargon/pattern/basic.hpp"


namespace upsylon {
    
    namespace Jargon {
        
        //! accept a range [lower-upper] of chars
        class Range : public Basic
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('R','N','G','_'); //!< UUID
            static const char     CLID[];                           //!< CLID
            
            //------------------------------------------------------------------
            //
            // specific methods
            //
            //------------------------------------------------------------------
            virtual        ~Range() throw();                          //!< cleanup
            static Pattern *Create(const uint8_t a, const uint8_t b); //!< create with optimization if a==b
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const uint8_t lower; //!< lower bound
            const uint8_t upper; //!< upper bound
            
        protected:
            explicit Range(const uint8_t,const uint8_t) throw(); //!< setup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Range);
            virtual Pattern    *clone()                    const;
            virtual bool        alike(const Pattern *)     const throw();
            virtual bool        good(const uint8_t)        const throw();
            virtual bool        univocal()                 const throw();
            
            virtual size_t      serialize( ios::ostream &) const;
            virtual const char *className()                const throw();
            virtual void        vizCore(ios::ostream &fp)  const;

        };
        
    }
    
}

#endif

