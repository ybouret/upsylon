//! \file
#ifndef Y_MATH_DATA_SET_INCLUDED
#define Y_MATH_DATA_SET_INCLUDED 1

#include "y/sequence/vector.hpp"
#include "y/associative/map.hpp"
#include "y/ios/icstream.hpp"
#include "y/exception.hpp"
#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"
#include "y/comparison.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon
{
    namespace math
    {
        namespace kernel
        {
            //! separators for columns
            inline bool data_set_separator(const int C) { return ' '==C || '\t' == C; }
        }

        class data_set_
        {
        public:
            typedef memory::pooled             memory_type;
            typedef key_dumper                 hasher_type;
            typedef vector<string,memory_type> strings;
            virtual ~data_set_() throw();

        protected:
            explicit data_set_() throw();
            strings labels;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_set_);
        };

        //! temporaty object to load data files
        template <typename T>
        class data_set : public data_set_
        {
        public:
            Y_DECL_ARGS(T,type);                                        //!< type aliases
            typedef sequence<type>                             *column; //!< simple pointer to a sequence
            typedef map<size_t,column,hasher_type,memory_type>  colmap; //!< [index:column] map

            //! constructor, reserve a little memory
            inline explicit data_set(size_t n=2) : columns(n,as_capacity) {}
            //! destructor
            inline virtual ~data_set() throw() {}
            //! link a column index to a columnx
            inline void use( const size_t column_index, sequence<type> &seq )
            {
                if(column_index<=0) throw exception("data_set: column index <= 0");
                if(!columns.insert(column_index,&seq))
                {
                    throw exception("data_set: multiple column #%u", unsigned(column_index));
                }
            }

            //! free all register columns
            inline void free_all() throw()
            {
                for(typename colmap::iterator i=columns.begin();i!=columns.end();++i)
                {
                    (**i).free();
                }
            }

            //! load data from input stream,
            inline void load( ios::istream &fp, const size_t skip=0, const size_t nmax=0)
            {
                labels.free();
                const size_t  nc = columns.size();
                labels.ensure(nc);

                // preparing columns, order by increasing index
                columns.sort_keys( comparison::increasing<size_t> );
                for(typename colmap::iterator i=columns.begin();i!=columns.end();++i)
                {
                    const string l = vformat("column #%u", unsigned(i.key()));
                    labels.push_back_(l);
                }

                unsigned iline = 1;
                string   line(255,as_capacity);
                for(;iline<=skip;++iline)
                {
                    if(!fp.gets(line)) throw exception("data_set: EOF while skipping line %u/%u", iline, unsigned(skip));
                }

                size_t loaded = 0;
                while(fp.gets(line))
                {
                    // check content
                    if(line.size()<=0) goto NEXT_LINE;
                    if(line[0] == '#') goto NEXT_LINE;

                    {
                        // now parse!
                        tokenizer<char> tkn(line);
                        typename colmap::iterator i=columns.begin();
                        for(size_t j=1;j<=nc;++i,++j)
                        {
                            const size_t idx = i.key();
                            while( tkn.count() != idx )
                            {
                                if(!tkn.next(kernel::data_set_separator))
                                {
                                    throw exception("%u:data_set missing %s",iline,*labels[j]);
                                }
                            }
                            const string word  = tkn.to_string();
                            const T      value = string_convert::to<T>(word,*labels[j]);
                            (**i).push_back(value);
                        }
                        if(nmax>0&&++loaded>=nmax)
                        {
                            break;
                        }
                    }
                NEXT_LINE:
                    ++iline;
                }
            }

            //! load using file name
            inline void load( const string &filename,const size_t skip=0, const size_t nmax=0)
            {
                ios::icstream fp(filename);
                load(fp,skip,nmax);
            }

            //! load using file name
            inline void load( const char *filename,const size_t skip=0, const size_t nmax=0)
            {
                const string _(filename);
                load(_,skip,nmax);
            }

            //! wrapper to load X/Y quickly
            static inline
            size_t loadXY(const string &filename,
                          const size_t  ix,
                          sequence<T>  &X,
                          const size_t  iy,
                          sequence<T>  &Y,
                          const size_t skip=0, const size_t nmax=0)
            {
                data_set<T> ds(2);
                ds.use(ix,X);
                ds.use(iy,Y);
                ds.load(filename,skip,nmax);
                assert( X.size() == Y.size() );
                return X.size();
            }

            //! wrapper to load X/Y quickly
            static inline
            size_t loadXY(const char   *filename,
                          const size_t  ix,
                          sequence<T>  &X,
                          const size_t  iy,
                          sequence<T>  &Y,
                          const size_t skip=0, const size_t nmax=0)
            {
                const string _(filename);
                return loadXY(_,ix,X,iy,Y,skip,nmax);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_set);
            colmap columns;
        };
    }
}

#endif

