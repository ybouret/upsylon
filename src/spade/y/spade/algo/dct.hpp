//! \file
#ifndef Y_SPADE_DCT_INCLUDED
#define Y_SPADE_DCT_INCLUDED 1

#include "y/spade/field/in2d.hpp"

namespace upsylon
{
    namespace Spade
    {
        //! Discret Cosine Transform base class
        class DCT
        {
        public:
            typedef Field2D<double> Table; //!< alias to store internal data
            typedef Layout2D::Loop  Loop;  //!< alias for loops
            
            virtual ~DCT() throw(); //!< cleanup

            const unit_t w; //!< width
            const unit_t h; //!< height

            //! reverse transform interface
            virtual void reverse(Table &pix, const Table &dct) throw() =0;

        protected:
            explicit DCT( const unit_t W, const unit_t H); //!< setup
            Table LAMBDA; //!< internal cosines

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DCT);
        };

        //! rectangular DCT
        class CommonDCT : public DCT
        {
        public:
            virtual ~CommonDCT() throw();                      //!< cleanup
            explicit CommonDCT(const unit_t W,const unit_t H); //!< setup

            //! forward transform
            /**
             \param tgt a Table(W,H)
             \param src a 2D table kind
             \param xx  x offset for src
             \param yy  y offset for src
             */
            template <typename TABLE>
            void forward(Table           &tgt,
                         const TABLE     &src,
                         const unit_t     xx,
                         const unit_t     yy) throw()
            {
                const unit_t    N     = w;
                const unit_t    M     = h;

                for(unit_t j=0;j<M;++j)
                {
                    for(unit_t i=0;i<N;++i)
                    {
                        double       q   = 0;
                        const double wij = LAMBDA[j][i];
                        for(unit_t y=0;y<M;++y)
                        {
                            const double Cyj    = YCOS[y][j];
                            for(unit_t x=0;x<N;++x)
                            {
                                const double Cxi = XCOS[x][i];
                                const double wxy = Cxi*Cyj;
                                q += double( src[yy+y][xx+x] ) * wxy;
                            }
                        }

                        tgt[j][i] = q * wij;
                    }
                }
            }

            //! reverse transform
            virtual void reverse(Table &pix, const Table &dct) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(CommonDCT);
            Table XCOS;
            Table YCOS;
        };

        //! optimized DCT for squared layout
        class SquareDCT : public DCT
        {
        public:
            virtual ~SquareDCT() throw();       //!< cleanup
            explicit SquareDCT(const unit_t W); //!< setup

            //! forward transform
            template <typename TABLE>
            void forward(Table         &tgt,
                         const TABLE   &src,
                         const unit_t   xx,
                         const unit_t   yy) throw()
            {
                const unit_t    N     = w;

                for(unit_t j=0;j<N;++j)
                {
                    for(unit_t i=0;i<N;++i)
                    {
                        double       q   = 0;
                        const double wij = LAMBDA[j][i];
                        for(unit_t y=0;y<N;++y)
                        {
                            const double Cyj    = COS[y][j];
                            for(unit_t x=0;x<N;++x)
                            {
                                const double Cxi = COS[x][i];
                                const double wxy = Cxi*Cyj;
                                q += double( src[yy+y][xx+x] ) * wxy;
                            }
                        }
                        tgt[j][i] = q * wij;
                    }
                }
            }

            //! reverse transform
            virtual void reverse(Table &pix, const Table &dct) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(SquareDCT);
            Table COS;
        };


    }
}

#endif

