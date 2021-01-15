
#include "y/mkl/fitting/built-in/conics.hpp"
#include "y/mkl/kernel/diagonalize.hpp"
#include "y/mkl/kernel/eigen.hpp"

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
                Wd(nvar,nvar),
                Q(2,2),
                R(2,2),
                L(2,0),
                J(2,0),
                lam(2,0),
                rhs(0)
                {}

                void __conics:: ellipse()
                {
                    ellipse_primary();
                    ellipse_replica();
                }
                
                void  __conics:: ellipse_primary() throw()
                {
                    C.ld(0);
                    C[1][3] =  2;
                    C[3][1] =  2;
                    C[2][2] = -1;
                }

                
                __conics:: ~__conics() throw() {}

                double __conics:: compute_UCU(const accessible<double> &u) throw()
                {
                    tao::mul(wi, C, u);
                    return tao::dot<double>::of(wi,u);
                }


                bool __conics:: find_values( )
                {
                    //----------------------------------------------------------
                    // diagonalize
                    //----------------------------------------------------------
                    size_t         nr = W.rows;
                    Wd.assign(W);
                    if(!diagonalize::eig(Wd,wr,wi,nr))
                    {
                        return false;
                    }


                    if(nr<=0)
                    {
                        // no real eigenvalue
                        return false;
                    }

                    //----------------------------------------------------------
                    // compute real eigenvectors
                    //----------------------------------------------------------
                    matrix<double> ev(nr,6);
                    diagonalize::eigv(ev,W,wr);
                    

                    //----------------------------------------------------------
                    // find optimal eigenvalue
                    //----------------------------------------------------------
                    for(size_t k=nr;k>0;--k)
                    {
                        const double mu = wr[k];
                        if(mu<=0)
                        {
                            // incompatible
                            break;
                        }
                        const array<double> &U   = ev[k];
                        const double         UCU = compute_UCU(U);
                        if(UCU<=0)
                        {
                            // incompatible
                            continue;
                        }

                        //------------------------------------------------------
                        // success!
                        //------------------------------------------------------
                        const accessible<double> &A = wi;
                        tao::divset(wi,sqrt(UCU),U);

                        std::cerr << "A=" << A << std::endl;

                        // build quadratic form
                        Q[1][1] = A[1];
                        Q[1][2] = Q[2][1] = A[2]/2;
                        Q[2][2] = A[3];
                        R.assign(Q); // to keep Q after LU

                        // build linear form
                        L[1]    = J[1] = A[4];
                        L[2]    = J[2] = A[5];

                        std::cerr << "Q=" << Q << std::endl;
                        std::cerr << "L=" << L << std::endl;

                        if(!LU::build(R))
                        {
                            // singular quadratic form
                            return false;
                        }

                        // find center
                        LU::solve(R,J);
                        tao::mulset(J,-0.5);
                        std::cerr << "J=" << J << std::endl;

                        // find eigenvalue
                        if(!eigen::build(Q,lam,R))
                        {
                            return false;
                        }
                        std::cerr << "lam=" << lam << std::endl;
                        std::cerr << "R="   << R << std::endl;

                        // find rhs by change of coordinates
                        rhs = - (0.5 * tao::dot<double>::of(L,J) + A[6]);
                        std::cerr << "rhs=" << rhs << std::endl;

                        double rx2 = rhs/lam[1];
                        double ry2 = rhs/lam[2];
                        std::cerr << "rx2=" << rx2 << " => " << sqrt(fabs(rx2)) << std::endl;
                        std::cerr << "ry2=" << ry2 << " => " << sqrt(fabs(ry2)) << std::endl;

                        return true;
                    }



                    return false;
                }

                bool __conics:: ellipse(vertex         &center,
                                        vertex         &radius,
                                        matrix<double> &rotate)
                {
                    bzset(center);
                    bzset(radius);
                    rotate.ld(0);
                    
                    if( build_shape() && find_values() )
                    {
                        center.x = J[1];
                        center.y = J[2];
                        
                        radius.x = sqrt(fabs(rhs/lam[1]));
                        radius.y = sqrt(fabs(rhs/lam[2]));

                        rotate.make(2,2);
                        rotate.assign(R);
                        
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }

            }
            
        }
    }
}


