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

            //------------------------------------------------------------------
            //
            //! base class to write some formatted data
            //
            //------------------------------------------------------------------
            class Format : public Object
            {
            public:
                typedef arc_ptr<Format> Handle; //!< alias
                virtual ~Format() throw();      //!< cleanup

                //! write formatted data to an output stream
                virtual ios::ostream & write(ios::ostream &, const void *) const = 0;

                const unsigned components; //!< >=1
                const bool     isScalar;   //!< components=1
                const bool     isVector;   //!< components>1

            protected:
                //! setup info
                explicit Format(const unsigned) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Format);
            };

            //------------------------------------------------------------------
            //
            //! base class for primary types
            //
            //------------------------------------------------------------------
            class PrimaryFormat : public Format
            {
            public:
                typedef arc_ptr<PrimaryFormat> Handle; //!< alias

                explicit PrimaryFormat(const char *);  //!< Format(1), setup format
                virtual ~PrimaryFormat() throw();      //!< cleanup
                const string format;                   //!< C-style format

            private:
                Y_DISABLE_COPY_AND_ASSIGN(PrimaryFormat);
            };

            //------------------------------------------------------------------
            //
            //! a collection of DIM primary types
            //
            //------------------------------------------------------------------
            template <typename T, const unsigned DIM>
            class LimitedFormat : public Format
            {
            public:
                const PrimaryFormat &F; //!< how to write one item

                //! cleanup
                inline virtual ~LimitedFormat() throw() {}

            protected:
                //! setup
                inline explicit LimitedFormat(const PrimaryFormat &f) : Format(DIM), F(f) { }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LimitedFormat);
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
            };


            //------------------------------------------------------------------
            //
            //! write a tuple
            //
            //------------------------------------------------------------------
            template <template <typename> class TUPLE,typename T>
            class TupleFormat : public LimitedFormat<T,sizeof(TUPLE<T>)/sizeof(T)>
            {
            public:
                //! setup
                inline explicit TupleFormat(const PrimaryFormat &f) : LimitedFormat<T,sizeof(TUPLE<T>)/sizeof(T)>(f) {}

                //! cleanup
                inline virtual ~TupleFormat() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(TupleFormat);
            };


            //------------------------------------------------------------------
            //
            //! database of formats
            //
            //------------------------------------------------------------------
            class Formats : public singleton<Formats>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const at_exit::longevity life_time = 0; //!< for singleton

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! format string for primary types
                string & formatString(const std::type_info &tid) const;

                //! format string for primary types, wrapper
                template <typename T> inline
                string & formatString() const
                {
                    static const std::type_info &tid = typeid(T);
                    return formatString(tid);
                }

                //! get a registered format
                const Format & get(const std::type_info &tid) const;

                //! get a registered format, wrapper
                template <typename T> inline
                const Format & get() const
                {
                    static const std::type_info &tid = typeid(T);
                    return get(tid);
                }

                //! high level write
                template <typename T> inline
                ios::ostream & write(ios::ostream &fp, const T &args) const
                {
                    static const Format &fmt = get<T>();
                    return fmt.write(fp,&args);
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const suffix_tree<Format::Handle>        fdb; //!< database of generic formats
                const suffix_tree<PrimaryFormat::Handle> pdb; //!< database of primary formats

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
