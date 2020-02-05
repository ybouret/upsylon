//! \file
#ifndef Y_LANG_PATTERN_INCLUDED
#define Y_LANG_PATTERN_INCLUDED 1

#include "y/lang/token.hpp"
#include "y/lang/source.hpp"
#include "y/lang/alias.hpp"
#include "y/type/fourcc.hpp"
#include "y/ios/ocstream.hpp"


namespace upsylon {

    namespace Lang {
#define Y_PATTERN_PAD         '.'
#define Y_PATTERN_CC1(A)       Y_FOURCC(A,Y_PATTERN_PAD,Y_PATTERN_PAD,Y_PATTERN_PAD)
#define Y_PATTERN_CC2(A,B)     Y_FOURCC(A,B,Y_PATTERN_PAD,Y_PATTERN_PAD)
#define Y_PATTERN_CC3(A,B,C)   Y_FOURCC(A,B,C,Y_PATTERN_PAD)
#define Y_PATTERN_CC4(A,B,C,D) Y_FOURCC(A,B,C,D)
        
        //! a pattern to accept from a source
        class Pattern : public CountedObject, public Serializable, public Vizible
        {
        public:
            //------------------------------------------------------------------
            //
            //
            // types and definitions
            //
            //
            //------------------------------------------------------------------
            typedef core::list_of_cloneable<Pattern> List;   //!< list of cloneable patterns

            //------------------------------------------------------------------
            //
            //
            // public members
            //
            //
            //------------------------------------------------------------------

            const uint32_t uuid; //!< unique ID for the class
            Pattern       *next; //!< for List
            Pattern       *prev; //!< for List
            void          *priv; //!< pointer on derived type for optimization/compilation


            //------------------------------------------------------------------
            //
            //
            // public methods
            //
            //
            //------------------------------------------------------------------

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual         ~Pattern() throw();                             //!< destructor
            virtual Pattern *clone()                   const         = 0;   //!< clone
            virtual bool     weak()                    const throw() = 0;   //!< a pattern is weak if it matches an empty expression
            virtual bool     match(Token &t, Source &) const         = 0;   //!< try to match
            virtual bool     univocal()                const throw() = 0;   //!< guess if univocal
            virtual bool     equals(const Pattern &)   const throw() = 0;   //!< test equality

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            const char *vizStyle() const throw();                               //!< get GraphViz style for __viz if needed, based on weak()
            bool        multiple() const throw();                               //!< !univocal

            template <typename PAT>
            PAT *as() throw() {
                assert(PAT::UUID==uuid);
                return static_cast<PAT *>(priv);
            }

            template <typename PAT>
            const PAT *as() const throw() {
                assert(PAT::UUID==uuid);
                return static_cast<const PAT *>(priv);
            }

            //__________________________________________________________________
            //
            // static interface
            //__________________________________________________________________
            static Pattern *Load(ios::istream &);                                     //!< load from an input stream
            static Pattern *Optimize(Pattern *) throw();                              //!< optimize pattern
            static void     NoMultiple(List &ops) throw();                            //!< remove multiple same patterns in a list of operands

            friend std::ostream & operator<<(std::ostream &, const Pattern &);

        protected:
            explicit Pattern(const uint32_t id)  throw();  //!< build pattern with a given ID

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pattern);
        };

        //! macro to mark priv as class
#define Y_LANG_PATTERN_IS(CLASS) do { priv = static_cast<CLASS*>(this); } while(false)

        //! implementing class wide className
#define Y_LANG_PATTERN_CLID(CLASS) const char CLASS::CLID[8] = Y_FOURCC_CHAR8(CLASS::UUID)
        
    }
}

#endif

