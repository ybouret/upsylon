
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
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~knots() throw();        //!< cleanup
            explicit knots() throw();        //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
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
        //! map of blobs
        //
        //______________________________________________________________________
        typedef pixmap<size_t> blob_map;


        //______________________________________________________________________
        //
        //
        //! a blob is a list of nodes, itself a node for blobs
        //
        //______________________________________________________________________
        class blob : public knots, public dnode<blob>
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________

            //! prototype to validate a newlyformed blob
            typedef bool (*proc)(const blob &, void *);

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit blob(const size_t, const shared_knots &) throw(); //!< setup
            virtual ~blob() throw();                                   //!< cleanup


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! change label, propagating
            blob  *change_label(const size_t value, blob_map &bmask) throw();

            //! change label to 0, dispatch
            blob  *remove_from(blob_map &bmask) throw();

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

            //! compare blobs by decreasing size
            static int  decreasing_size(const blob *, const blob *, void*) throw();

            //! true if size <= *(size_t *)args
            static bool is_smaller_than(const blob &, void *args) throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t label; //!< identifier
            shared_knots cache; //!< pool to return knots to

        private:
            Y_DISABLE_COPY_AND_ASSIGN(blob);
        };

    }
    
}

#endif
