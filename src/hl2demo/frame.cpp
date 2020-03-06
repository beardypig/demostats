#include <iostream>
#include "frame.h"

hl2demo::frame_t::frame_t(dstream *io) : dstruct(io), m_body(nullptr) {
    read();
}

void hl2demo::frame_t::read() {
    m_frame_type = static_cast<frame_type_t>(io->u1());
    m_tick = io->s4le();
    m_player_slot = io->u1();

    switch(m_frame_type) {
        case FRAME_TYPE_DEM_SIGNON:
        case FRAME_TYPE_DEM_PACKET:
            m_body = new frame_packet_t(io);
            break;
        case FRAME_TYPE_DEM_SYNCTICK:
            m_body = new frame_synctick_t(io);
            break;
        case FRAME_TYPE_DEM_CONSOLECMD:
            m_body = new frame_consolecmd_t(io);
            break;
        case FRAME_TYPE_DEM_USERCMD:
            m_body = new frame_usercmd_t(io);
            break;
        case FRAME_TYPE_DEM_DATATABLES:
            m_body = new frame_datatables_t(io);
            break;
        case FRAME_TYPE_DEM_STOP:
            m_body = new frame_stop_t(io);
            break;
        case FRAME_TYPE_DEM_CUSTOMDATA:
            m_body = new frame_customdata_t(io);
            break;
        case FRAME_TYPE_DEM_STRINGTABLES:
            m_body = new frame_stringtables_t(io);
            break;
    }
}

void hl2demo::frame_t::printDebug(uint32_t indent) const {
    std::cout << std::string(indent, '\t') << "frame_t: " << std::endl
              << std::string(indent+1, '\t') << "frame_type=" << frame_type_t_name(frame_type()) << std::endl
              << std::string(indent+1, '\t') << "tick=" << tick() << std::endl
              << std::string(indent+1, '\t') << "player_slot=" << player_slot() << std::endl;
    switch(m_frame_type) {
        case FRAME_TYPE_DEM_SIGNON:
        case FRAME_TYPE_DEM_PACKET:
            dynamic_cast<frame_packet_t*>(body())->printDebug(indent+1);
            break;
        case FRAME_TYPE_DEM_SYNCTICK:
            dynamic_cast<frame_synctick_t*>(body())->printDebug(indent+1);
            break;
        case FRAME_TYPE_DEM_CONSOLECMD:
            dynamic_cast<frame_consolecmd_t*>(body())->printDebug(indent+1);
            break;
        case FRAME_TYPE_DEM_USERCMD:
            dynamic_cast<frame_usercmd_t*>(body())->printDebug(indent+1);
            break;
        case FRAME_TYPE_DEM_DATATABLES:
            dynamic_cast<frame_datatables_t*>(body())->printDebug(indent+1);
            break;
        case FRAME_TYPE_DEM_STOP:
            dynamic_cast<frame_stop_t*>(body())->printDebug(indent+1);
            break;
        case FRAME_TYPE_DEM_CUSTOMDATA:
            dynamic_cast<frame_customdata_t*>(body())->printDebug(indent+1);
            break;
        case FRAME_TYPE_DEM_STRINGTABLES:
            dynamic_cast<frame_stringtables_t*>(body())->printDebug(indent+1);
            break;
    }
}

hl2demo::frame_t::~frame_t() {
    delete m_body;
}

std::string hl2demo::frame_type_t_name(hl2demo::frame_type_t type) {
    switch (type) {
        case FRAME_TYPE_DEM_SIGNON:
            return "FRAME_TYPE_DEM_SIGNON";
        case FRAME_TYPE_DEM_PACKET:
            return "FRAME_TYPE_DEM_PACKET";
        case FRAME_TYPE_DEM_SYNCTICK:
            return "FRAME_TYPE_DEM_SYNCTICK";
        case FRAME_TYPE_DEM_CONSOLECMD:
            return "FRAME_TYPE_DEM_CONSOLECMD";
        case FRAME_TYPE_DEM_USERCMD:
            return "FRAME_TYPE_DEM_USERCMD";
        case FRAME_TYPE_DEM_DATATABLES:
            return "FRAME_TYPE_DEM_DATATABLES";
        case FRAME_TYPE_DEM_STOP:
            return "FRAME_TYPE_DEM_STOP";
        case FRAME_TYPE_DEM_CUSTOMDATA:
            return "FRAME_TYPE_DEM_CUSTOMDATA";
        case FRAME_TYPE_DEM_STRINGTABLES:
            return "FRAME_TYPE_DEM_STRINGTABLES";
    }
}

