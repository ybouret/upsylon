//! \file
#ifndef Y_IPSO_DCT_INCLUDED
#define Y_IPSO_DCT_INCLUDED 1

#include "y/ipso/field2d.hpp"

namespace upsylon
{
    namespace ipso
    {
        class DCT
        {
        public:
            typedef field2D<double> Table;

            virtual ~DCT() throw();
            const unit_t w;
            const unit_t h;
        protected:
            explicit DCT( const unit_t W, const unit_t H);
            Table LAMBDA;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DCT);
        };

        class CommonDCT : public DCT
        {
        public:
            virtual ~CommonDCT() throw();
            explicit CommonDCT(const unit_t W,const unit_t H);

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

            void reverse(Table &pix, const Table &dct) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(CommonDCT);
            Table XCOS;
            Table YCOS;
        };

        class SquareDCT : public DCT
        {
        public:
            virtual ~SquareDCT() throw();
            explicit SquareDCT(const unit_t W);

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

            void reverse(Table &pix, const Table &dct) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(SquareDCT);
            Table COS;
        };


    }
}

#endif

