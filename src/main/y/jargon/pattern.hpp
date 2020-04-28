//! \file

#ifndef Y_JARGON_PATTERN_INCLUDED
#define Y_JARGON_PATTERN_INCLUDED 1

#include "y/jargon/source.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon {
 
    namespace Jargon {
    
        class FirstChars;
        
        //----------------------------------------------------------------------
        //
        //! base class for patterns recognition
        //
        //------------------------------------------------------------------
        class Pattern :
        public CountedObject,
        public inode<Pattern>,
        public Serializable,
        public Vizible
        {
        public:
            //------------------------------------------------------------------
            //
            //types and definitions
            //
            //------------------------------------------------------------------
            typedef core::list_of_cloneable<Pattern> List; //!< alias
           
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
            virtual void      adjoin( FirstChars &)     const         = 0;  //!< adjoin first char(s)
            virtual void      express(ios::ostream &)   const         = 0;  //!< regular expression
            
            //------------------------------------------------------------------
            //
            // non virtual interface
            //
            //------------------------------------------------------------------
            bool         multiple() const throw();         //!< !univocal()
            bool         strong()   const throw();         //!< !feeble()
            const  char *vizStyle() const throw();         //!< according to string/feeble
            
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
            
            
            void   test(Source &, Token &) const; //!< test pattern on all source, save content for re-use
            bool   checkIO()               const; //!< check serialization
            string toRegExp()              const; //!< use express()
            
            //! matching extacly
            bool matches_exactly(Token &, const string &) const;
            
            //! matching partly
            bool matches_partly(Token &, const string &) const;
          
            //------------------------------------------------------------------
            //
            // global static methods
            //
            //------------------------------------------------------------------
            static Pattern *Load(ios::istream&);                    //!< load from previously serialized patterns
            static Pattern *Optimize( Pattern *p ) throw();         //!< optimize
            static void     Express(ios::ostream &, const uint8_t); //!< code to compilable regexp
            
            //------------------------------------------------------------------
            //
            // common members
            //
            //------------------------------------------------------------------
            const uint32_t uuid;    //!< identifier
            void          *self;    //!< pointer to derived class
            const double   entropy; //!< entropy, if necessary
            
            void updateEntropy() const throw(); //!< update current entropy
            
        protected:
            explicit Pattern(const uint32_t) throw(); //!< setup uuid, self=0
            explicit Pattern(const Pattern&) throw(); //!< setup uuid, self=0
            
        private:
            Y_DISABLE_ASSIGN(Pattern);
            
        public:
            //------------------------------------------------------------------
            //
            // advanced ops
            //
            //------------------------------------------------------------------
            
            //! remove redundant alike patterns from the list, preserve order
            static void RemoveRedundant(List &patterns );
            
            //! pattern-wise procedure
            typedef Pattern * (*Proc)( Pattern *);
            
            //! replace all patterns by its transformed
            static void Transform(List &patterns, Proc proc ) throw();
            
            //! merge all patterns pairwise
            /**
             for a block of OR'd equivalent patterns, get
             the most simplified version (merge ranges, consecutive singles...)
             */
            static void PairwiseMerge(List &patterns);
            
            //! sort by increasing entropy
            static void SortByEntropy(List &patterns);
            
        };
        
        
        
        //! registering self
#define Y_PATTERN_SELF(CLASS) do { self = static_cast<CLASS*>(this); } while(false)
        
        //! uses CLID as className
#define Y_PATTERN_CLID(CLASS) \
const char *CLASS:: className() const throw() { return CLASS::CLID; }\
const char  CLASS::CLID[8] = Y_FOURCC_CHAR8(CLASS::UUID)
        
        
        //! smart pointer for a compilerd pattern
        typedef arc_ptr<const Pattern> Motif;
        
        
        
    }
    
}

#endif

