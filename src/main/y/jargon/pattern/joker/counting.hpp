

//! \file
#ifndef Y_JARGON_PATTERN_COUNTING_INCLUDED
#define Y_JARGON_PATTERN_COUNTING_INCLUDED 1

#include "y/jargon/pattern/joker.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! accept a given number of repeated pattern
        //
        //----------------------------------------------------------------------
        class Counting : public Joker
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('C','N','T','_'); //!< UUID
            static const char     CLID[];                           //!< CLID
            
            //------------------------------------------------------------------
            //
            // specific methods
            //
            //------------------------------------------------------------------
            virtual         ~Counting() throw(); //!< cleanup
            static  Pattern *Create(Pattern *, const size_t, const size_t);  //!< create!
            static  Pattern *Create(Pattern *, const size_t);                //!< create with one count

            const size_t minimalCount; //!< minimal valid count
            const size_t maximalCount; //!< maximal valid count
            
        private:
            explicit Counting(const Motif &, const size_t, const size_t) throw();
            explicit Counting(const Counting&);
            Y_DISABLE_ASSIGN(Counting);
            virtual Pattern    *clone()                    const;
            virtual bool        alike(const Pattern *)     const throw();
            virtual bool        univocal()                 const throw();
            virtual bool        feeble()                   const throw();
            virtual bool        match(Token&,Source&)      const;
            virtual void        express(ios::ostream &)    const;

            virtual size_t      serialize( ios::ostream &) const;
            virtual const char *className()                const throw();
            
            virtual void        vizCore(ios::ostream &fp)  const;
        };
        
    }
    
}

#endif
