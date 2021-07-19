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
        typedef core::list_of_cpp<blob> blobs_;
        
        //______________________________________________________________________
        //
        //
        //! blobs for a pixmap
        //
        //______________________________________________________________________
        class blobs : public blobs_
        {
        public:
            explicit blobs() throw(); //!< setup empty
            virtual ~blobs() throw(); //!< cleanup
            
            void     sort(pixmap<size_t> &masks);                                  //!< sort by decreasing order, relabel and rewrite
            void     remove_if(blob::proc, void *, pixmap<size_t> &)      throw(); //!< remove if, relabel and rewrite
            void     remove_below(const size_t cutSize, pixmap<size_t> &) throw(); //!< remove if blob.size <= cutSize, rlabel and rewrite

            //! generic build algorithm
            template <typename T> inline
            void build(pixmap<size_t>  &masks,
                       const pixmap<T> &field,
                       shared_knots    &cache,
                       const size_t     connectivity,
                       blob::proc       proc = NULL,
                       void            *args = NULL)
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                assert(masks.has_same_metrics_than(field));
                assert(connectivity==4||connectivity==8);
                masks.ldz();
                release();
                
                size_t       label = 0;       // initial label
                const unit_t w     = masks.w; // full width
                const unit_t h     = masks.h; // full height
                knots        stack;           // local search stack
                
               
                
                for(unit_t y=0;y<h;++y)
                {
                    const pixrow<T> &f_y  = field(y);
                    pixrow<size_t>  &m_y  = masks(y);
                    
                    for(unit_t x=0;x<w;++x)
                    {
                        //------------------------------------------------------
                        //
                        // ENTER: inner loop
                        //
                        //------------------------------------------------------
                        
                        if( pixel::is_zero(f_y(x)) ) continue;            // no data
                        size_t &m = m_y(x); if(m>0)  continue;            // already visited
                        blob   *b = push_back( new blob(++label,cache) ); // start new blob!
                        {
                            //--------------------------------------------------
                            // initialize stack
                            //--------------------------------------------------
                            assert(0==stack.size);
                            m = label;
                            **stack.push_back( fetch_knot(cache) )  = coord(x,y);
                            
                            while(stack.size)
                            {
                                //----------------------------------------------
                                // register front node, take its coordinates
                                //----------------------------------------------
                                const coord  curr = **(b->push_back( stack.pop_front() ));
                                for(size_t i=0;i<connectivity;++i)
                                {
                                    //------------------------------------------
                                    // check if can be added
                                    //------------------------------------------
                                    const coord link = curr + area::delta[i];     // where to look at
                                    if( !masks.owns(link)  ) continue;            // does not belong  -> continue
                                    size_t &l = masks(link);                      // check label
                                    if( l > 0 ) { assert(label==l); continue; }   // already visited  -> continue
                                    if( pixel::is_zero( field(link) ) ) continue; // a zero pixel     -> continue
                                   
                                    //------------------------------------------
                                    // added to stack
                                    //------------------------------------------
                                    l = label;
                                    **stack.push_back( fetch_knot(cache) )  = link;
                                }
                            }
                            
                        }
                        //------------------------------------------------------
                        // a new blob was created!
                        //------------------------------------------------------
                        if( proc && ! (*proc)(*b,args) )
                        {
                            // clean zone
                            delete unlink(b->remove_from(masks));
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
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(blobs);
            static knot *fetch_knot(shared_knots &cache);
            void         relabel() throw();
            void         rewrite(pixmap<size_t> &) const throw();
        };
        
 
    }


}

#endif

