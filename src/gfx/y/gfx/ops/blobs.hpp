//! \file

#ifndef Y_GFX_OPS_BLOBS_INCLUDED
#define Y_GFX_OPS_BLOBS_INCLUDED 1

#include "y/gfx/ops/blob.hpp"
#include "y/gfx/pixel.hpp"


namespace upsylon
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! base class for blobs
        //
        //______________________________________________________________________
        typedef core::list_of_cpp<blob> blob_list;

        //______________________________________________________________________
        //
        //
        //! blobs: up-to-date list and map of contiguous regions
        //
        //______________________________________________________________________
        class blobs
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~blobs() throw(); //!< cleanup
            explicit blobs(const unit_t W, const unit_t H, const shared_knots &K); //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     clear() throw(); //!< clear all
            void     sort();          //!< decreasing/relabel

            //! generic build algorithm
            template <typename T> inline
            void build(const pixmap<T> &field,
                       const size_t     conn = 8,
                       blob::proc       proc = NULL,
                       void            *args = NULL)
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                assert(bmask.has_same_metrics_than(field));
                assert(conn==4||conn==8);
                clear();

                size_t       label = 0;       // initial label
                const unit_t w     = bmask.w; // full width
                const unit_t h     = bmask.h; // full height
                knots        stack;           // local search stack

                for(unit_t y=0;y<h;++y)
                {
                    const pixrow<T> &f_y  = field(y);
                    pixrow<size_t>  &m_y  = aliasing::_(bmask(y));

                    for(unit_t x=0;x<w;++x)
                    {
                        //------------------------------------------------------
                        //
                        // ENTER: inner loop
                        //
                        //------------------------------------------------------

                        if( pixel::is_zero(f_y(x)) ) continue;            // no data
                        size_t &m = m_y(x); if(m>0)  continue;            // already visited
                        blob   *b = produce(label);
                        {
                            //--------------------------------------------------
                            // initialize stack
                            //--------------------------------------------------
                            assert(0==stack.size);
                            m = label;
                            **stack.push_back( fetch_knot() ) = coord(x,y);

                            while(stack.size)
                            {
                                //----------------------------------------------
                                // register front node, take its coordinates
                                //----------------------------------------------
                                const coord  curr = **(b->push_back( stack.pop_front() ));
                                for(size_t i=0;i<conn;++i)
                                {
                                    //------------------------------------------
                                    // check if can be added
                                    //------------------------------------------
                                    const coord link = curr + area::delta[i];     // where to look at
                                    if( !bmask.owns(link)  ) continue;            // does not belong  -> continue
                                    size_t &l = aliasing::_(bmask(link));         // check label
                                    if( l > 0 ) { assert(label==l); continue; }   // already visited  -> continue
                                    if( pixel::is_zero( field(link) ) ) continue; // a zero pixel     -> continue

                                    //------------------------------------------
                                    // added to stack
                                    //------------------------------------------
                                    l = label;
                                    **stack.push_back( fetch_knot() )  = link;
                                }
                            }

                        }

                        //------------------------------------------------------
                        // a new blob was created!
                        //------------------------------------------------------
                        if( proc && ! (*proc)(*b,args) )
                        {
                            // clean zone
                            expunge(b);
                            --label;
                        }

                        //------------------------------------------------------
                        //
                        // LEAVE: inner loop
                        //
                        //------------------------------------------------------
                    }

                }
            }

            void     remove_if(blob::proc, void *)      throw(); //!< remove according to proc
            void     remove_below(const size_t cutSize) throw(); //!< remove if blob size<=cutSize

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const blob_list blist; //!< current list
            const blob_map  bmask; //!< current mask
            shared_knots    cache; //!< shared cache for knots I/O

        private:
            Y_DISABLE_ASSIGN(blobs);
            knot *fetch_knot();            //!< a new (cached) knot
            void  rewrite() throw();       //!< dispatch each label
            void  relabel() throw();       //!< label from 1 to blist.size, rewrite
            void  expunge(blob *) throw(); //!< expunge blob from list and mask, delete it
            blob *produce(size_t&);        //!< produce new blob with update label
        };

        
    }


}

#endif

