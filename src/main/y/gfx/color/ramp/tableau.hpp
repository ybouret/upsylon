

//! \file

#ifndef Y_GFX_COLOR_RAMP_TABLEAU_INCLUDED
#define Y_GFX_COLOR_RAMP_TABLEAU_INCLUDED 1

#include "y/gfx/color/ramp.hpp"

namespace upsylon
{

    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! fixed dimension tableau of colors
        //
        //______________________________________________________________________
        class tableau : public color_ramp
        {
        public:
            virtual ~tableau() throw();

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual size_t      dimensions() const throw(); //!< registered colors
            virtual const rgba *repository() const throw(); //!< first color address


        protected:
            //! todo
            explicit tableau(const rgba  * const, const size_t) throw();

        private:
            const rgba  * const repository_;
            const size_t        dimensions_;
            Y_DISABLE_COPY_AND_ASSIGN(tableau);
        };

        //! todo
        template <typename CLASS>
        class tableau_of: public tableau
        {
        public:
            //! todo
            explicit tableau_of(const CLASS &host) throw() :
            tableau( host.repo, host.dims )
            {
            }

            //! todo
            explicit tableau_of( ) throw() :
            tableau( CLASS::repo, CLASS::dims )
            {
            }

            //! todo
            virtual ~tableau_of() throw()
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(tableau_of);
        };

        //! todo
        class monochrome
        {
        public:
            //! todo
            monochrome(const rgba &c) throw() :
            repo(),
            dims(2)
            {
                repo[1] = c;
            }

            //! todo
            ~monochrome() throw()
            {
            }

            rgba         repo[2]; //!< todo
            const size_t dims;    //!< todo
        };

        //! todo
        struct cold_to_hot
        {
            static const size_t dims = 5;   //!< todo
            static const rgba   repo[dims]; //!< todo
        };

    }

}

#endif

