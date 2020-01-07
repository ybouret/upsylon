
//! \file

#ifndef Y_GRAPHIC_STACK_INCLUDED
#define Y_GRAPHIC_STACK_INCLUDED 1

#include "y/graphic/image/tiff.hpp"
#include "y/sequence/slots.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace Graphic {

        //! check valid file name
        void CheckTIFF( const string &fileName );

        //! a stack of pixmaps
        template <typename T>
        class Stack : public Area, public slots< Pixmap<T> >
        {
        public:
            typedef arc_ptr<Stack>     Pointer; //!< alias
            typedef slots< Pixmap<T> > Slots; //!< alias

            //! setup
            inline explicit Stack(const size_t W,
                                  const size_t H,
                                  const size_t n) :
            Area(W,H),
            Slots(n)
            {
                for(size_t i=0;i<n;++i)
                {
                    this->template build<size_t,size_t>(w,h);
                }
            }

            //! cleanup
            inline virtual ~Stack() throw()
            {
            }

            //! average on [ini..end-1]
            template <typename U, const size_t DIMS>
            inline void average_(Pixmap<T>   &target,
                                 const size_t ini,
                                 const size_t end) const
            {
                if(end>ini)
                {
                    const Slots &self  = *this;
                    const size_t count = end-ini;
                    float        sum[DIMS] = { 0 };
                    const Point  lo = target->lower;
                    const Point  up = target->upper;
                    for(unit_t y=up.y;y>=lo.y;--y)
                    {
                        typename Pixmap<T>::RowType &r = target[y];

                        for(unit_t x=up.x;x>=lo.x;--x)
                        {
                            Y_BZSET_STATIC(sum);
                            for(size_t i=ini;i<end;++i)
                            {
                                const T &src = self[i][y][x];
                                const U *p   = (const U *) &src;
                                for(size_t dim=0;dim<DIMS;++dim)
                                {
                                    sum[dim] += float( p[dim] );
                                }
                            }
                            T &tgt = r[x];
                            U *q   = (U *)&tgt;
                            for(size_t dim=0;dim<DIMS;++dim)
                            {
                                q[dim] = Convert::FloatTo<U>( sum[dim] / count );
                            }
                        }

                    }
                }
            }

            //! type specific average
            void average(Pixmap<T> &target, const size_t ini, const size_t end) const;

            //! full average
            inline void average(Pixmap<T> &target)
            {
                average( target, 0, this->size() );
            }

            //! save to TIFF
            inline void saveTIFF(const string         &fileName,
                                 const size_t          ini,
                                 const size_t          end)
            {
                CheckTIFF(fileName);
                const Slots  &self = *this;
                GetRGBA<T>    proc;
                _TIFF::Raster raster;
                bool          append = false;
                for(size_t i=ini;i<end;++i)
                {
                    O_TIFF        tiff(fileName,append);
                    TIFF_Format::Compile(raster,*self[i], proc);
                    tiff.WriteRGBAImage(raster, w, h,i);
                    append = true;
                 }
            }
            
            static Stack *LoadTIFF(const string &filename, const size_t maxDirectories=0)
            {
                size_t numDirectories = I_TIFF::CountDirectoriesOf(filename);
                if(maxDirectories>0)
                {
                    numDirectories = min_of(maxDirectories,numDirectories);
                }
                I_TIFF tiff(filename);
                _TIFF::Raster   raster;
                const unit_t    w    = tiff.GetWidth();
                const unit_t    h    = tiff.GetHeight();
                auto_ptr<Stack> s    = new Stack(w,h,numDirectories);
                Slots          &self = *s;
                PutRGBA<T>      proc;
                
                for(size_t i=0;i<numDirectories;++i)
                {
                    tiff.SetDirectory(i);
                    tiff.ReadRBGAImage(raster);
                    TIFF_Format::Expand(*self[i],raster,proc);
                }
                
                return s.yield();
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Stack);
        };

    }

}

#endif

