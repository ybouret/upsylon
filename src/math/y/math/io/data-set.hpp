//! \file
#ifndef Y_MATH_DATA_SET_INCLUDED
#define Y_MATH_DATA_SET_INCLUDED 1

#include "y/associative/map.hpp"
#include "y/container/sequence.hpp"
#include "y/ios/istream.hpp"
#include "y/exception.hpp"
#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"
#include "y/comparison.hpp"

namespace upsylon
{
    namespace math
    {
        //! temporaty object to load data files
        template <typename T>
        class data_set
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef sequence<type>           sequence_type;
            typedef sequence_type           *column;
            typedef map<size_t,column>       colmap;

            inline explicit data_set() : columns(4,as_capacity) {}
            inline virtual ~data_set() throw() {}

            inline void use( const size_t column_index, sequence_type &seq )
            {
                if(column_index<=0) throw exception("data_set: column index <= 0");
                if(!columns.insert(column_index,&seq))
                {
                    throw exception("data_set: multiple column #%u", unsigned(column_index));
                }
            }

            inline void free_all() throw()
            {
                for(typename colmap::iterator i=columns.begin();i!=columns.end();++i)
                {
                    (**i).free();
                }
            }

            inline void load( ios::istream &fp, const size_t skip=0, const size_t nmax=0)
            {
                // preparing columns, order by increasing index
                columns.sort_keys( comparison::increasing<size_t> );

                unsigned iline = 1;
                string   line;
                for(;iline<=skip;++iline)
                {
                    if(!fp.gets(line)) throw exception("data_set: EOF while skipping line %u/%u", iline, unsigned(skip));
                }
                size_t loaded = 0;
                while(fp.gets(line))
                {
                    // check content
                    if(line.size()<=0) continue;
                    if(line[0] == '#') continue;


                    // now parse!
                    tokenizer<char> tkn(line);
                    for(typename colmap::iterator i=columns.begin();i!=columns.end();++i)
                    {
                        const size_t idx = i.key();
                        while( tkn.count() != idx )
                        {
                            if(!tkn.next(__sep))
                            {
                                throw exception("%u:data_set missing column #%u",iline,unsigned(idx));
                            }
                        }
                        const string word  = tkn.to_string();
                        const T      value = string_convert::to<T>(word); // todo: add field?
                        (**i).push_back(value);
                    }
                    if(nmax>0&&++loaded>=nmax)
                    {
                        break;
                    }
                    ++iline;
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_set);
            colmap columns;
            static inline bool __sep(const int C) { return ' '==C || '\t' == C; }
        };
    }
}

#endif

