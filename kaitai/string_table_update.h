#ifndef DEMOSTATS_STRING_TABLE_UPDATE_H
#define DEMOSTATS_STRING_TABLE_UPDATE_H

#include <kaitai/kaitaistruct.h>
#include <utility>
#include <vector>
#include <iostream>

class sting_table_entry_t  {
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


class string_table_update_t : public kaitai::kstruct {
public:
    string_table_update_t(uint32_t entries, uint32_t max_entries, uint32_t user_data_size,
                          uint32_t user_data_size_bits, bool user_data_fixed_size,
                          kaitai::kstream *io) :
            kstruct(io), m_entry_count(entries), m_max_entries(max_entries), m_user_data_size(user_data_size),
            m_user_data_size_bits(user_data_size_bits), m_user_data_fixed_size(user_data_fixed_size), m_entry_bits(0) {
        while (max_entries >>= 1) ++m_entry_bits;
        _read();
    }
private:
    static const int SUBSTRING_BITS = 5;
    static const int MAX_USERDATA_BITS = 14;

    void _read();
    char read_bit_byte();
    uint64_t read_bits_int_rev(int nbits);
    std::string read_bit_bytes_term(char term = 0, bool include = false);
    std::string read_bit_bytes(size_t length);
    std::string read_bit_bytes_packed(size_t nbits);

    uint32_t m_entry_count;
    uint32_t m_max_entries;
    uint8_t m_entry_bits;
    uint32_t m_user_data_size;
    uint32_t m_user_data_size_bits;
    bool m_user_data_fixed_size;
    std::vector<sting_table_entry_t> m_entries;
public:
    const std::vector<sting_table_entry_t> &entries() { return m_entries; }
};

void string_table_update_t::_read() {

    bool m_is_dict = m__io->read_bits_int(1);

    if (m_is_dict) {
        return;
    }

    std::vector<sting_table_entry_t> history;

    int last_entry = -1;
    for (int entry_i = 0; entry_i < m_entry_count; entry_i++) {
        int entry_index = last_entry + 1;
        if (!m__io->read_bits_int(1)) {
            // is there and entry index, the number of entry bits is specified by the max entries
            entry_index = m__io->read_bits_int(m_entry_bits);
        }
        last_entry = entry_index;

        if ( history.size() > 31 )
        {
            history.erase( history.begin() );
        }

        std::string entry;
        std::string userdata;

        bool has_entry = m__io->read_bits_int(1);
        if (has_entry) { // has entry
            if (m__io->read_bits_int(1)) { // sub string check
                int history_index = read_bits_int_rev(5);
                int length = read_bits_int_rev(SUBSTRING_BITS);
                entry = history[history_index].value().substr(0, length);
            }
            entry.append(m__io->bytes_to_str(read_bit_bytes_term(0, false), std::string("ascii")));
        }

        bool has_userdata = m__io->read_bits_int(1);
        if (has_userdata) { // has user data check
            if (m_user_data_fixed_size) {
                userdata = read_bit_bytes_packed(m_user_data_size_bits);
            } else {
                int userDataBytes = read_bits_int_rev(MAX_USERDATA_BITS);
                userdata = read_bit_bytes(userDataBytes);
            }
        }

        auto table_entry = sting_table_entry_t(entry_index, has_entry, has_userdata, entry, userdata);
        m_entries.push_back(table_entry);
        history.push_back(table_entry);
    }
}

uint64_t string_table_update_t::read_bits_int_rev(int nbits) {
    uint64_t num = m__io->read_bits_int(nbits);
    uint64_t temp, reversed = 0;
    for (int i = 0; i < nbits; i++) {
        temp = (num & (1 << i));
        if(temp) {
            reversed |= (1 << ((nbits - 1) - i));
        }
    }
    return reversed;
}

char string_table_update_t::read_bit_byte() {
    return static_cast<char>(read_bits_int_rev(8) & 0xff);
}

std::string string_table_update_t::read_bit_bytes_term(char term, bool include)  {
    std::string result;
    while(true) {
        char t = read_bit_byte();
        if (t == term) {
            if (include)
                result.push_back(t);
            break;
        } else {
            result.push_back(t);
        }
    }
    return result;
}

std::string string_table_update_t::read_bit_bytes(size_t length) {
    std::string result;
    for (int i = 0; i < length; ++i) {
        result.push_back(read_bit_byte());
    }
    return result;
}

std::string string_table_update_t::read_bit_bytes_packed(size_t nbits) {
    std::string result = read_bit_bytes(nbits/8);
    // if there are any bits left over, read them too
    if (nbits % 8) {
        result.push_back(static_cast<char>(read_bits_int_rev(nbits % 8) & 0xff));
    }
    return result;
}

#endif //DEMOSTATS_STRING_TABLE_UPDATE_H
