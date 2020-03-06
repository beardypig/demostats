#include <iostream>
#include <bitset>
#include "message.h"

void hl2demo::message_t::read() {
    m_msg_type_id = io->vlq<uint64_t>();
    m_length = io->vlq<uint64_t>();
    m_data = io->read(m_length);
}

void hl2demo::message_t::printDebug(uint32_t indent) const {
    std::cout << std::string(indent, '\t') << "message_t:" << std::endl;
    std::cout << std::string(indent+1, '\t') << "msg_type_id=" << m_msg_type_id << std::endl;
    std::cout << std::string(indent+1, '\t') << "msg_length=" << m_length << std::endl;
    if (SVC_Messages_IsValid(m_msg_type_id)) {
        std::cout << std::string(indent + 1, '\t') << "msg_type=" << SVC_Messages_Name(m_msg_type_id) << std::endl;
    } else if (NET_Messages_IsValid(m_msg_type_id)) {
        std::cout << std::string(indent + 1, '\t') << "msg_type=" << NET_Messages_Name(m_msg_type_id) << std::endl;
    }
}

hl2demo::message_t::message_t(hl2demo::dstream *io) : dstruct(io), m_message(nullptr) {
    read();
}

hl2demo::message_t::~message_t() {
    delete m_message;
}