void hl2demo::frame_packet_t::printDebug(uint32_t indent) const {
    std::cout << std::string(indent, '\t') << "frame_packet_t:" << std::endl;
    for(int i = 0; i < 2; i++) {
        m_user[i]->printDebug(indent+1);
    }
    std::cout << std::string(indent+1, '\t') << "seq_in=" << m_seq_in << std::endl;
    std::cout << std::string(indent+1, '\t') << "seq_out=" << m_seq_out << std::endl;
    std::cout << std::string(indent+1, '\t') << "n_messages=" << m_messages->size() << std::endl;
    for(auto & msg : *m_messages) {
        msg->printDebug(indent+1);
    }
}

void hl2demo::frame_packet_t::read() {
    for (int i = 0; i < 2; ++i) {
        m_user[i] = new split_t(io);
    }
    m_seq_in = io->s4le();
    m_seq_out = io->s4le();
    auto length = io->s4le();

    char* buf = new char[length];
    io->read(buf, length);
    auto d = std::string(buf, length);
    auto messages_data_io = new dstream(d);
    m_messages = new std::vector<message_t*>();
    while (!messages_data_io->is_eof()) {
        auto m = new message_t(messages_data_io);
        m_messages->push_back(m);
    }
    delete[] buf;
    delete messages_data_io;
}

hl2demo::frame_packet_t::~frame_packet_t() {
    for(int i = 0; i < 2; i++) {
        delete m_user[i];
    }

    for(auto & it : *m_messages) {
        delete it;
    }
    delete m_messages;
}

hl2demo::frame_packet_t::frame_packet_t(hl2demo::dstream *io) : dstruct(io) {
    read();
}

void hl2demo::frame_usercmd_t::printDebug(uint32_t indent) const {
    std::cout << std::string(indent, '\t') << "frame_usercmd_t" << std::endl;
}

void hl2demo::frame_usercmd_t::read() {
    auto length = io->s4le();
    m_data = dstream::bytes_terminate(io->read(length));
}

void hl2demo::frame_datatables_t::printDebug(uint32_t indent) const {
    std::cout << std::string(indent, '\t') << "frame_datatables_t" << std::endl;
}

void hl2demo::frame_datatables_t::read() {
    auto length = io->s4le();
    m_data = dstream::bytes_terminate(io->read(length));
}

void hl2demo::frame_customdata_t::printDebug(uint32_t indent) const {
    std::cout << std::string(indent, '\t') << "frame_customdata_t" << std::endl;
}

void hl2demo::frame_customdata_t::read() {
    auto length = io->s4le();
    m_data = dstream::bytes_terminate(io->read(length));
}

void hl2demo::frame_stringtables_t::printDebug(uint32_t indent) const {
    std::cout << std::string(indent, '\t') << "frame_stringtables_t" << std::endl;
}

void hl2demo::frame_stringtables_t::read() {
    auto length = io->s4le();
    m_data = dstream::bytes_terminate(io->read(length));
    m_data_io = new dstream(m_data);
    m_table_count = m_data_io->u1();
    for (int i = 0; i < m_table_count; ++i) {
        auto table = new frame_stringtable_t(m_data_io);
        m_tables.push_back(table);
    }
}

hl2demo::frame_stringtables_t::~frame_stringtables_t() {
    delete m_data_io;
    for (auto table : m_tables) {
        delete table;
    }
}

