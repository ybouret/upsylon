

#include "y/concurrent/device/simt.hpp"
#include <iomanip>
#include "y/os/real-time-clock.hpp"

namespace upsylon {

    namespace concurrent
    {



        static const char pfx[] = "[simt.";

        simt:: simt() :
        self(),
        access( self.synchronize ),
        number(self.size()),
        online(0),
        course(),
        joined(0),
        finish(),
        kcode(0),
        kargs(0),
        verbose( query_simt_verbosity() )
        {
            self.once(loop_stub,this);
            Y_MUTEX_PROBE(access,online>=number);
            Y_CREW_PRINTLN(pfx<<"init] synchronized...");
        }

        const engine & simt:: bulk() const throw()
        {
            return self;
        }

        void simt:: loop(executable code, void *args)
        {
            assert(code);
            assert(number==online);
            assert(joined<=0);
            assert(0     ==kcode);
            assert(0     ==kargs);

            kcode = code;
            kargs = args;

            Y_CREW_PRINTLN(pfx<<"loop] go!");
            course.broadcast();

            real_time_clock rtc;
            rtc.sleep(1);

            access.lock();
            if(++joined>number)
            {
                Y_CREW_PRINTLN(pfx<<"done] @primary");
            }
            else
            {
                Y_CREW_PRINTLN(pfx<<"wait] @primary");
                finish.wait(access);
            }

            joined = 0;
            kcode  = 0;
            kargs  = 0;
            Y_CREW_PRINTLN(pfx<<"ready!]");
            access.unlock();
            



        }

        simt:: ~simt() throw()
        {
            assert(0==kcode);
            assert(0==kargs);
            Y_CREW_PRINTLN(pfx<<"quit]");
            course.broadcast();
            Y_MUTEX_PROBE(access,online<=0);
        }

        void simt:: join() throw()
        {
            kcode = 0;
            kargs = 0;
        }

        void simt:: loop_stub(void *p,const context &c, lockable &) throw()
        {
            assert(p);
            static_cast<simt*>(p)->loop_code(c);
        }


        void simt:: loop_code(const context &ctx) throw()
        {
            //------------------------------------------------------------------
            //
            // initialize when called by crew
            //
            //------------------------------------------------------------------
            const char *label   = ctx.label;

            //------------------------------------------------------------------
            //
            // synchronize all threads
            //
            //------------------------------------------------------------------
            access.lock();
            ++online;
            Y_CREW_PRINTLN(pfx<<"loop] @"<<label << " (ready=" << std::setw(4) << online << "/" << number << ")" );


            //------------------------------------------------------------------
            // wait on the LOCKED mutex
            //------------------------------------------------------------------
        LOOP:
            course.wait(access);
            
            //------------------------------------------------------------------
            // wake on the LOCKED mutex
            //------------------------------------------------------------------
            if(kcode)
            {
                access.unlock();
                kcode(kargs,ctx,access);
                access.lock();

                // barrier pattern, LOCKED
                if(++joined>number)
                {
                    Y_CREW_PRINTLN(pfx<<"done] @"<<label);
                    finish.broadcast();
                }
                else
                {
                    Y_CREW_PRINTLN(pfx<<"wait] @"<<label);
                    finish.wait(access);
                    assert(joined<=0);
                }

                goto LOOP;
            }
            else
            {
                // done
                --online;
                Y_CREW_PRINTLN(pfx<<"done] @"<<label <<" => online: " << online);
                access.unlock();
                return;
            }


        }

    }

}


#include "y/string/env.hpp"

namespace upsylon
{
    namespace concurrent
    {
        bool simt:: query_simt_verbosity()
        {
            if(  environment::flag(Y_VERBOSE_SIMT) )
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    }

}


