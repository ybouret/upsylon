//! \file
#ifndef Y_MATH_FIT_VECTORS_INCLUDED
#define Y_MATH_FIT_VECTORS_INCLUDED 1

#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"
#include "y/math/fit/samples.hpp"

namespace upsylon {

    namespace math
    {

        namespace Fit
        {
            //! base class for Vectors
            class VectorsInfo : public counted_object
            {
            public:
                const string name;                  //!< identifier for database
                virtual ~VectorsInfo() throw();     //!< destructor
                const string & key() const throw(); //!< for database

                //! throw an exception upon multiple vectors name
                static void MultipleVectorsException(const string &);

            protected:
                explicit VectorsInfo(const string &id); //!< setup
                explicit VectorsInfo(const char   *id); //!< setup

                //! check same sizes, throw and exception otherwise
                static void CheckSamesSizes( const container &x, const container &y);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(VectorsInfo);
            };

            //! dedicated vectors for samples
            template <typename T>
            class Vectors : public VectorsInfo
            {
            public:
                typedef vector<T>                Type;     //!< alias
                typedef intr_ptr<string,Vectors> Pointer;  //!< alias
                typedef set<string,Pointer>      DataBase; //!< alias


                Type X;  //!< X data
                Type Y;  //!< Y data
                Type Yf; //!< Y fit data

                //! setup
                inline explicit Vectors(const string &id) : VectorsInfo(id), X(), Y(), Yf() {}

                //! setup
                inline explicit Vectors(const char   *id) : VectorsInfo(id), X(), Y(), Yf() {}

                //! destructor
                inline virtual ~Vectors() throw() {}

                //! check same sizes and prepare Yf
                inline void adjust()
                {
                    CheckSamesSizes(X,Y);
                    Yf.free();
                    Yf.make( Y.size(), 0);
                }

                //! test
                inline bool AreValid() const throw()
                {
                    return (X.size()==Y.size()) && (X.size()==Yf.size());
                }

                //! make a new sample
                inline Sample<T> & add_to( Samples<T> &samples, const size_t nvar_max=0)
                {
                    adjust();
                    return samples.add(X,Y,Yf,nvar_max);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vectors);
            };

            //! database of Vectors
            template <typename T>
            class VectorsDB : public Vectors<T>::DataBase
            {
            public:

                typedef typename Vectors<T>::DataBase DB_Type; //!< alias

                //! destructor
                inline virtual ~VectorsDB() throw() {}

                //! setup
                inline explicit VectorsDB(const size_t n) : DB_Type(n,as_capacity) {}

                //! create vectors with default size
                inline Vectors<T> & create( const string &id, const size_t sz=0)
                {
                    typename Vectors<T>::Pointer p = new Vectors<T>(id);
                    if(sz>0)
                    {
                        p->X. make(sz,0);
                        p->Y. make(sz,0);
                        p->Yf.make(sz,0);
                    }
                    if( !this->insert(p) ) VectorsInfo::MultipleVectorsException(id);
                    return *p;
                }

                //! create vectors with default size
                inline Vectors<T> & create( const char *id, const size_t sz=0)
                {
                    const string _(id); return create(_,sz);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(VectorsDB);
            };

        }

    }
}

#endif

