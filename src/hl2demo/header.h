#ifndef DEMOSTATS_HL2DEMO_HEADER_H
#define DEMOSTATS_HL2DEMO_HEADER_H

#include "dstruct.h"

namespace hl2demo {
    class header_t : public dstruct {
    public:
        explicit header_t(dstream *p__io);

        void printDebug(uint32_t indent) const override;

    private:
        void read() final;

        int32_t m_demo_version{};
        int32_t m_network_version{};
        std::string m_server_name;
        std::string m_client_name;
        std::string m_map_name;
        std::string m_game_directory;
        float m_playback_time{};
        int32_t m_ticks{};
        int32_t m_frames{};
        int32_t m_signon_length{};

    public:
        int32_t demo_version() const { return m_demo_version; }

        int32_t network_version() const { return m_network_version; }

        std::string server_name() const { return m_server_name; }

        std::string client_name() const { return m_client_name; }

        std::string map_name() const { return m_map_name; }

        std::string game_directory() const { return m_game_directory; }

        float playback_time() const { return m_playback_time; }

        int32_t ticks() const { return m_ticks; }

        int32_t frames() const { return m_frames; }

        int32_t signon_length() const { return m_signon_length; }

    };

    header_t::header_t(dstream *_io) : dstruct(_io) {
        read();
    }

    void header_t::read() {
        if (!io->compare_fixed_string("\x48\x4C\x32\x44\x45\x4D\x4F\x00", 8)) {
            throw std::runtime_error("invalid demo magic number");
        }
        m_demo_version = io->s4le();
        m_network_version = io->s4le();
        m_server_name = dstream::bytes_terminate(io->read(260));
        m_client_name = dstream::bytes_terminate(io->read(260));
        m_map_name = dstream::bytes_terminate(io->read(260));
        m_game_directory = dstream::bytes_terminate(io->read(260));
        m_playback_time = io->f4le();
        m_ticks = io->s4le();
        m_frames = io->s4le();
        m_signon_length = io->s4le();
    }

    void header_t::printDebug(uint32_t indent) const {
        std::cout << std::string(indent, '\t') << "header_t: " << std::endl
                  << std::string(indent + 1, '\t') << "demo_version=" << demo_version() << std::endl
                  << std::string(indent + 1, '\t') << "network_version=" << network_version() << std::endl
                  << std::string(indent + 1, '\t') << "server_name=" << server_name() << std::endl
                  << std::string(indent + 1, '\t') << "client_name=" << client_name() << std::endl
                  << std::string(indent + 1, '\t') << "map_name=" << map_name() << std::endl
                  << std::string(indent + 1, '\t') << "game_directory=" << game_directory() << std::endl
                  << std::string(indent + 1, '\t') << "playback_time=" << playback_time() << std::endl
                  << std::string(indent + 1, '\t') << "ticks=" << ticks() << std::endl
                  << std::string(indent + 1, '\t') << "frames=" << frames() << std::endl
                  << std::string(indent + 1, '\t') << "signon_length=" << signon_length() << std::endl;
    }
}

#endif //DEMOSTATS_HL2DEMO_HEADER_H
