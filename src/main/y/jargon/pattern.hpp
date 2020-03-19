//! \file

#ifndef Y_JARGON_PATTERN_INCLUDED
#define Y_JARGON_PATTERN_INCLUDED 1

#include "y/jargon/source.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon {
 
    namespace Jargon {
    
        //! base class for patterns recognition
        class Pattern : public CountedObject,
        public Serializable
        {
        public:
            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
            virtual          ~Pattern() throw();                            //!< cleanup
            virtual Pattern * clone()                   const         = 0;  //!< clone
            virtual bool      alike(const Pattern *)    const throw() = 0;  //!< test equality
            virtual bool      match(Token &t, Source &) const         = 0;  //!< try to match
            virtual bool      feeble()                  const throw() = 0;  //!< accept empty token!
            virtual bool      univocal()                const throw() = 0;  //!< guess if univocal
           
           
            
            //------------------------------------------------------------------
            //
            // non virtual interface
            //
            //------------------------------------------------------------------
            size_t emitUUID(ios::ostream &fp) const; //!< helper to emit uuid
            bool   multiple() const throw();         //!< !univocal()
            bool   strong()   const throw();         //!< !feeble()
            
            //! retrieve derived class
            template <typename PATTERN> inline
            PATTERN *as() throw()
            {
                assert(PATTERN::UUID==uuid);assert(self);
                return static_cast<PATTERN *>(self);
            }
            
            //! retrieve const derived class
            template <typename PATTERN> inline
            const PATTERN *as() const throw()
            {
                assert(PATTERN::UUID==uuid);assert(self);
                return static_cast<const PATTERN *>(self);
            }
            
            //! test pattern on all source, save content for re-use
            void            test(Source &source, Token &content) const;
            
            //! load from previously serialized patterns
            static Pattern *Load(ios::istream&);
            
            //------------------------------------------------------------------
            //
            // common members
            //
            //------------------------------------------------------------------
            const uint32_t uuid; //!< identifier
            void          *self; //!< pointer to derived class
            
        protected:
            explicit Pattern(const uint32_t) throw(); //!< setup uuid, self=0
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pattern);
        };
        
        
        
        //! registering self
#define Y_PATTERN_SELF(CLASS) do { self = static_cast<CLASS*>(this); } while(false)
        
        //! uses CLID as className
#define Y_PATTERN_CLID(CLASS) \
const char *CLASS:: className() const throw() { return CLASS::CLID; }\
const char CLASS::CLID[] = Y_FOURCC_CHAR8(CLASS::UUID)
        
        //! padding value for UUID of patterns
#define Y_PATTERN_PAD         '_'
        
        //! for 1 char UUID
#define Y_PATTERN_CC1(A)       Y_FOURCC(A,Y_PATTERN_PAD,Y_PATTERN_PAD,Y_PATTERN_PAD)
        
        //! for 2 chars UUID
#define Y_PATTERN_CC2(A,B)     Y_FOURCC(A,B,Y_PATTERN_PAD,Y_PATTERN_PAD)
        
        //! for 2 chars UUID
#define Y_PATTERN_CC3(A,B,C)   Y_FOURCC(A,B,C,Y_PATTERN_PAD)
        
        //! for 4 chars UUID
#define Y_PATTERN_CC4(A,B,C,D) Y_FOURCC(A,B,C,D)
        
        //! smart pointer for a compilerd pattern
        typedef arc_ptr<const Pattern> Motif;
        
        
        
    }
    
}

#endif

