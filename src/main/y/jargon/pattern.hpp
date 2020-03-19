//! \file

#ifndef Y_JARGON_PATTERN_INCLUDED
#define Y_JARGON_PATTERN_INCLUDED 1

#include "y/jargon/source.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon {
 
    namespace Jargon {
    
        class Pattern : public CountedObject,
        public inode<Pattern>,
        public Serializable
        {
        public:
            
            // virtual interface
            virtual          ~Pattern() throw();
            virtual Pattern * clone()                   const         = 0;
            virtual bool      alike(const Pattern *)    const throw() = 0;
            virtual bool      match(Token &t, Source &) const         = 0;   //!< try to match
            virtual bool      univocal()                const throw() = 0;   //!< guess if univocal
           
           
            
            // non virtual interface
            size_t emitUUID(ios::ostream &fp) const;
            bool   multiple() const throw(); //!< !univocal()
            
            
            template <typename PATTERN> inline
            PATTERN *as() throw()
            {
                assert(PATTERN::UUID==uuid);assert(self);
                return static_cast<PATTERN *>(self);
            }
            
            template <typename PATTERN> inline
            const PATTERN *as() const throw()
            {
                assert(PATTERN::UUID==uuid);assert(self);
                return static_cast<const PATTERN *>(self);
            }
            
            const uint32_t uuid;
            void          *self;
            
            void test(Source &source, Token &content) const;
            
        protected:
            explicit Pattern(const uint32_t) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pattern);
        };
        
        typedef arc_ptr<const Pattern> Motif;
        
        // registering
#define Y_PATTERN_SELF(CLASS) do { self = static_cast<CLASS*>(this); } while(false)
#define Y_PATTERN_CLID(CLASS) \
const char *CLASS:: className() const throw() { return CLASS::CLID; }\
const char CLASS::CLID[] = Y_FOURCC_CHAR8(CLASS::UUID)
        
        //! padding value for UUID of patterns
#define Y_PATTERN_PAD         '.'
        
        //! for 1 char UUID
#define Y_PATTERN_CC1(A)       Y_FOURCC(A,Y_PATTERN_PAD,Y_PATTERN_PAD,Y_PATTERN_PAD)
        
        //! for 2 chars UUID
#define Y_PATTERN_CC2(A,B)     Y_FOURCC(A,B,Y_PATTERN_PAD,Y_PATTERN_PAD)
        
        //! for 2 chars UUID
#define Y_PATTERN_CC3(A,B,C)   Y_FOURCC(A,B,C,Y_PATTERN_PAD)
        
        //! for 4 chars UUID
#define Y_PATTERN_CC4(A,B,C,D) Y_FOURCC(A,B,C,D)
        
    }
    
}

#endif

