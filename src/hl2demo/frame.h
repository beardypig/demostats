#ifndef DEMOSTATS_HL2DEMO_FRAME_H
#define DEMOSTATS_HL2DEMO_FRAME_H

#include "dstruct.h"
#include "message.h"
#include <vector>

namespace hl2demo {
    enum frame_type_t {
        FRAME_TYPE_DEM_SIGNON = 1,
        FRAME_TYPE_DEM_PACKET = 2,
        FRAME_TYPE_DEM_SYNCTICK = 3,
        FRAME_TYPE_DEM_CONSOLECMD = 4,
        FRAME_TYPE_DEM_USERCMD = 5,
        FRAME_TYPE_DEM_DATATABLES = 6,
        FRAME_TYPE_DEM_STOP = 7,
        FRAME_TYPE_DEM_CUSTOMDATA = 8,
        FRAME_TYPE_DEM_STRINGTABLES = 9,
        FRAME_TYPE_DEM_LAST = FRAME_TYPE_DEM_STRINGTABLES
    };

    static std::string frame_type_t_name(frame_type_t type);

    class vector3_t {
    public:
        explicit vector3_t(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

        float x() const { return m_x; }

        float y() const { return m_y; }

        float z() const { return m_z; }

    private:
        float m_x;
        float m_y;
        float m_z;
    };

    class frame_t : public dstruct {
    public:
        explicit frame_t(dstream *io);

        ~frame_t();

        void printDebug(uint32_t indent) const final;

        frame_type_t frame_type() const { return m_frame_type; }

        int32_t tick() const { return m_tick; }

        uint8_t player_slot() const { return m_player_slot; }

        dstruct *body() const { return m_body; }

    private:
        void read() final;

        frame_type_t m_frame_type;
        int32_t m_tick;
        uint8_t m_player_slot;
        dstruct *m_body;
    };

    class frame_packet_t : public dstruct {
    public:
        class split_t : public dstruct {
        public:
            explicit split_t(dstream *io);

            ~split_t();

            void printDebug(uint32_t indent) const override;

            int32_t flags() const { return m_flags; };

            const vector3_t *view_origin() const { return m_view_origin; };

            const vector3_t *view_angles() const { return m_view_angles; };

            const vector3_t *local_view_angles() const { return m_local_view_angles; };

            const vector3_t *view_origin2() const { return m_view_origin2; };

            const vector3_t *view_angles2() const { return m_view_angles2; };

            const vector3_t *local_view_angles2() const { return m_local_view_angles2; };
        private:
            void read() final;

            int32_t m_flags;
            vector3_t *m_view_origin;
            vector3_t *m_view_angles;
            vector3_t *m_local_view_angles;
            vector3_t *m_view_origin2;
            vector3_t *m_view_angles2;
            vector3_t *m_local_view_angles2;
        };

        explicit frame_packet_t(dstream *io);

        ~frame_packet_t();

        void printDebug(uint32_t indent) const override;

        //const std::vector<split_t *> *user() const { return m_user; }

        int32_t seq_in() const { return m_seq_in; }

        int32_t seq_out() const { return m_seq_out; }

        //int32_t messages_length() const { return m_length; }
        //const std::string &messages_data() const { return m_messages_data; }
        const std::vector<message_t *> &messages() const { return *m_messages; };
    private:
        void read() final;

        split_t* m_user[2];
        int32_t m_seq_in;
        int32_t m_seq_out;
        std::vector<message_t *> *m_messages;
    };

    class frame_usercmd_t : public dstruct {
    public:
        frame_usercmd_t(dstream *io) : dstruct(io) {
            read();
        }

        void read() override;

        void printDebug(uint32_t indent) const override;

        const std::string &data() const { return m_data; }

    private:
        std::string m_data;
    };

    class frame_datatables_t : public dstruct {
    public:
        frame_datatables_t(dstream *io) : dstruct(io) {
            read();
        }

        void read() override;

        void printDebug(uint32_t indent) const override;

        const std::string &data() const { return m_data; }

    private:
        std::string m_data;
    };

    class frame_customdata_t : public dstruct {
    public:
        frame_customdata_t(dstream *io) : dstruct(io) {
            read();
        }

        void printDebug(uint32_t indent) const override;

    private:
        void read() final;

        std::string m_data;
    };

    class frame_stringtables_t : public dstruct {
    public:
        class frame_stringtable_t : public dstruct {
        public:
            explicit frame_stringtable_t(dstream *io);

            const std::string &table_name() { return m_table_name; }

            const std::vector<std::string> &strings() { return *m_strings; }

        private:
            void read() final;
            std::string m_table_name;
            int16_t m_table_size;
            std::vector<std::string> *m_strings;
        };

        frame_stringtables_t(dstream *io) : dstruct(io) {}

        ~frame_stringtables_t();

        void read() override;

        void printDebug(uint32_t indent) const override;

        const std::vector<frame_stringtable_t *> &tables() const { return m_tables; }

    private:
        std::string m_data;
        dstream *m_data_io;
        uint8_t m_table_count;
        std::vector<frame_stringtable_t *> m_tables;
    };

    class frame_consolecmd_t : public dstruct {
    public:
        frame_consolecmd_t(dstream *io);

        void printDebug(uint32_t indent) const override;

        const std::string &cmd() const { return m_cmd; }

    private:
        void read() final;

        std::string m_cmd;
    };

    class frame_stop_t : public dstruct {
    public:
        explicit frame_stop_t(dstream *io) : dstruct(io) {} // no content, no read implementation
        void printDebug(uint32_t indent) const override;
    };

    class frame_synctick_t : public dstruct {
    public:
        explicit frame_synctick_t(dstream *io) : dstruct(io) {} // no content, no read implementation
        void printDebug(uint32_t indent) const override;
    };


}

#endif //DEMOSTATS_HL2DEMO_FRAME_H
