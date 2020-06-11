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
            class Writer : public Object
            {
            public:
                typedef arc_ptr<Writer> Handle; //!< alias
                virtual ~Writer() throw();      //!< cleanup

                //! write formatted data to an output stream
                virtual ios::ostream & write(ios::ostream &, const void *) const = 0;

                const unsigned components; //!< >=1
                const bool     isScalar;   //!< components=1
                const bool     isVector;   //!< components>1

            protected:
                //! setup info
                explicit Writer(const unsigned) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Writer);
            };

            //------------------------------------------------------------------
            //
            //! base class for native types
            //
            //------------------------------------------------------------------
            class NativeWriter : public Writer
            {
            public:
                typedef arc_ptr<NativeWriter> Handle; //!< alias

                explicit NativeWriter(const char *);  //!< Format(1), setup format
                virtual ~NativeWriter() throw();      //!< cleanup
                const string format;                   //!< C-style format

            private:
                Y_DISABLE_COPY_AND_ASSIGN(NativeWriter);
            };

            //------------------------------------------------------------------
            //
            //! a repeat of COUNT native types
            //
            //------------------------------------------------------------------
            template <typename T, const unsigned COUNT>
            class RepeatWriter : public Writer
            {
            public:
                const NativeWriter &F; //!< how to write one item

                //! cleanup
                inline virtual ~RepeatWriter() throw() {}

            protected:
                //! setup
                inline explicit RepeatWriter(const NativeWriter &f) : Writer(COUNT), F(f) { }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RepeatWriter);
                inline virtual ios::ostream & write(ios::ostream &fp, const void *addr) const
                {
                    assert(addr);
                    const T *p = static_cast<const T *>(addr);
                    F.write(fp,&p[0]);
                    for(size_t dim=1;dim<COUNT;++dim)
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
            class TupleWriter : public RepeatWriter<T,sizeof(TUPLE<T>)/sizeof(T)>
            {
            public:
                //! setup
                inline explicit TupleWriter(const NativeWriter &f) : RepeatWriter<T,sizeof(TUPLE<T>)/sizeof(T)>(f) {}

                //! cleanup
                inline virtual ~TupleWriter() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(TupleWriter);
            };


            //------------------------------------------------------------------
            //
            //! database of writers
            //
            //------------------------------------------------------------------
            class Writers : public singleton<Writers>
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
                const Writer & get(const std::type_info &tid) const;

                //! get a registered format, wrapper
                template <typename T> inline
                const Writer & get() const
                {
                    static const std::type_info &tid = typeid(T);
                    return get(tid);
                }

                //! high level write
                template <typename T> inline
                ios::ostream & write(ios::ostream &fp, const T &args) const
                {
                    static const Writer &fmt = get<T>();
                    return fmt.write(fp,&args);
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const suffix_tree<Writer::Handle>       wdb; //!< database of generic formats
                const suffix_tree<NativeWriter::Handle> ndb; //!< database of primary formats

            private:
                explicit Writers();
                virtual ~Writers() throw();
                friend class singleton<Writers>;
                Y_DISABLE_COPY_AND_ASSIGN(Writers);
            };

        }

    }

}

#endif
