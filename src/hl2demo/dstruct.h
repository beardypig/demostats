#ifndef DEMOSTATS_DSTRUCT_H
#define DEMOSTATS_DSTRUCT_H

#include "dstream.h"

namespace hl2demo {
    class dstruct {
    public:
        explicit dstruct(dstream *_io) {
            io = _io;
            read();
        }
        virtual ~dstruct() {}
        virtual void printDebug(uint32_t indent) const {};
    protected:
        dstream *io;
        virtual void read() {};
    };

};

#endif //DEMOSTATS_DSTRUCT_H
