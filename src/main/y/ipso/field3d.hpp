//! \file
#ifndef Y_IPSO_FIELD3D_INCLUDED
#define Y_IPSO_FIELD3D_INCLUDED 1

#include "y/ipso/field2d.hpp"

namespace upsylon
{
    namespace ipso
    {

        //! wrapper for constructor
#define Y_IPSO_FIELD3D_CTOR() layout_type(L), field<T>(id), slices(0)

        //! field in 3D
        template <typename T>
        class field3D : public layout3D, public field<T>
        {
        public:
            Y_DECL_ARGS(T,type);            //!< aliases
            typedef layout3D   layout_type; //!< alias
            typedef field1D<T> row_type;    //!< alias
            typedef field2D<T> slice_type;  //!< alias

            //! setup
            inline explicit field3D( const string &id, const layout_type &L) :
            Y_IPSO_FIELD3D_CTOR() { setup(); }

            //!setup
            inline explicit field3D( const char *id, const layout_type &L) :
            Y_IPSO_FIELD3D_CTOR() { setup(); }

            //! desctructor
            inline virtual ~field3D() throw()
            {
            }

            //! number of held items
            virtual size_t size() const throw() { return items; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(field3D);
            slice_type *slices;

            void setup()
            {
                
            }
        };

    }

}

#endif

