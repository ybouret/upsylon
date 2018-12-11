//! \file
#ifndef Y_LANG_PATTERN_BASIC_INCLUDED
#define Y_LANG_PATTERN_BASIC_INCLUDED 1

#include "y/lang/pattern.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! Matching One char
        class Match1 : public Pattern
        {
        public:
            inline virtual ~Match1() throw() {}

            //! common match function
            virtual bool match( Token &tkn, Source &src ) const;

            //! false
            virtual bool          weak() const throw() { return false; }

        protected:
            //! initialize
            inline explicit Match1(const uint32_t id) throw() : Pattern(id) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Match1);
            virtual bool accept_byte( const uint8_t c ) const throw() = 0;
        };

        //! Matching Any One char
        class Any1 : public Match1
        {
        public:
            static const uint32_t UUID = Y_FOURCC('A', 'N', 'Y', '1'); //!< 0xANY1
            inline virtual ~Any1() throw() {}                          //!< destructor
            //! constructor
            inline explicit Any1() throw() : Match1(UUID)
            {
                Y_LANG_PATTERN_IS(Any1);
            }

            inline virtual  Any1 *clone() const { return new Any1(); } //!< clone
            virtual void          __viz( ios::ostream &fp ) const;     //!< GraphViz
            virtual void          write( ios::ostream &fp ) const;     //!< output

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Any1);
            inline virtual bool accept_byte( const uint8_t ) const throw()
            {
                return true;
            }
        };

        //! Matching a Single char
        class Single : public Match1
        {
        public:
            static const uint32_t UUID = Y_FOURCC('S', 'N', 'G', 'L'); //!< 0xSNGL
            const uint8_t         code;                                //!< the matching code

            //! initialize
            inline explicit Single(const uint8_t c) throw() : Match1(UUID), code(c)
            {
                Y_LANG_PATTERN_IS(Single);
            }
            //! destructor
            inline virtual ~Single() throw() {}

            inline virtual Single *clone() const { return new Single(code); } //!< output
            virtual void           __viz( ios::ostream &fp ) const;           //!< GraphViz
            virtual void           write( ios::ostream &fp ) const;           //!< output

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Single);
            inline virtual bool accept_byte( const uint8_t c) const throw()
            {
                return c==code;
            }
        };

        //! matching a char in a range
        class Range : public Match1
        {
        public:
            static const uint32_t UUID = Y_FOURCC('R','N','G','E');  //!< 0xRNGE
            const uint8_t         lower;                             //!<  lower code
            const uint8_t         upper;                             //!<  lower code

            //! intialize
            inline explicit Range(const uint8_t lo, const uint8_t up) throw() : Match1(UUID), lower(lo), upper(up)
            {
                Y_LANG_PATTERN_IS(Range);
                if(upper<lower) cswap(lower,upper);
            }
            //! destructor
            inline virtual ~Range() throw() {}

            inline virtual Pattern *clone() const { return new Range(lower,upper); } //!< clone
            virtual void            __viz( ios::ostream &fp ) const;                 //!< GraphViz
            virtual void            write( ios::ostream &fp ) const;                 //!< output

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Range);
            inline virtual bool accept_byte( const uint8_t c) const throw()
            {
                return (c>=lower) && (c<=upper);
            }
        };


    }
}

#endif

