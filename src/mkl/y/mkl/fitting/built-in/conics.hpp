
//! \file

#ifndef Y_MKL_FITTING_CONICS_INCLUDED
#define Y_MKL_FITTING_CONICS_INCLUDED 1

#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/yap/rational.hpp"
#include "y/mkl/fitting/v-gradient.hpp"
#include "y/mkl/fitting/sequential.hpp"
#include "y/mkl/fitting/sample.hpp"
#include "y/mkl/fitting/least-squares.hpp"
#include "y/type/point2d.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {

                //______________________________________________________________
                //
                //
                //! common parts for conics fitting
                //
                //______________________________________________________________

                class __conics
                {
                public:
                    static const size_t nvar = 6;

                    virtual ~__conics() throw();

                    virtual bool build_shape() = 0; //!< build and transfer W

                    bool find( );



                protected:
                    explicit __conics();

                    matrix<double> W;  //!< workspace   matrix
                    matrix<double> C;  //!< constraints matrix
                    vector<double> wr; //!< eigv re
                    vector<double> wi; //!< eigv im
                    vector<double> sp; //!< possible values

                    double compute_UCU(const accessible<double> &u) throw();

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(__conics);

                };


                template <typename T>
                class _conics  : public __conics
                {
                public:
                    typedef _conics<T> conics_type;
                    
                    inline virtual ~_conics() throw() {}


                    inline virtual bool build_shape()
                    {
                        assert(x.size()==y.size());
                        S.ld(zero);
                        assemble();
                        regularize();
                        std::cerr << "S=" << S << std::endl;
                        if( !LU::build(S) )
                        {
                            return false;
                        }
                        else
                        {
                            _W.assign(_C);
                            LU::solve(S,_W); // _W = inv(S)*C
                            transfer();
                            return true;
                        }
                    }

                    const T        zero; //!< a zero value
                    matrix<T>      _C;   //!< constraint matrix

                protected:
                    //! setup
                    inline explicit _conics() :
                    zero(0), _C(nvar,nvar), x(), y(), S(nvar,nvar), _W(nvar,nvar)
                    {
                    }

                    list<T>        x;    //!< store coordinate
                    list<T>        y;    //!< store coordinate
                    matrix<T>      S;    //!< shape matrix
                    matrix<T>     _W;    //!< inv(S)*C

                    //! make symmetric matrix
                    void regularize()
                    {
                        S[2][1] = S[1][2];

                        S[3][1] = S[1][3];
                        S[3][2] = S[2][3];

                        S[4][1] = S[1][4];
                        S[4][2] = S[2][4];
                        S[4][3] = S[3][4];

                        S[5][1] = S[1][5];
                        S[5][2] = S[2][5];
                        S[5][3] = S[3][5];
                        S[5][4] = S[4][5];


                        S[6][1] = S[1][6];
                        S[6][2] = S[2][6];
                        S[6][3] = S[3][6];
                        S[6][4] = S[4][6];
                        S[6][5] = S[5][6];
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(_conics);
                    virtual void assemble() = 0; //!< build S from x and y
                    virtual void transfer() = 0; //!< _W and _C to W and C
                };


                class iConics : public _conics<apq>
                {
                public:
                    explicit iConics();
                    virtual ~iConics() throw();

                    void add(unit_t X, unit_t Y);


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(iConics);
                    virtual void assemble();
                    virtual void transfer();
                };


                class dConics : public _conics<double>
                {
                public:
                    explicit dConics();
                    virtual ~dConics() throw();

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(dConics);
                    virtual void assemble();
                    virtual void transfer();
                };
            }

        }

    }

}

#endif
