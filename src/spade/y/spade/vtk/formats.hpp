//! \file
#ifndef Y_SPADE_VTK_FORMAT_INCLUDED
#define Y_SPADE_VTK_FORMAT_INCLUDED 1

#include "y/spade/types.hpp"
#include "y/string.hpp"
#include "y/associative/suffix-tree.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace Spade {

        namespace VTK {

            class Format : public Object
            {
            public:
                typedef arc_ptr<Format> Handle;
                virtual ~Format() throw();

                const unsigned components;
                virtual ios::ostream & write(ios::ostream &, const void *) const = 0;

            protected:
                explicit Format(const unsigned) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Format);
            };

            class PrimaryFormat : public Format
            {
            public:
                typedef arc_ptr<PrimaryFormat> Handle;
                explicit PrimaryFormat(const char *);
                virtual ~PrimaryFormat() throw();

                const string format;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(PrimaryFormat);
            };

            template <typename T, const unsigned DIM>
            class LimitedFormat : public Format
            {
            public:
                const PrimaryFormat &F;

                inline virtual ~LimitedFormat() throw()
                {
                }

                inline virtual ios::ostream & write(ios::ostream &fp, const void *addr) const
                {
                    assert(addr);
                    const T *p = static_cast<const T *>(addr);
                    F.write(fp,&p[0]);
                    for(size_t dim=1;dim<DIM;++dim)
                    {
                        F.write(fp<<' ',&p[dim]);
                    }
                    return fp;
                }

            protected:
                inline explicit LimitedFormat(const PrimaryFormat &f) :
                Format(DIM),
                F(f)
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(LimitedFormat);
            };

            template <template <typename> class TUPLE,typename T>
            class TupleFormat : public LimitedFormat<T,sizeof(TUPLE<T>)/sizeof(T)>
            {
            public:
                inline explicit TupleFormat(const PrimaryFormat &f) : LimitedFormat<T,sizeof(TUPLE<T>)/sizeof(T)>(f) {}

                inline virtual ~TupleFormat() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(TupleFormat);
            };



            class Formats : public singleton<Formats>
            {
            public:

                suffix_tree<Format::Handle>        fdb;
                suffix_tree<PrimaryFormat::Handle> pdb;
                
                static const at_exit::longevity life_time = 0;

                string & formatString(const std::type_info &tid) const;

                template <typename T> inline
                string & formatString() const
                {
                    static const std::type_info &tid = typeid(T);
                    return formatString(tid);
                }

                const Format & get(const std::type_info &tid) const;

                template <typename T> inline
                const Format & get() const
                {
                    static const std::type_info &tid = typeid(T);
                    return get(tid);
                }

                template <typename T> inline
                ios::ostream & write(ios::ostream &fp, const T &args) const
                {
                    static const Format &fmt = get<T>();
                    return fmt.write(fp,&args);
                }



            private:
                explicit Formats();
                virtual ~Formats() throw();
                friend class singleton<Formats>;
                Y_DISABLE_COPY_AND_ASSIGN(Formats);
            };

        }

    }

}

#endif
