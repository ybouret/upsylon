#include "y/oxide/vtk.hpp"
#include "y/exception.hpp"
#include "y/mpl/rational.hpp"

namespace upsylon {

    namespace Oxide {

        vtk::Writer:: ~Writer() throw() {}
        vtk::Writer::  Writer(const std::type_info &t,
                              const char           *f) :
        tid(t), fmt( (0!=f) ? new string(f) : 0 )
        {
        }

        const std::type_info &  vtk::Writer:: key() const throw() { return tid; }

        bool  vtk::Writer:: isScalar() const throw() { return 1 == components(); }


        namespace
            {
                static const char __dataType[] = "int";

                template <typename T>
                class WriterI : public vtk::Writer
                {
                public:
                    inline WriterI() : vtk::Writer( typeid(T), "%ld" )
                    {
                    }

                    inline virtual ~WriterI() throw()
                    {
                    }

                    inline virtual void write( ios::ostream &fp, const void *addr) const
                    {
                        assert(addr);
                        assert(fmt.is_valid());
                        const long i = long(*static_cast<const T *>(addr) );
                        fp( **fmt, i );
                    }

                    inline virtual const char * dataType()   const throw() { return __dataType; }
                    inline virtual unsigned     components() const throw() { return  1;         }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(WriterI);
                };

                template <typename T>
                class WriterU : public vtk::Writer
                {
                public:
                    inline WriterU() : vtk::Writer( typeid(T), "%lu" )
                    {
                    }

                    inline virtual ~WriterU() throw()
                    {
                    }

                    inline virtual void write( ios::ostream &fp, const void *addr) const
                    {
                        assert(addr);
                        assert(fmt.is_valid());
                        const unsigned long i = *static_cast<const T *>(addr);
                        fp( **fmt, i );
                    }

                    inline virtual const char * dataType()   const throw() { return __dataType; }
                    inline virtual unsigned     components() const throw() { return 1;          }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(WriterU);
                };


                class WriterF : public vtk::Writer
                {
                public:
                    inline explicit WriterF() : vtk::Writer( typeid(float), "%.3g" ) {}
                    inline virtual ~WriterF() throw() {}

                    virtual void write( ios::ostream &fp, const void *addr) const
                    {
                        assert(addr);
                        assert(fmt.is_valid());
                        fp( **fmt, *static_cast<const float *>(addr)  );
                    }

                    inline virtual const char * dataType()   const throw() { return "float"; }
                    inline virtual unsigned     components() const throw() { return 1;       }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(WriterF);
                };

                class WriterD : public vtk::Writer
                {
                public:
                    inline explicit WriterD() : vtk::Writer( typeid(double), "%.3lg" ) {}
                    inline virtual ~WriterD() throw() {}

                    virtual void write( ios::ostream &fp, const void *addr) const
                    {
                        assert(addr);
                        assert(fmt.is_valid());
                        fp( **fmt, *static_cast<const double *>(addr) );
                    }

                    inline virtual const char * dataType()   const throw() { return "double"; }
                    inline virtual unsigned     components() const throw() { return 1;       }


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(WriterD);
                };

                template <typename T, typename COORD>
                class WriterMulti : public vtk::Writer
                {
                public:
                    static const unsigned Components = sizeof(COORD)/sizeof(T);

                    const vtk::SharedWriter shared;

                    explicit WriterMulti( const vtk &VTK ) :
                    vtk::Writer( typeid(COORD), NULL ),
                    shared( (vtk::Writer *)&VTK.get<T>() )
                    {
                        assert(Components>0);
                        assert(1==shared->components());
                    }

                    inline virtual ~WriterMulti() throw()
                    {
                    }

                    inline virtual void write( ios::ostream &fp, const void *addr) const
                    {
                        assert(addr);
                        assert(fmt.is_empty());
                        const T *p = static_cast<const T *>(addr);
                        shared->write(fp,p);
                        for(unsigned i=1;i<Components;++i)
                        {
                            shared->write(fp << ' ',++p);
                        }
                    }

