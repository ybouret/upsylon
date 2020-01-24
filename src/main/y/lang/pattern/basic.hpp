//! \file
#ifndef Y_LANG_PATTERN_BASIC_INCLUDED
#define Y_LANG_PATTERN_BASIC_INCLUDED 1

#include "y/lang/pattern.hpp"

namespace upsylon {

    namespace Lang {

        //----------------------------------------------------------------------
        //
        //! Matching One char interface
        //
        //----------------------------------------------------------------------
        class Match1 : public Pattern
        {
        public:
            virtual     ~Match1() throw();               //!< cleanup
            virtual bool match(Token &, Source &) const; //!< common match function
            virtual bool weak() const throw();           //!< false, always one char is returned

        protected:
            explicit Match1(const uint32_t id) throw();  //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Match1);
            virtual bool accept_byte( const uint8_t c ) const throw() = 0;
        };

        //----------------------------------------------------------------------
        //
        //! Matching Any One char
        //
        //----------------------------------------------------------------------
        class Any1 : public Match1
        {
        public:
            static const uint32_t UUID = Y_FOURCC('A', 'N', 'Y', '1'); //!< 0xANY1
            static const char     CLID[8];                             //!< UUID

            virtual       ~Any1() throw();                  //!< destructor
            explicit       Any1() throw();                  //!< constructor
            virtual Any1 * clone() const;                   //!< clone
            virtual void   __viz(ios::ostream &) const;     //!< GraphViz
            virtual void   write(ios::ostream &) const;     //!< output [0xANY1] (+4)
            virtual bool   univocal() const throw();        //!< false

            virtual const char *className() const throw();      //!< CLID
            virtual size_t      serialize(ios::ostream&) const; //!< [0xANY1] (+4)

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Any1);
            virtual bool accept_byte( const uint8_t ) const throw();
        };

        //----------------------------------------------------------------------
        //
        //! Matching a Single char
        //
        //----------------------------------------------------------------------
        class Single : public Match1
        {
        public:
            static const uint32_t UUID = Y_FOURCC('S', 'N', 'G', 'L'); //!< 0xSNGL
            static const char     CLID[8];                             //!< UUID
            const uint8_t         code;                                //!< the matching code

            explicit         Single(const uint8_t c) throw(); //!< setup
            virtual         ~Single() throw();                //!< destructor
            virtual Single * clone() const;                   //!< clone
            virtual void     __viz(ios::ostream &) const;     //!< GraphViz
            virtual void     write(ios::ostream &) const;     //!< output [0xSNGL] [code] (+5)
            virtual bool     univocal() const throw();        //!< true

            virtual size_t      serialize(ios::ostream&) const; //!< [0xSNGL] [code] (+5)
            virtual const char *className() const throw();      //!< CLID

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Single);
            virtual bool accept_byte( const uint8_t c) const throw();
        };

        //----------------------------------------------------------------------
        //
        //! matching a char in a range
        //
        //----------------------------------------------------------------------
        class Range : public Match1
        {
        public:
            static const uint32_t UUID = Y_FOURCC('R','N','G','E');  //!< 0xRNGE
            static const char     CLID[8];                           //!< UUID
            const uint8_t         lower;                             //!< lower code
            const uint8_t         upper;                             //!< lower code

            explicit       Range(const uint8_t, const uint8_t) throw(); //!< intialize
            virtual       ~Range() throw();                             //!< destructor
            virtual Range *clone() const;                               //!< clone
            virtual void   __viz(ios::ostream &) const;                 //!< GraphViz
            virtual void   write(ios::ostream &) const;                 //!< output [0xRNGE] [lower] [upper] (+6)
            virtual bool   univocal() const throw();                    //!< true is lower==upper

            virtual size_t      serialize(ios::ostream&) const; //!< output [0xRNGE] [lower] [upper] (+6)
            virtual const char *className() const throw();      //!< CLID

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Range);
            virtual bool accept_byte( const uint8_t c) const throw();
        };


    }
}

#endif

