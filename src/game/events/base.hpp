#ifndef SDL03_Game_Events_Base
#define SDL03_Game_Events_Base

#include <string>

namespace Game {
    namespace Events {
        class Base {
        public:
            Base();
            virtual ~Base() = 0;

        private:
            // TODO: Use enums or something here. For now strings will do
            // since I'm not 100% certain where I even want to go with this.
            // subject is sort of a bucket which an event listener can be
            // subscribed to. type is more specific. e.g. time = subject,
            // tick = type; or timer = subject, timerFinished = type.
            // I'm not sure I like this. The goal is to be able to partition
            // events so that a listener doesn't have to sort through every
            // single event, just ones it is likely to be interested in.
            // See also: log channels.
            std::string subject;
            std::string type;
        };
    }
}

#endif