                    inline virtual const char * dataType() const throw()
                    {
                        return shared->dataType();
                    }

                    inline virtual unsigned     components() const throw() { return Components; }



                private:
                    Y_DISABLE_COPY_AND_ASSIGN(WriterMulti);
                };

                //! using MPT::to_decimal()
                template <typename MPT>
                class WriterMP : public vtk::Writer
                {
                public:
                    inline virtual ~WriterMP() throw() {}
                    inline explicit WriterMP() : vtk::Writer( typeid(MPT), NULL ) {}
                    inline virtual const char * dataType()   const throw() { return __dataType; }
                    inline virtual unsigned     components() const throw() { return 1;          }

                    inline virtual void write( ios::ostream &fp, const void *addr) const
                    {
                        assert(addr);
                        assert(fmt.is_empty());
                        const MPT   *p = static_cast<const MPT *>(addr);
                        const string s = p->to_decimal();
                        fp << s;
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(WriterMP);
                };

                class WriterMPQ : public vtk::Writer
                {
                public:
                    const Writer &dw;

                    inline virtual ~WriterMPQ() throw() {}
                    inline explicit WriterMPQ(const vtk &VTK ) :
                    vtk::Writer( typeid(mpq), NULL ), dw(VTK.get<double>())
                    {}

                    inline virtual const char * dataType()   const throw() { return dw.dataType(); }
                    inline virtual unsigned     components() const throw() { return 1;             }

                    inline virtual void write( ios::ostream &fp, const void *addr) const
                    {
                        assert(addr);
                        assert(fmt.is_empty());
                        const mpq   *q = static_cast<const mpq *>(addr);
                        const double d = q->to_real();
                        dw.write(fp,&d);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(WriterMPQ);
                };

            }

        vtk:: ~vtk() throw()
        {
        }

        static const char Fn[] = "vtk::Writer";


