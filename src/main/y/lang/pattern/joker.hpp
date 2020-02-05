//! \file
#ifndef Y_LANG_PATTERN_JOKER_INCLUDED
#define Y_LANG_PATTERN_JOKER_INCLUDED 1

#include "y/lang/pattern.hpp"
#include "y/ptr/auto.hpp"
#include "y/type/utils.hpp"

namespace upsylon {

    namespace Lang {

        //----------------------------------------------------------------------
        //
        //! joker base class
        //
        //----------------------------------------------------------------------
        class Joker : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef auto_ptr<Pattern> Motif; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Joker()    throw();     //!< destructor
            void     optimize() throw();     //!< call optimize on the motif

        protected:
            explicit Joker(const uint32_t, Pattern *) throw(); //!< initialize
            void     vizlink(ios::ostream &) const;            //!< create a directed link

            const Motif motif; //!< the motif

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Joker);
        };

        //----------------------------------------------------------------------
        //
        //! optional pattern
        //
        //----------------------------------------------------------------------
        class Optional : public Joker
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const uint32_t UUID = Y_PATTERN_CC1('?'); //!< [0x?   ]
            static const char     CLID[8];                   //!< UUID

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual         ~Optional() throw() ;             //!< destructor
            static  Pattern *Create(Pattern *jk);             //!< create with memory management

            //__________________________________________________________________
            //
            // interface: Pattern
            //__________________________________________________________________
            virtual Pattern *clone() const;                   //!< clone
            virtual void     vizCore(ios::ostream &)  const;  //!< GraphViz
            virtual bool     match(Token &, Source &) const;  //!< match
            virtual bool     weak()     const throw();        //!< always true
            virtual bool     univocal() const throw();        //!< always false

            //__________________________________________________________________
            //
            // interface: Serializable
            //__________________________________________________________________
            virtual size_t   serialize(ios::ostream &) const; //!< [UUID] [motif]
            const char *     className() const throw();       //!< CLID

        private:
            explicit Optional(Pattern *jk) throw();           //!< setup
            Y_DISABLE_COPY_AND_ASSIGN(Optional);
        };

        //----------------------------------------------------------------------
        //
        //! #motif >= nmin
        //
        //----------------------------------------------------------------------
        class Repeating : public Joker
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const uint32_t UUID = Y_PATTERN_CC2('>','='); //!< ID
            static const uint32_t _ZOM = Y_PATTERN_CC1('*');     //!< nmin=0
            static const uint32_t _OOM = Y_PATTERN_CC1('+');     //!< nmin=1
            static const char     CLID[8];                       //!< UUID
            static const char     _ID0[8];                       //!< _ZOM
            static const char     _ID1[8];                       //!< _OOM

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            const size_t     nmin;                              //!< minimal count
            virtual         ~Repeating() throw();               //!< cleanup
            virtual Pattern *clone() const;                     //!< clone
            virtual void     vizCore(ios::ostream &)  const;    //!< GraphViz
            virtual bool     match(Token &, Source &) const;    //!< match
            virtual bool     weak()     const throw();          //!< check
            virtual bool     univocal() const throw();          //!< false
            virtual size_t   serialize(ios::ostream &) const;   //!< [UUID] [nmin] [motif]
            const char *     className() const throw();         //!< CLID

            //__________________________________________________________________
            //
            // Static Methods
            //__________________________________________________________________
            static Pattern  *ZeroOrMore(Pattern *);             //!< '*'
            static Pattern  *OneOrMore(Pattern *);              //!< '+'
            static Pattern  *Create(Pattern *, const size_t);   //!< create with memory management, jk must NOT be weak!!

        private:
            explicit Repeating(Pattern *jk, const size_t n) throw();
            Y_DISABLE_COPY_AND_ASSIGN(Repeating);
        };


        //----------------------------------------------------------------------
        //
        //! nmin <= count <= nmax
        //
        //----------------------------------------------------------------------
        class Counting : public Joker
        {
        public:
            static const uint32_t UUID = Y_PATTERN_CC2('[',']'); //!< ID
            static const char     CLID[8];                       //!< UUID
            const size_t          nmin;                          //!< minimal count
            const size_t          nmax;                          //!< maximal count

            virtual         ~Counting() throw();              //!< destructor
            virtual Pattern *clone() const;                   //!< clone
            virtual void     vizCore(ios::ostream &)   const; //!< GraphViz
            virtual bool     match(Token &, Source &)  const; //!< match
            virtual bool     weak()     const throw();        //! nmin<=0
            virtual bool     univocal() const throw();        //!< true if nmin==nmax and motif->univocal
            virtual size_t   serialize(ios::ostream &) const; //!< [UUID] [nmin] [nmax] [motif]
            const char *     className() const throw();         //!< CLID

            //! create with memory management
            static  Pattern *Create(Pattern *jk, const size_t n, const size_t m);

        private:
            explicit Counting(Pattern *jk, const size_t n, const size_t m) throw();
            Y_DISABLE_COPY_AND_ASSIGN(Counting);
        };

    }
}

#endif

