//! \file
#ifndef Y_JARGON_PATTERN_REPEATING_INCLUDED
#define Y_JARGON_PATTERN_REPEATING_INCLUDED 1

#include "y/jargon/pattern/joker.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! accept any char
        class Repeating : public Joker
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            
            static const uint32_t UUID  = Y_FOURCC('R','E','P','_'); //!< UUID
            static const char     CLID[];                            //!< CLID
            
            static const uint32_t UUID0 = Y_FOURCC('Z','O','M','_'); //!< UUID0
            static const char     CLID0[];                           //!< CLID0
            
            static const uint32_t UUID1 = Y_FOURCC('O','O','M','_'); //!< UUID1
            static const char     CLID1[];                           //!< CLID1
            
            
            //------------------------------------------------------------------
            //
            // specific methods
            //
            //------------------------------------------------------------------
            virtual         ~Repeating() throw();              //!< cleanup
            static  Pattern *Create(Pattern *, const size_t);  //!< create!
            static  Pattern *ZeroOrMore(Pattern *);            //!< Create(p,0)
            static  Pattern *OneOrMore(Pattern *);             //!< Create(p,1)

            const size_t minimalCount; //!< minmal count of pattern to be valid
            
        private:
            explicit Repeating(const Motif &,const size_t) throw();
            explicit Repeating(const Repeating&);
            Y_DISABLE_ASSIGN(Repeating);
            virtual Pattern    *clone()                    const;
            virtual bool        alike(const Pattern *)     const throw();
            virtual bool        univocal()                 const throw();
            virtual bool        feeble()                   const throw();
            virtual bool        match(Token&,Source&)      const;
            
            virtual size_t      serialize( ios::ostream &) const;
            virtual const char *className()                const throw();
            
            virtual void        vizCore(ios::ostream &fp)  const;
        };
        
    }
    
}

#endif