void hl2demo::frame_consolecmd_t::printDebug(uint32_t indent) const {
    std::cout << std::string(indent, '\t') << "frame_consolecmd_t" << std::endl;
    std::cout << std::string(indent+1, '\t') << "cmd=" << cmd() << std::endl;
}

void hl2demo::frame_consolecmd_t::read() {
    auto length = io->s4le();
    m_cmd = dstream::bytes_terminate(io->read(length));
}

hl2demo::frame_consolecmd_t::frame_consolecmd_t(hl2demo::dstream *io) : dstruct(io) {
    read();
}

void hl2demo::frame_synctick_t::printDebug(uint32_t indent) const {
    std::cout << std::string(indent, '\t') << "frame_synctick_t" << std::endl;
}

void hl2demo::frame_stop_t::printDebug(uint32_t indent) const {
    std::cout << std::string(indent, '\t') << "frame_stop_t" << std::endl;
}

void hl2demo::frame_packet_t::split_t::read() {
    m_flags = io->s4le();
    m_view_origin = new vector3_t(io->f4le(), io->f4le(), io->f4le());
    m_view_angles = new vector3_t(io->f4le(), io->f4le(), io->f4le());
    m_local_view_angles = new vector3_t(io->f4le(), io->f4le(), io->f4le());
    m_view_origin2 = new vector3_t(io->f4le(), io->f4le(), io->f4le());
    m_view_angles2 = new vector3_t(io->f4le(), io->f4le(), io->f4le());
    m_local_view_angles2 = new vector3_t(io->f4le(), io->f4le(), io->f4le());
}

hl2demo::frame_packet_t::split_t::~split_t() {
    delete m_view_origin;
    delete m_view_angles;
    delete m_local_view_angles;
    delete m_view_origin2;
    delete m_view_angles2;
    delete m_local_view_angles2;
}

void hl2demo::frame_packet_t::split_t::printDebug(uint32_t indent) const {
    std::cout << std::string(indent, '\t') << "split_t:" << std::endl;
    std::cout << std::string(indent+1, '\t') << "flags=" << m_flags << std::endl;
    std::cout << std::string(indent+1, '\t') << "view_origin=(" << m_view_origin->x() << ", " << m_view_origin->y() << ", " << m_view_origin->z() << ")" << std::endl;
    std::cout << std::string(indent+1, '\t') << "view_angles=(" << m_view_angles->x() << ", " << m_view_angles->y() << ", " << m_view_angles->z() << ")" << std::endl;
    std::cout << std::string(indent+1, '\t') << "local_view_angles=(" << m_local_view_angles->x() << ", " << m_local_view_angles->y() << ", " << m_local_view_angles->z() << ")" << std::endl;
    std::cout << std::string(indent+1, '\t') << "view_origin2=(" << m_view_origin2->x() << ", " << m_view_origin2->y() << ", " << m_view_origin2->z() << ")" << std::endl;
    std::cout << std::string(indent+1, '\t') << "view_angles2=(" << m_view_angles2->x() << ", " << m_view_angles2->y() << ", " << m_view_angles2->z() << ")" << std::endl;
    std::cout << std::string(indent+1, '\t') << "local_view_angles2=(" << m_local_view_angles2->x() << ", " << m_local_view_angles2->y() << ", " << m_local_view_angles2->z() << ")" << std::endl;
}

hl2demo::frame_packet_t::split_t::split_t(hl2demo::dstream *io) : dstruct(io), m_flags(0), m_view_angles(nullptr), m_local_view_angles(nullptr), m_view_origin2(nullptr), m_view_angles2(nullptr), m_local_view_angles2(nullptr) {
    read();
}

void hl2demo::frame_stringtables_t::frame_stringtable_t::read() {
    m_table_name = io->read_until('\x00');
    m_table_size = io->s2le();
    m_strings = new std::vector<std::string>();
    for (int i = 0; i < m_table_size; i++) {
        m_strings->push_back(io->read_until('\x00'));
    }
}

hl2demo::frame_stringtables_t::frame_stringtable_t::frame_stringtable_t(hl2demo::dstream *io) : dstruct(io) {
    read();
}
