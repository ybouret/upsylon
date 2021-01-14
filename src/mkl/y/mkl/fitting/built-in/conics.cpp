
#include "y/mkl/fitting/built-in/conics.hpp"
#include "y/mkl/kernel/diagonalize.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {
                __conics::  __conics() :
                W(nvar,nvar),
                C(nvar,nvar),
                wr(nvar,0),
                wi(nvar,0),
                sp(nvar,0)
                {}

                __conics:: ~__conics() throw() {}

                double __conics:: compute_UCU(const accessible<double> &u) throw()
                {
                    tao::mul(wi, C, u);
                    return tao::dot<double>::of(wi,u);
                }


                bool __conics:: find( )  
                {
                    size_t         nr = W.rows;
                    sp.free();
                    matrix<double> Wd(W);
                    if(!diagonalize::eig(Wd,wr,wi,nr))
                    {
                        return false;
                    }

                    if(nr<=0)
                    {
                        return false;
                    }

                    std::cerr << "#nr=" << nr << std::endl;
                    std::cerr << "wr=" << wr << std::endl;
                    std::cerr << "wi=" << wi << std::endl;

                    matrix<double> ev(nr,6);

                    diagonalize::eigv(ev, W, wr);
                    std::cerr << "ev=" << ev << std::endl;

                    for(size_t k=nr;k>0;--k)
                    {
                        const double mu = wr[k];
                        if(mu<=0) break;
                        const array<double> &U   = ev[k];
                        const double         UCU = compute_UCU(U);

                        std::cerr << "trying mu  = " << mu << std::endl;
                        std::cerr << "       U   = " << U << std::endl;
                        std::cerr << "       UCU = " << UCU << std::endl;
                        if(UCU<=0) continue;

                        const double den = sqrt(UCU);
                        tao::divset(wi,den,U);
                        std::cerr << "A=" << wi << std::endl;
                    }



                    return false;
                }



            }

            namespace built_in
            {
                iConics:: iConics() : conics_type()
                {
                }

                iConics:: ~iConics() throw()
                {
                }

                void iConics:: assemble()
                {
                    size_t n = x.size();
                    // Z = [x^2 xy y^2 x y 1]
                    S[6][6] = n;
                    for( list<apq>::iterator i=x.begin(),j=y.begin();n>0;--n,++i,++j)
                    {
                        const apq &X = *i;
                        const apq &Y = *j;
                        const apq  X2 = square_of(X);
                        const apq  Y2 = square_of(Y);
                        const apq  XY = X*Y;
                        S[1][1] += square_of(X2);
                        S[1][2] += X2*XY;
                        S[1][3] += X2*Y2;
                        S[1][4] += X2*X;
                        S[1][5] += X2*Y;
                        S[1][6] += X2;

                        S[2][2] += square_of(XY);
                        S[2][3] += XY * Y2;
                        S[2][4] += XY * X;
                        S[2][5] += XY * Y;
                        S[2][6] += XY;

                        S[3][3] += square_of(Y2);
                        S[3][4] += Y2 * X;
                        S[3][5] += Y2 * Y;
                        S[3][6] += Y2;

                        S[4][4] += square_of(X);
                        S[4][5] += X*Y;
                        S[4][6] += X;

                        S[5][5] += square_of(Y);
                        S[5][6] += Y;
                    }

                }

                void iConics:: transfer()
                {

                    for(size_t i=1;i<=nvar;++i)
                    {
                        for(size_t j=1;j<=nvar;++j)
                        {
                            W[i][j] = _W[i][j].to_double();
                            C[i][j] = _C[i][j].to_double();
                        }
                    }
                    std::cerr << "W=" << W << std::endl;
                }

                void iConics:: add(unit_t X, unit_t Y)
                {
                    assert(x.size()==y.size());

                    const apq qx = X;
                    const apq qy = Y;
                    x.push_back(qx);
                    try
                    {
                        y.push_back(qy);
                    }
                    catch(...)
                    {
                        x.pop_back();
                        throw;
                    }
                }


            }

            namespace built_in
            {
                dConics:: dConics() : conics_type()
                {
                }

                dConics:: ~dConics() throw()
                {
                }

                void dConics:: assemble()
                {
                    
                }

                void dConics:: transfer()
                {

                }
            }


        }
    }
}


