//! \file

#ifndef Y_GFX_PIXMAP_INCLUDED
#define Y_GFX_PIXMAP_INCLUDED 1

#include "y/gfx/bitmap.hpp"
#include "y/gfx/pixrow.hpp"
#include "y/gfx/broker.hpp"
#include <typeinfo>

namespace upsylon
{
    namespace graphic
    {

        namespace crux
        {
            //! common stuff for pixmaps
            struct pixmap
            {
                //! throw exception
                static void invalid_depth(const unit_t depth, const std::type_info &);
                
            };
        }

        //______________________________________________________________________
        //
        //
        //! generic pixmap, using bitmap as memory layout
        //
        //______________________________________________________________________
        template <typename T>
        class pixmap : public bitmap
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);     //!< aliase
            typedef pixrow<T> row;   //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            inline explicit pixmap(const unit_t W, const unit_t H) :
            bitmap(W,H,sizeof(T)), rows( mine() )
            {
            }

            //! cleanup
            inline virtual ~pixmap() throw() { rows = 0; }

            //! shared copy
            inline pixmap(const pixmap &pxm) throw() :
            bitmap(pxm), rows( mine() )
            {
                
            }

            //! from bitmap
            inline pixmap(const bitmap &bmp):
            bitmap(bmp), rows( mine() )
            {
                if( depth != sizeof(T) ) crux::pixmap::invalid_depth(depth, typeid(T) );
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________


            //! raw access
            inline row & operator()(const unit_t j) throw()
            {
                assert(j>=0);assert(j<h);
                return rows[j];
            }

            //! raw access, const
            inline const row & operator()(const unit_t j) const throw()
            {
                assert(j>=0);assert(j<h);
                return rows[j];
            }

            //! raw access by coordinate
            inline type & operator()(const coord p) throw()
            {
                pixmap<T> &self = *this; return self(p.y)(p.x);
            }

            //! raw accces by coordinate, const
            inline const type & operator()(const coord p) const throw()
            {
                const pixmap<T> &self = *this; return self(p.y)(p.x);
            }

            //! zero-flux access
            inline row & operator[](const unit_t j) throw()
            {
                return rows[zfh(j)];
            }

            //! zero-flux access, const
            inline const row & operator[](const unit_t j) const throw()
            {
                return rows[zfh(j)];
            }

            //! zero-flux access by coordinate
            inline type & operator[](const coord p) throw()
            {
                pixmap<T> &self = *this; return self[p.y][p.x];
            }

            //! zero-flux access by coordinate, const
            inline const_type & operator[](const coord p) const throw()
            {
                const pixmap<T> &self = *this; return self[p.y][p.x];
            }


            //__________________________________________________________________
            //
            // operations
            //__________________________________________________________________
            //! hard-copy with conversion
            template <typename U, typename FUNC>
            explicit pixmap( const pixmap<U> &source, broker &apply, FUNC &conv ) :
            bitmap(source.w,source.h,sizeof(T)),rows( mine() )
            {
                assign(source,apply,conv);
            }

            //! assign with conversion
            template <typename U, typename FUNC>
            inline void assign(const pixmap<U> &source, broker &apply, FUNC &conv) throw()
            {
                assert(source.has_same_metrics_than(*this));
                struct ops {
                    const pixmap<U> &source;
                    pixmap<T>       &target;
                    FUNC            &conv;
                    static inline void run(const tile &t,
                                           void       *args,
                                           lockable   &) throw()
                    {
                        ops  &self = *static_cast<ops *>(args);
                        FUNC &conv = self.conv;
                        for(size_t j=t.size();j>0;--j)
                        {
                            const segment   &s = t[j];
                            const unit_t     y = s.y;
                            const pixrow<U> &src = self.source[y];
                            pixrow<T>       &tgt = self.target[y];
                            const unit_t     x0 = s.xmin;
                            for(unit_t x=s.xmax;x>=x0;--x)
                            {
                                tgt(x) = conv(src(x));
                            }
                        }
                    }
                };

                ops todo = { source, *this, conv };
                apply(ops::run,&todo);
            }

            //! assign same type
            inline void assign(const pixmap<T> &source, broker &apply ) throw()
            {
                struct id { static inline const T &_(const T &__) throw() { return __; } };
                assign(source,apply,id::_);
            }
            
            //! count how many pixels fullfill the property
            template <typename PPTY> inline
            size_t how_many(broker &apply, PPTY &property) const
            {
                // prepare local memory
                assert(has_same_metrics_than(apply));
                apply.caches.make<size_t>();
                
                // parallel run
                struct ops
                {
                    const pixmap<T> &source;
                    PPTY            &property;

                    static inline void run(const tile &t,
                                           void       *args,
                                           lockable   &) throw()
                    {
                        ops             &self     = *static_cast<ops *>(args);
                        PPTY            &property = self.property;
                        const pixmap<T> &source   = self.source;
                        size_t  lres = 0;
                        for(size_t j=t.size();j>0;--j)
                        {
                            const segment   &s = t[j];
                            const unit_t     y = s.y;
                            const pixrow<T> &src  =  source[y];
                            const unit_t     xmin = s.xmin;
                            for(unit_t x=s.xmax;x>=xmin;--x)
                            {
                                if(property(src(x))) ++lres;
                            }
                        }
                        t.cache->as<size_t>() = lres;
                    }
                };

                ops todo = { *this, property };
                apply(ops::run,&todo);

                // reduction
                return apply.caches.sum<size_t>();
            }



        private:
            Y_DISABLE_ASSIGN(pixmap);
            row *rows;
            inline row * mine() throw() { return static_cast<row *>(a_rows->impl.block_addr); }

        };
    }

}

#endif

