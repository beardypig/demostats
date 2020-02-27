#include <iostream>
#include <fstream>
#include <netmessages.pb.h>

#include "string_table_update.h"
#include "vlq_base128_le.h"
#include "message.h"
#include "frame.h"
#include "header.h"
#include "demoparser.h"
#include "player_info.h"

struct player_death {
    int userid;
    int attacker;
    int assister;
    bool assistedflash;
    std::string weapon;
    std::string weapon_itemid;
    std::string weapon_fauxitemid;
    std::string weapon_originalowner_xuid;
    bool headshot;
    int dominated;
    int revenge;
    int wipe;
    int penetrated;
    bool noreplay;
};

struct string_table_t {
    size_t id;
    int max_entries;
    int user_data_size;
    int user_data_size_bits;
    bool user_data_fixed_size;
    int flags;
    std::string name;
    std::vector<sting_table_entry_t> entries;
};

std::vector<std::unique_ptr<player_info_t>> players;
std::vector<string_table_t> tables;

int main() {
    bool complete = false;

    std::ifstream is("003391530591110299847_1909302021.dem", std::ifstream::binary);
    demostream demo(&is);

    std::cout << "Map: " << demo.demo->header()->map_name() << std::endl;

    CSVCMsg_GameEventList gameEventList;

    for (frame_t* frame : *demo.demo->frames()) {
        switch (frame->frame_type()) {
            case frame_t::FRAME_TYPE_DEM_PACKET:
            case frame_t::FRAME_TYPE_DEM_SIGNON: {
                auto packet = dynamic_cast<frame_t::frame_packet_t *>(frame->body());
                for (auto const &message : *packet->messages()->messages()) {
                    int32_t msg_type_id = message->msg_type_id()->value();
                    if (SVC_Messages_IsValid(msg_type_id)) {
                        //std::cout << frame->tick() << ": " << SVC_Messages_Name(msg_type_id) << std::endl;
                        std::string data = message->data();
                        switch (static_cast<SVC_Messages>(msg_type_id)) {
                            case svc_ServerInfo:
                                break;
                            case svc_SendTable:
                                break;
                            case svc_ClassInfo:
                                break;
                            case svc_SetPause:
                                break;
                            case svc_CreateStringTable: {
                                CSVCMsg_CreateStringTable create_msg;
                                CSVCMsg_UpdateStringTable svc_msg;

                                create_msg.ParseFromString(data);
                                std::cout << "New Table: " << create_msg.name() << std::endl;

                                // TODO: not have to reverse these bytes
                                // kaitai reads bits from msb to lsb, but they are actually encoded lsb to msb
                                std::string table_data;
                                for (const auto &c : create_msg.string_data()) {
                                    int temp;
                                    char reversed = 0;
                                    for (int i = 0; i < 8; i++) {
                                        temp = (c & (1 << i));
                                        if(temp) {
                                            reversed |= (1 << ((8 - 1) - i)) & 0xff;
                                        }
                                    }
                                    table_data.push_back(reversed);
                                }
                                std::stringstream update_stream(table_data);
                                kaitai::kstream ks(&update_stream);

                                int temp = create_msg.max_entries();
                                int entry_bits = 0;
                                while (temp >>= 1) ++entry_bits;
                                string_table_update_t stu(create_msg.num_entries(), create_msg.max_entries(), create_msg.user_data_size(), create_msg.user_data_size_bits(), create_msg.user_data_fixed_size(), &ks);
                                auto newTable = string_table_t {
                                    tables.size(),
                                    create_msg.max_entries(),
                                    create_msg.user_data_size(),
                                    create_msg.user_data_size_bits(),
                                    create_msg.user_data_fixed_size(),
                                    create_msg.flags(),
                                    create_msg.name(),
                                    stu.entries()
                                };

                                tables.push_back(newTable);

                                if (create_msg.name() == "userinfo") {
                                    std::cout << "Creating player info table:" << std::endl;
                                    players.resize(create_msg.max_entries());
                                    for (auto const &entry : stu.entries()) {
                                        if (entry.has_userdata()) {
                                            std::stringstream userdata_stream(entry.userdata());
                                            kaitai::kstream uks(&userdata_stream);
                                            players[entry.entry()] = std::make_unique<player_info_t>(&uks);
                                            std::cout << players[entry.entry()]->user_id() << " : " << players[entry.entry()]->name() << std::endl;
                                        }
                                    }
                                }

                            }
                            break;
                            case svc_UpdateStringTable: {
                                CSVCMsg_UpdateStringTable update_msg;
                                update_msg.ParseFromString(data);

                                auto table = tables[update_msg.table_id()];

                                std::string table_data;
                                for (const auto &c : update_msg.string_data()) {
                                    int temp;
                                    char reversed = 0;
                                    for (int i = 0; i < 8; i++) {
                                        temp = (c & (1 << i));
                                        if(temp) {
                                            reversed |= (1 << ((8 - 1) - i)) & 0xff;
                                        }
                                    }
                                    table_data.push_back(reversed);
                                }
                                std::stringstream update_stream(table_data);
                                kaitai::kstream ks(&update_stream);

                                string_table_update_t stu(update_msg.num_changed_entries(),
                                                          table.max_entries,
                                                          table.user_data_size,
                                                          table.user_data_size_bits,
                                                          table.user_data_fixed_size,
                                                          &ks);

                                for (auto const &entry : stu.entries()) {
                                    for (size_t i = 0; i < table.entries.size(); ++i) {
                                        if (entry.entry() == table.entries[i].entry()) {
                                            table.entries[i] = entry;
                                        }
                                    }
                                }

                                if (table.name == "userinfo") {
                                    std::cout << "Updating player info table:" << std::endl;
                                    for (auto const &entry : table.entries) {
                                        if (entry.has_userdata()) {
                                            std::stringstream userdata_stream(entry.userdata());
                                            kaitai::kstream uks(&userdata_stream);
                                            players[entry.entry()] = std::make_unique<player_info_t>(&uks);
                                        }
                                    }
                                }

                            }
                                break;
                            case svc_VoiceInit:
                                break;
                            case svc_VoiceData:
                                break;
                            case svc_Print: {
                                CSVCMsg_Print svc_msg;
                                svc_msg.ParseFromString(data);
                                if (svc_msg.has_text()) {
                                    std::cout << "svc_Print: " << svc_msg.text();
                                }
                                break;
                            }
                            case svc_Sounds:
                                break;
                            case svc_SetView:
                                break;
                            case svc_FixAngle:
                                break;
                            case svc_CrosshairAngle:
                                break;
                            case svc_BSPDecal:
                                break;
                            case svc_SplitScreen:
                                break;
                            case svc_UserMessage:
                                break;
                            case svc_EntityMessage:
                                break;
                            case svc_GameEvent: {
                                CSVCMsg_GameEvent gameEvent;
                                gameEvent.ParseFromString(data);
                                CSVCMsg_GameEventList_descriptor_t gameEventDescriptor;

                                for (auto const &descriptor : gameEventList.descriptors()) {
                                    if (descriptor.eventid() == gameEvent.eventid()) {
                                        gameEvent.set_event_name(descriptor.name());
                                        gameEventDescriptor = descriptor;
                                        break;
                                    }
                                }

                                if (gameEvent.event_name() == "player_death") {
                                    auto death = player_death {
                                            gameEvent.keys()[0].val_short(),
                                            gameEvent.keys()[1].val_short(),
                                            gameEvent.keys()[2].val_short(),
                                            gameEvent.keys()[3].val_bool(),
                                            gameEvent.keys()[4].val_string(),
                                            gameEvent.keys()[5].val_string(),
                                            gameEvent.keys()[6].val_string(),
                                            gameEvent.keys()[7].val_string(),
                                            gameEvent.keys()[8].val_bool(),
                                            gameEvent.keys()[9].val_short(),
                                            gameEvent.keys()[10].val_short(),
                                            gameEvent.keys()[11].val_short(),
                                            gameEvent.keys()[12].val_short(),
                                            gameEvent.keys()[13].val_bool(),
                                    };

                                    //std::cout << gameEventDescriptor.DebugString();
                                    //std::cout << gameEvent.DebugString();
                                    std::cout << death.attacker << " killed " << death.userid << " with " << death.weapon << std::endl;
                                }

                                break;
                            }
                            case svc_PacketEntities:
                                break;
                            case svc_TempEntities:
                                break;
                            case svc_Prefetch:
                                break;
                            case svc_Menu:
                                break;
                            case svc_GameEventList: {
                                gameEventList.ParseFromString(data);
                                break;
                            }
                            case svc_GetCvarValue:
                                break;
                            case svc_PaintmapData:
                                break;
                            case svc_CmdKeyValues:
                                break;
                            case svc_EncryptedData:
                                break;
                            case svc_HltvReplay:
                                break;
                            case svc_Broadcast_Command:
                                break;
                        }
                    } else if (NET_Messages_IsValid(msg_type_id)) {
                        //std::cout << frame->tick() << ": " << NET_Messages_Name(msg_type_id) << std::endl;
                        switch (static_cast<NET_Messages>(msg_type_id)) {
                            case net_NOP:
                                break;
                            case net_Disconnect:
                                break;
                            case net_File:
                                break;
                            case net_SplitScreenUser:
                                break;
                            case net_Tick:
                                break;
                            case net_StringCmd:
                                break;
                            case net_SetConVar:
                                break;
                            case net_SignonState:
                                break;
                            case net_PlayerAvatarData:
                                break;
                        }
                    }
                }
                break;
            }
            case frame_t::FRAME_TYPE_DEM_SYNCTICK:
                // just to update the current tick
                break;
            case frame_t::FRAME_TYPE_DEM_CONSOLECMD: {
                auto consolecmd = dynamic_cast<frame_t::frame_consolecmd_t *>(frame->body());
                break;
            }
            case frame_t::FRAME_TYPE_DEM_USERCMD: {
                auto usercmd = dynamic_cast<frame_t::frame_usercmd_t *>(frame->body());
                break;
            }
            case frame_t::FRAME_TYPE_DEM_DATATABLES: {
                auto datatables = dynamic_cast<frame_t::frame_datatables_t *>(frame->body());
                break;
            }
            case frame_t::FRAME_TYPE_DEM_STOP:
                complete = true;
                break;
            case frame_t::FRAME_TYPE_DEM_CUSTOMDATA:
                break;
            case frame_t::FRAME_TYPE_DEM_STRINGTABLES:
                auto string_table = dynamic_cast<frame_t::frame_stringtables_t *>(frame->body());
                for (auto const &table : *string_table->string_table()->tables()) {
                    std::cout << "table: " << table->tablename() << std::endl;

                }
        }
    }

    return 0;
}
