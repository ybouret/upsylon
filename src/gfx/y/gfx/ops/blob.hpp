
//! \file

#ifndef Y_GFX_OPS_BLOB_INCLUDED
#define Y_GFX_OPS_BLOB_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/core/knode.hpp"
#include "y/core/dnode.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        // types and definition
        //
        //______________________________________________________________________
        typedef core::knode<coord>    knot;   //!< dynamic coordinate
        typedef knot::list_type       knots_; //!< base class for list of knots


        //______________________________________________________________________
        //
        //
        //! list of knots
        //
        //______________________________________________________________________
        class knots : public entity, public knots_
        {
        public:
            virtual ~knots() throw();        //!< cleanup
            explicit knots() throw();        //!< setup
            void     reserve(size_t n);      //!< push_back n nodes
            void     ensure(const size_t n); //!< ensure n nodes

        private:
            Y_DISABLE_COPY_AND_ASSIGN(knots);
        };

        //______________________________________________________________________
        //
        //
        //! shared list of knots
        //
        //______________________________________________________________________
        typedef arc_ptr<knots> shared_knots;

        //______________________________________________________________________
        //
        //
        //! a blob is a list of nodes, itself a node for blobs
        //
        //______________________________________________________________________
        class blob : public knots, public dnode<blob>
        {
        public:

            //! prototype to validate a newlyformed blob
            typedef bool (*proc)(const blob &, void *);

            explicit blob(const size_t, const shared_knots &) throw(); //!< setup
            virtual ~blob() throw();                                   //!< cleanup

            const size_t label; //!< identifier
            shared_knots cache; //!< pool to return knots to

            //! change label, propagating
            blob  *change_label(const size_t    value,
                                pixmap<size_t> &masks) throw();

            //! change label to 0, dispatch
            blob  *remove_from(pixmap<size_t> &masks) throw();

            //! set value on knots
            template <typename T> inline
            void dispatch(typename type_traits<T>::parameter_type args, pixmap<T> &pxm ) const
            {
                for(const knot *node=head;node;node=node->next)
                {
                    pxm(**node) = args;
                }
            }

            //! compare blobs by increasing size
            static int increasing_size(const blob *, const blob *, void*) throw();

            //! comapre blobs by decreasing size
            static int decreasing_size(const blob *, const blob *, void*) throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(blob);
        };

    }
    
}

#endif
