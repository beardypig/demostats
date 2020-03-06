#include "string_table_update.h"

void hl2demo::string_table_update_t::read() {
    bool m_is_dict = io->bitbool();

    if (m_is_dict) {
        return;
    }

    std::vector<sting_table_entry_t> history;

    int last_entry = -1;
    for (int entry_i = 0; entry_i < m_entry_count; entry_i++) {
        int entry_index = last_entry + 1;
        if (!io->bitbool()) {
            // is there and entry index, the number of entry bits is specified by the max entries
            entry_index = io->ulong(m_entry_bits);
        }
        last_entry = entry_index;

        if (history.size() > 31) {
            history.erase(history.begin());
        }

        std::string entry;
        std::string userdata;

        bool has_entry = io->bitbool();
        if (has_entry) { // has entry
            if (io->bitbool()) { // sub string check
                int history_index = io->ulong(5);
                int length = io->ulong(SUBSTRING_BITS);
                entry = history[history_index].value().substr(0, length);
            }
            entry.append(io->read_until(0));
        }

        bool has_userdata = io->bitbool();
        if (has_userdata) { // has user data check
            if (m_user_data_fixed_size) {
                userdata = io->read(m_user_data_size_bits/8);
                // if there are any bits left over, read them too
                if (m_user_data_size_bits % 8) {
                    userdata.push_back(static_cast<char>(io->ulong(m_user_data_size_bits % 8) & 0xffu));
                }
            } else {
                int userDataBytes = io->ulong(MAX_USERDATA_BITS);
                userdata = io->read(userDataBytes);
            }
        }

        auto table_entry = sting_table_entry_t(entry_index, has_entry, has_userdata, entry, userdata);
        m_entries.push_back(table_entry);
        history.push_back(table_entry);
    }
}