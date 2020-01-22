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
            typedef auto_ptr<const Pattern> Motif; //!< alias
            virtual ~Joker() throw();              //!< destructor
            void optimize() throw();               //!< call optimize on the motif

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
            static const uint32_t UUID = Y_FOURCC('?',0,0,0); //!< [0x?000]

            virtual         ~Optional() throw() ;             //!< destructor
            virtual Pattern *clone() const;                   //!< clone
            virtual void     __viz(ios::ostream &)    const;  //!< GraphViz
            virtual void     write(ios::ostream &)    const;  //!< output
            virtual bool     match(Token &, Source &) const;  //!< match
            virtual bool     weak()     const throw();        //!< always true
            virtual bool     univocal() const throw();        //!< always false
            static  Pattern *Create(Pattern *jk);             //!< create with memory management

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
            static const uint32_t UUID = Y_FOURCC('>','=',0,0); //!< ID
            const size_t          nmin;                         //!< minimal count

            virtual         ~Repeating() throw();               //!< cleanup
            virtual Pattern *clone() const;                     //!< clone
            virtual void     __viz(ios::ostream &)    const;    //!< GraphViz
            virtual void     write(ios::ostream &)    const;    //!< output
            virtual bool     match(Token &, Source &) const;    //!< match
            virtual bool     weak()     const throw();          //!< check
            virtual bool     univocal() const throw();          //!< false

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
            static const uint32_t UUID = Y_FOURCC('<','=','=','>'); //!< ID
            const size_t          nmin; //!< minimal count
            const size_t          nmax; //!< maximal count

            //! destructor
            inline virtual ~Counting() throw() {}

            //! create with memory management
            static  Pattern *Create( Pattern *jk, const size_t n, const size_t m);

            //! clone
            inline virtual Pattern *clone() const { return Create( motif->clone(), nmin, nmax); }

            //! GraphViz
            virtual void __viz( ios::ostream &fp ) const;

            //! output
            virtual void write( ios::ostream &fp ) const;

            //! nmin<=0
            virtual bool weak() const throw()
            {
                assert(! motif->weak() );
                return (nmin<=0);
            }

            //! match
            virtual bool match( Token &tkn, Source &src) const;

            //! true if nmin==nmax and motif->univocal
            virtual bool univocal() const throw();

        private:
            inline explicit Counting( Pattern *jk, const size_t n, const size_t m) throw() : Joker(UUID,jk), nmin(n), nmax(m)
            {
                Y_LANG_PATTERN_IS(Counting);
                assert(nmin<=nmax);
            }

            Y_DISABLE_COPY_AND_ASSIGN(Counting);
        };

    }
}

#endif

