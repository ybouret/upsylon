
//! \file

#ifndef Y_GFX_IMAGE_FORMAT_OPTION_INCLUDED
#define Y_GFX_IMAGE_FORMAT_OPTION_INCLUDED 1

#include "y/string.hpp"
#include "y/gfx/entity.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace image
        {

            //__________________________________________________________________
            //
            //! name=value pair for format options
            //__________________________________________________________________
            class option : public entity
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef intr_ptr<string,option> pointer; //!< alias


                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup
                template <typename NAME, typename VALUE>
                inline explicit option(const NAME &n, const VALUE &v) :
                name(n), value(v)
                {
                }

                //! cleanup
                virtual ~option() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const  string       &key() const throw();                        //!< for pointer
                friend std::ostream & operator<<(std::ostream &, const option&); //!< display

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const string name;  //!< option name
                string       value; //!< option value



            private:
                Y_DISABLE_COPY_AND_ASSIGN(option);
            };

        }

    }

}

#endif
