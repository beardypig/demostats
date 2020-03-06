#ifndef DEMOSTATS_STRING_TABLE_UPDATE_H
#define DEMOSTATS_STRING_TABLE_UPDATE_H

#include <string>
#include "hl2demo/dstream.h"
#include "hl2demo/dstruct.h"

namespace demostats {
    class sting_table_entry_t {
    private:
        int m_entry;
        bool m_has_value;
        std::string m_value;
        bool m_has_userdata;
        std::string m_userdata;
    public:
        sting_table_entry_t(int entry, bool has_value, bool has_userdata, std::string value, std::string userdata)
                : m_entry(entry),
                  m_has_value(has_value),
                  m_has_userdata(has_userdata),
                  m_value(std::move(value)),
                  m_userdata(std::move(userdata)) {}

        int entry() const { return m_entry; }
        bool has_value() const { return m_has_value; }
        const std::string &value() const { return m_value; }
        bool has_userdata() const { return m_has_userdata; }
        const std::string &userdata() const { return m_userdata; }
    };


    class string_table_update_t : public dstruct {
    public:
        string_table_update_t(uint32_t entries, uint32_t max_entries, uint32_t user_data_size,
                              uint32_t user_data_size_bits, bool user_data_fixed_size,
                              dstream *io) :
                dstruct(io), m_entry_count(entries), m_max_entries(max_entries), m_user_data_size(user_data_size),
                m_user_data_size_bits(user_data_size_bits), m_user_data_fixed_size(user_data_fixed_size),
                m_entry_bits(0) {
            while (max_entries >>= 1) ++m_entry_bits;
        }
        void read();

    private:
        static const int SUBSTRING_BITS = 5;
        static const int MAX_USERDATA_BITS = 14;

        uint32_t m_entry_count;
        uint32_t m_max_entries;
        uint8_t m_entry_bits;
        uint32_t m_user_data_size;
        uint32_t m_user_data_size_bits;
        bool m_user_data_fixed_size;
        std::vector <sting_table_entry_t> m_entries;
    public:
        const std::vector <sting_table_entry_t> &entries() { return m_entries; }
    };

};

#endif //DEMOSTATS_STRING_TABLE_UPDATE_H
