#ifndef DEMOSTATS_HL2DEMO_MESSAGES_H
#define DEMOSTATS_HL2DEMO_MESSAGES_H

#include <netmessages.pb.h>
#include "dstruct.h"

namespace hl2demo {
    class message_t : public dstruct {
    public:
        explicit message_t(dstream *io);
        ~message_t();

        void printDebug(uint32_t indent) const override;

        uint64_t msg_type_id() const { return m_msg_type_id; }

        uint64_t length() const { return m_length; }

        const std::string &data() const { return m_data; }

        template <typename T>
        const T *message() {
            m_message = new T();
            m_message->ParseFromString(m_data);
            return dynamic_cast<T*>(m_message);
        }

    private:
        void read() final;

        uint64_t m_msg_type_id;
        uint64_t m_length;
        std::string m_data;
        google::protobuf::Message *m_message;
    };
}

#endif //DEMOSTATS_HL2DEMO_MESSAGES_H
