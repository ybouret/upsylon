
#include "y/concurrent/nucleus/cluster.hpp"
#include "y/os/hw.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include "y/string.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/string/convert.hpp"
#include <iostream>

namespace upsylon
{

    namespace concurrent
    {

        namespace nucleus
        {

            cluster:: ~cluster() throw()
            {
            }

            cluster:: cluster(const size_t start_,
                              const size_t width_,
                              const size_t every_) throw() :
            object(), dnode<cluster>(),
            start(start_),
            width(width_),
            every(max_of<size_t>(every_,1)),
            count(width/every)
            {
                aliasing::_(width) = count*every;
            }

            static const char fn[] = "concurrent::cluster: ";

            cluster * cluster:: create(const size_t start,
                                       size_t       width,
                                       size_t       every)
            {
                const size_t num_procs = hardware::nprocs();
                if(start>=num_procs)
                {
                    throw exception("%sstart@%u>=%u",fn,unsigned(start),unsigned(num_procs));
                }

                const size_t max_width = num_procs-start;
                if(width<=0) width     = max_width;
                if(width>max_width)
                {
                    throw exception("%swidth=%u>=%u",fn,unsigned(width),unsigned(max_width));

                }

                every = clamp<size_t>(1,every,width);

                return new cluster(start,width,every);
            }

            cluster * cluster:: create(const string &description)
            {
                static const char start[] = "cluster.start";
                static const char width[] = "cluster.width";
                static const char every[] = "cluster.every";

                vector<string,memory::pooled> words(4,as_capacity);
                tokenizer<char>::split_with(words,description,':');
                const size_t n = words.size();
                for(size_t i=n;i>0;--i) words[i].clean_with(" \t");
                
                switch(n)
                {
                    case 1: return create(0, string_convert::to<size_t>(words[1],width), 1);
                    case 2: return create( string_convert::to<size_t>(words[1],start), string_convert::to<size_t>(words[2],width), 1);
                    case 3: return create( string_convert::to<size_t>(words[1],start), string_convert::to<size_t>(words[2],width), string_convert::to<size_t>(words[3],every));

                    default:
                        throw exception("%sinvalid #fields=%u",fn,unsigned(n));
                }
                return NULL;
            }

            std::ostream & operator<<(std::ostream &os, const cluster &cls)
            {
                os << '{' << '#' << cls.count << '<' << '-' << cls.start << ':' << cls.width << ':' << cls.every << '}';
                os << '=' << '[';
                size_t rank = 0;
                os << cls.core_of(rank);
                for(rank=1;rank<cls.count;++rank) os << ',' << cls.core_of(rank);
                os << ']';
                return os;
            }

            size_t cluster:: core_of(const size_t worker_rank) const
            {
                assert(worker_rank<count);
                return start + every*worker_rank;
            }


        }

    }

}

