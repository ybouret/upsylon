#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/counting/parts.hpp"
#include "y/counting/permutations.hpp"
#include "y/ios/icstream.hpp"
#include "y/associative/suffix/store.hpp"
#include "y/fs/disk/istream.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace {

    static inline bool accept_single(const char C) throw()
    {
        switch(C)
        {
        case 'a':
        case 'i':
                return true;

            default: break;
        }
        return false;
    }

}

Y_UTEST(phrases)
{
    if(argc>1)
    {
        const string alphabet = argv[1];
        const size_t letters  = alphabet.size();
        if(letters>0)
        {
            suffix_store<char> db;
            suffix_store<char> phrases;
            if(argc>2)
            {
                const string            fn = argv[2];
                ios::shared_disk_buffer buf = new ios::disk_buffer<>(BUFSIZ);
                ios::readable_disk_file raw(fn);
                ios::disk_istream       fp(raw,buf);
                string line(100,as_capacity,false);
                while( fp.gets(line) )
                {
                    line.clean_with(" \t\r\n");
                    const size_t len = line.size();
                    if(len>0)
                    {
                        (void) db.insert(*line,len);
                    }
                }
                std::cerr << "|db| = " << db.entries << std::endl;
            }

            // prepare all different readings
            permutations<char> readings(argv[1],letters);
            std::cerr << "#readings=" << readings.count << std::endl;

            // prepare all different partitions
            integer_partitions      parts(letters);
            std::cerr << "#outcomes=" << parts.count << std::endl;

            const accessible<char>   &reading = readings; // current reading
            const accessible<size_t> &part    = *parts;   // current part

            string phrase(2*letters,as_capacity,false);
            string word(letters,as_capacity,false);

            // outer loop: all readings
            for(readings.boot();readings.good();readings.next())
            {
                //std::cerr << reading << std::endl;
                // inner loop: all partitions
                for( parts.boot(); parts.good(); parts.next() )
                {
                    permutations<size_t> lengths(part);

                    // core loop: extract letters
                    const size_t num_lengths = lengths.size();

                    for(lengths.boot();lengths.good();lengths.next())
                    {
                        const char *p = &reading[1];
                        phrase.clear();
                        for(size_t i=1;i<=num_lengths;++i)
                        {
                            const size_t n = lengths[i];
                            word.clear();
                            for(size_t j=n;j>0;--j)
                            {
                                word += *(p++);
                            }
                            if(db.entries && !db.has(*word,n) ) continue;
                            if(n==1)
                            {
                                if(!accept_single(word[0])) continue;
                            }
                            phrase += word;
                            if(i<num_lengths) phrase += ' ';
                        }
                        phrase.clean_with(' ');
                        if(phrase.size()>0 && phrases.insert(*phrase,phrase.size()) )
                        {
                            std::cerr << '[' << phrase << ']' << std::endl;
                        }
                    }
                }

            }
        }

    }
}
Y_UTEST_DONE()


