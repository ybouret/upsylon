//! \file

#ifndef Y_GFX_OPS_BLOBS_INCLUDED
#define Y_GFX_OPS_BLOBS_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/pixel.hpp"
#include "y/core/knode.hpp"
#include "y/core/inode.hpp"

namespace upsylon
{
    namespace graphic
    {

        typedef core::knode<coord>    knot;
        typedef knot::list_type       knots_;

        class knots : public entity, public knots_
        {
        public:
            virtual ~knots() throw();
            explicit knots() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(knots);
        };

        typedef arc_ptr<knots> shared_knots;

        class blob : public knots, public inode<blob>
        {
        public:


            explicit blob(const size_t        id,
                          const shared_knots &ks) throw();
            virtual ~blob() throw();


            const size_t label;
            shared_knots kpool;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(blob);
        };

        typedef core::list_of_cpp<blob> blobs_;

        typedef pixmap<size_t> marks;
        typedef arc_ptr<marks> shared_marks;

        class blobs :  public blobs_
        {
        public:
            explicit blobs(const shared_marks &M,
                           const shared_knots &K);
            virtual ~blobs() throw();




            void initialize(size_t num_knots);


            shared_marks probe;
            shared_knots kpool;

            knot *fetch_knot();

            template <typename T>
            void build(const pixmap<T> &field,
                       const size_t     n)
            {
                assert(probe->has_same_metrics_than(field));
                size_t label = 0;
                marks &_mark = *probe;
                knots  stack;

                const unit_t w = _mark.w;
                const unit_t h = _mark.h;

                for(unit_t y=0;y<h;++y)
                {
                    const pixrow<T> &f_y  = field(y);
                    pixrow<size_t>  &m_y  = _mark(y);

                    for(unit_t x=0;x<w;++x)
                    {
                        if( pixel::is_zero(f_y(x)) ) continue; // no data
                        size_t &m = m_y(x); if(m>0)  continue; // already visited


                        blob *b = push_back( new blob(++label,kpool) );
                        assert(0==stack.size);
                        {
                            // initialize stack
                            m = label;
                            **stack.push_back( fetch_knot() )  = coord(x,y);

                            while(stack.size)
                            {
                                // pop front
                                const coord  curr = **(b->push_back( stack.pop_front() ));
                                for(size_t i=0;i<n;++i)
                                {
                                    // check if can be added
                                    const coord link = curr + area::delta[i];
                                    if( !_mark.owns(link)  ) { continue; }                // another region
                                    size_t &msub = _mark(link);
                                    if( msub > 0 )
                                    {
                                        assert(label==msub);
                                        continue;

                                    } // already visited

                                    if( pixel::is_zero( field(link) ) )
                                    {
                                        continue;

                                    } // a zero pixel

                                    // add
                                    msub = label;
                                    **stack.push_back( fetch_knot() )  = link;
                                }
                            }

                        }
                    }

                }

            }


#if 0
            template <typename T>
            void build(const tile      &t,
                       const pixmap<T> &field,
                       const size_t     n)
            {
                std::cerr << "in " << t << std::endl;
                assert(0==size);
                assert(4==n||8==n);
                size_t label = t.shift;
                marks &_mark = *probe;
                knots  stack;
                
                for(size_t j=t.size();j>0;--j)
                {
                    const segment   &s    = t[j];
                    const unit_t     y    = s.y;
                    const pixrow<T> &f_y  = field(y);
                    pixrow<size_t>  &m_y  = _mark(y);
                    const unit_t     xmin = s.xmin;

                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        if( pixel::is_zero(f_y(x)) ) continue; // no data
                        size_t &m = m_y(x); if(m>0)  continue; // already visited
                        
                        
                        blob *b = push_back( new blob(++label,kpool) );
                        assert(0==stack.size);
                        {
                            // initialize stack
                            m = label;
                            **stack.push_back( cache.pop_front() )  = coord(x,y);

                            while(stack.size)
                            {
                                // pop front
                                const coord  curr = **(b->push_back( stack.pop_front() ));
                                for(size_t i=0;i<n;++i)
                                {
                                    // check if can be added
                                    const coord link = curr + area::delta[i];
                                    if( !t.owns(link)  ) { continue; }                // another region
                                    size_t &msub = _mark(link);
                                    if( msub > 0 )
                                    {
                                        assert(label==msub);
                                        continue;

                                    } // already visited

                                    if( pixel::is_zero( field(link) ) )
                                    {
                                        continue;

                                    } // a zero pixel

                                    // add
                                    msub = label;
                                    **stack.push_back( cache.pop_front() )  = link;
                                }
                            }

                        }
                        //return;

                    }
                }
            }
#endif
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(blobs);

        };


    }


}

#endif

