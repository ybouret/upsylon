//! \file
#ifndef Y_LANG_PATTERN_JOKER_INCLUDED
#define Y_LANG_PATTERN_JOKER_INCLUDED 1

#include "y/lang/pattern.hpp"
#include "y/ptr/auto.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! joker base class
        class Joker : public Pattern
        {
        public:
            typedef auto_ptr<Pattern> Motif;       //!< alias
            virtual ~Joker() throw();              //!< destructor
            void optimize() throw();               //!< call optimize on the motif

        protected:
            const Motif motif;                                      //!< the motif
            explicit Joker(const uint32_t id, Pattern *jk) throw(); //!< initialize
            void vizlink( ios::ostream &fp ) const;                 //!< create a directed link

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Joker);
        };

        //! optional pattern
        class Optional : public Joker
        {
        public:
            static const uint32_t UUID = Y_FOURCC('?',0,0,0); //!< [0x?000]
            inline virtual ~Optional() throw() {}             //!< destructor
            //! create with memory management
            static  Pattern *Create( Pattern *jk );

            //! clone
            inline virtual Pattern *clone() const { return Create( motif->clone() ); }

            //! GraphViz
            virtual void __viz( ios::ostream &fp ) const;

            //! output
            virtual void write( ios::ostream &fp ) const;

            //! match
            virtual bool match( Token &tkn, Source &src) const;

            //! always true
            inline virtual bool weak() const throw() { return true; }

            //! always false
            virtual bool univocal() const throw();

        private:
            inline explicit Optional( Pattern *jk ) throw() : Joker(UUID,jk)
            {
                Y_LANG_PATTERN_IS(Optional);
            }
            Y_DISABLE_COPY_AND_ASSIGN(Optional);
        };

        //! #motif >= nmin
        class Repeating : public Joker
        {
        public:
            static const uint32_t UUID = Y_FOURCC('>','=',0,0); //!< ID
            const size_t          nmin; //!< minimal count

            inline virtual ~Repeating() throw() {}

            //! create with memory management, jk must NOT be weak!!
            static  Pattern *Create( Pattern *jk, const size_t n);
            
            //! clone
            inline virtual Pattern *clone() const { return Create( motif->clone(), nmin ); }

            //! GraphViz
            virtual void __viz( ios::ostream &fp ) const;

            //! output
            virtual void write( ios::ostream &fp ) const;

            //! match
            virtual bool match( Token &tkn, Source &src) const;

            //! check
            virtual bool weak() const throw()
            {
                assert(!motif->weak());
                return (nmin<=0);
            }

            //! '*'
            static inline Pattern *ZeroOrMore( Pattern *p ) { return Repeating::Create(p,0); }

            //! '+'
            static inline Pattern *OneOrMore( Pattern * p ) { return Repeating::Create(p,1); }

            virtual bool univocal() const throw(); //!< false

        private:
            inline explicit Repeating( Pattern *jk, const size_t n) throw() : Joker(UUID,jk), nmin(n)
            {
                assert(! motif->weak() );
                Y_LANG_PATTERN_IS(Repeating);
            }

            Y_DISABLE_COPY_AND_ASSIGN(Repeating);
        };


        //! nmin <= count <= nmax
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