        const vtk::Writer & vtk:: get(const std::type_info &tid) const
        {
            const SharedWriter *ppW = writers.search(tid);
            if(!ppW)
            {
                throw exception("%s(no <%s>)", Fn, tid.name() );
            }
            return **ppW;
        }

#define Y_VTK_I(TYPE) do { const SharedWriter w = new WriterI<TYPE>(); (void) writers.insert(w); } while(false)
#define Y_VTK_U(TYPE) do { const SharedWriter w = new WriterU<TYPE>(); (void) writers.insert(w); } while(false)
#define Y_VTK_W(TYPE) Y_VTK_I(TYPE); Y_VTK_U(unsigned TYPE)

#define Y_VTK_IB(BITS) do { const SharedWriter w = new WriterI<int##BITS##_t>(); (void) writers.insert(w); } while(false)
#define Y_VTK_UB(BITS) do { const SharedWriter w = new WriterI<uint##BITS##_t>(); (void) writers.insert(w); } while(false)
#define Y_VTK_B(BITS ) Y_VTK_IB(BITS); Y_VTK_UB(BITS)

#define Y_VTK_(TYPE,WRITER) do { const SharedWriter w = new WRITER(); if(!writers.insert(w)) throw exception("%s(multiple <" #TYPE ">)",Fn ); } while(false)

#define Y_VTK_M(TYPE,COORD) do\
{ const SharedWriter w = new WriterMulti<TYPE,COORD>(*this);\
if(!writers.insert(w)) throw exception("%s(multiple <" #TYPE "," #COORD  ">)",Fn );\
} while(false)

        vtk:: vtk() : writers(32,as_capacity)
        {

            Y_VTK_W(char);
            Y_VTK_W(short);
            Y_VTK_W(int);
            Y_VTK_W(long);
            Y_VTK_W(long long);

            Y_VTK_B(8);
            Y_VTK_B(16);
            Y_VTK_B(32);
            Y_VTK_B(64);

            Y_VTK_(float,WriterF);
            Y_VTK_(double,WriterD);

            Y_VTK_M(float,point2d<float>);
            Y_VTK_M(float,point3d<float>);

            Y_VTK_M(double,point2d<double>);
            Y_VTK_M(double,point3d<double>);

            Y_VTK_M(float,complex<float>);
            Y_VTK_M(double,complex<double>);


            Y_VTK_(mpn,WriterMP<mpn>);
            Y_VTK_(mpz,WriterMP<mpz>);
            {
                const SharedWriter w = new WriterMPQ(*this);
                if(!writers.insert(w)) throw exception("%s(multiple <%s>)",Fn,w->tid.name());
            }

#if 0
            std::cerr << "#vtk::Writer=" << writers.size() << std::endl;
            for( SharedWriters::iterator i=writers.begin();i!=writers.end();++i)
            {
                std::cerr << "\t(*) " << (*i)->key().name() << std::endl;
            }
#endif

        }


        void vtk:: writeHeader(ios::ostream & fp,
                               const unsigned major,
                               const unsigned minor) const
        {
            fp << "# vtk DataFile Version ";
            fp("%u.%u\n",major,minor);
        }

        void vtk:: writeTitle(ios::ostream &fp,
                              const string &title) const
        {
            for(size_t i=0;i<title.size();++i)
            {
                char C = title[i];
                switch(C)
                {
                    case '\n':
                    case '\r':
                        C = '_';
                        break;

                    default:
                        break;
                }
                fp << C;
            }
            fp << '\n';
            fp << "ASCII\n";
        }


        ios::ostream & vtk:: composeAs3D(ios::ostream  &fp,
                                         const Coord1D   *v,
                                         const size_t     dims,
                                         const Coord1D    pad) const
        {
            assert(dims>=1); assert(dims<=3);
            assert(v!=NULL);

            const vtk           &VTK = *this;

            VTK(fp,v[0]);
            for(size_t dim=1;dim<dims;++dim) VTK(fp << ' ',v[dim]);
            for(size_t dim=dims;dim<3;++dim) VTK(fp << ' ',pad);
            return fp;
        }

        const size_t vtk::Repeat[4] = { 0, 4, 2, 1 };

        void vtk:: structuredPoints_(ios::ostream  &fp,
                                     const size_t   dims,
                                     const Coord1D *width,
                                     const Coord1D *lower) const
        {
            assert(dims>=1); assert(dims<=3); assert(width); assert(lower);
            fp << "DATASET STRUCTURED_POINTS\n";
            composeAs3D(fp << "DIMENSIONS ",width,dims,2) << '\n';
            composeAs3D(fp << "ORIGIN ",    lower,dims,0) << '\n';
            fp << "SPACING 1 1 1\n";
        }


        void vtk:: writePointData(ios::ostream     &fp,
                                  const LayoutInfo &L) const
        {
            assert(L.dimension>=1);
            assert(L.dimension<=3);
            fp << "POINT_DATA ";
            (*this)(fp, L.items * Repeat[L.dimension] );
            fp << '\n';
        }

        const vtk::Writer & vtk:: revealField(ios::ostream &fp,
                                              const Field  &F ) const
        {
            const Writer &W = get(F.typeOfObject);

            if( W.components() == 1)
            {
                fp << "SCALARS " << F.name << ' ' << W.dataType() << '\n';
                fp << "LOOKUP_TABLE " << F.name << '\n';
            }
            else
            {
                fp << "VECTORS " << F.name << ' ' << W.dataType() << '\n';
            }

            return W;
        }

        void vtk:: writeScalar( ios::ostream &fp, const Writer &W, const void *addr ) const
        {
            W.write(fp,addr);
            fp << '\n';
        }


        void vtk:: writeVector(ios::ostream &fp,
                               const Writer &W,
                               const void   *addr ) const
        {
            W.write(fp,addr);
            for(size_t dim=W.components();dim<3;++dim)
            {
                fp << ' ' << '0';
            }
            fp << '\n';
        }


    }
}
