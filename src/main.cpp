#include <iostream>
#include <fstream>
#include <netmessages.pb.h>
#include "hl2demo/header.h"
#include "hl2demo/frame.h"
#include "hl2demo/string_table_update.h"
#include "hl2demo/dstream.h"
#include <chrono>

using namespace std::chrono;

struct string_table_t {
    size_t id;
    int max_entries;
    int user_data_size;
    int user_data_size_bits;
    bool user_data_fixed_size;
    int flags;
    std::string name;
    std::vector<hl2demo::sting_table_entry_t> entries;
};

std::vector<string_table_t> tables;

int main(int argc, char** argv) {
    if (argc <= 1) {
        auto prog = std::string(argv[0]);
        auto progname = prog.substr(prog.find_last_of("/\\") + 1);
        std::cout << "usage: " << progname << " <demopath>" << std::endl;
        exit(1);
    }
    std::ifstream is(argv[1], std::ifstream::binary);
    hl2demo::dstream demo(&is);
    hl2demo::header_t header(&demo);
    header.printDebug(0);
    int frames = 0;
    int previous_frame_type = -1;
    auto start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    while (!demo.is_eof()) {
        hl2demo::frame_t frame(&demo);
        frames++;
        switch (static_cast<hl2demo::frame_type_t>(frame.frame_type())) {
            case hl2demo::FRAME_TYPE_DEM_PACKET:
            case hl2demo::FRAME_TYPE_DEM_SIGNON: {
                auto packet = dynamic_cast<hl2demo::frame_packet_t *>(frame.body());
                for (auto const &message : packet->messages()) {
                    int32_t msg_type_id = message->msg_type_id();
                    const std::string& data = message->data();
                    if (SVC_Messages_IsValid(msg_type_id)) {
                        switch (static_cast<SVC_Messages>(msg_type_id)) {
                            case svc_ServerInfo: {
                                auto m = message->message<CSVCMsg_ServerInfo>();
                                break;
                            }
                            case svc_SendTable: {
                                auto m = message->message<CSVCMsg_SendTable>();
                                break;
                            }
                            case svc_ClassInfo: {
                                auto m = message->message<CSVCMsg_ClassInfo>();
                                break;
                            }
                            case svc_SetPause: {
                                auto m = message->message<CSVCMsg_SetPause>();
                                break;
                            }
                            case svc_CreateStringTable: {
                                auto m = message->message<CSVCMsg_CreateStringTable>();
                                break;
                            }
                            case svc_UpdateStringTable: {
                                auto m = message->message<CSVCMsg_UpdateStringTable>();
                                break;
                            }
                            case svc_VoiceInit: {
                                auto m = message->message<CSVCMsg_VoiceInit>();
                                break;
                            }
                            case svc_VoiceData: {
                                auto m = message->message<CSVCMsg_VoiceData>();
                                break;
                            }
                            case svc_Print: {
                                auto m = message->message<CSVCMsg_Print>();
                                break;
                            }
                            case svc_Sounds: {
                                auto m = message->message<CSVCMsg_Sounds>();
                                break;
                            }
                            case svc_SetView: {
                                auto m = message->message<CSVCMsg_SetView>();
                                break;
                            }
                            case svc_FixAngle: {
                                auto m = message->message<CSVCMsg_FixAngle>();
                                break;
                            }
                            case svc_CrosshairAngle: {
                                auto m = message->message<CSVCMsg_CrosshairAngle>();
                                break;
                            }
                            case svc_BSPDecal: {
                                auto m = message->message<CSVCMsg_BSPDecal>();
                                break;
                            }
                            case svc_SplitScreen: {
                                auto m = message->message<CSVCMsg_SplitScreen>();
                                break;
                            }
                            case svc_UserMessage: {
                                auto m = message->message<CSVCMsg_UserMessage>();
                                break;
                            }
                            case svc_EntityMessage: {
                                auto m = message->message<CSVCMsg_EntityMsg>();
                                break;
                            }
                            case svc_GameEvent: {
                                auto m = message->message<CSVCMsg_GameEvent>();
                                break;
                            }
                            case svc_PacketEntities: {
                                auto m = message->message<CSVCMsg_PacketEntities>();
                                break;
                            }
                            case svc_TempEntities: {
                                auto m = message->message<CSVCMsg_TempEntities>();
                                break;
                            }
                            case svc_Prefetch: {
                                auto m = message->message<CSVCMsg_Prefetch>();
                                break;
                            }
                            case svc_Menu: {
                                auto m = message->message<CSVCMsg_Menu>();
                                break;
                            }
                            case svc_GameEventList: {
                                auto m = message->message<CSVCMsg_GameEventList>();
                                break;
                            }
                            case svc_GetCvarValue: {
                                auto m = message->message<CSVCMsg_GetCvarValue>();
                                break;
                            }
                            case svc_PaintmapData: {
                                auto m = message->message<CSVCMsg_PaintmapData>();
                                break;
                            }
                            case svc_CmdKeyValues: {
                                auto m = message->message<CSVCMsg_CmdKeyValues>();
                                break;
                            }
                            case svc_EncryptedData: {
                                auto m = message->message<CSVCMsg_EncryptedData>();
                                break;
                            }
                            case svc_HltvReplay: {
                                auto m = message->message<CSVCMsg_HltvReplay>();
                                break;
                            }
                            case svc_Broadcast_Command: {
                                auto m = message->message<CSVCMsg_Broadcast_Command>();
                                break;
                            }
                        }
                    }
                    if (NET_Messages_IsValid(msg_type_id)){
                        switch (static_cast<NET_Messages>(msg_type_id)) {
                            case net_NOP: {
                                auto m = message->message<CNETMsg_NOP>();
                                break;
                            }
                            case net_Disconnect: {
                                auto m = message->message<CNETMsg_Disconnect>();
                                break;
                            }
                            case net_File: {
                                auto m = message->message<CNETMsg_File>();
                                break;
                            }
                            case net_SplitScreenUser: {
                                auto m = message->message<CNETMsg_SplitScreenUser>();
                                break;
                            }
                            case net_Tick: {
                                auto m = message->message<CNETMsg_Tick>();
                                break;
                            }
                            case net_StringCmd: {
                                auto m = message->message<CNETMsg_StringCmd>();
                                break;
                            }
                            case net_SetConVar: {
                                auto m = message->message<CNETMsg_SetConVar>();
                                break;
                            }
                            case net_SignonState: {
                                auto m = message->message<CNETMsg_SignonState>();
                                break;
                            }
                            case net_PlayerAvatarData: {
                                auto m = message->message<CNETMsg_PlayerAvatarData>();
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case hl2demo::FRAME_TYPE_DEM_SYNCTICK: {
                auto synctick = dynamic_cast<hl2demo::frame_synctick_t*>(frame.body());
                break;
            }
            case hl2demo::FRAME_TYPE_DEM_CONSOLECMD:{
                auto consolecmd = dynamic_cast<hl2demo::frame_consolecmd_t *>(frame.body());
                break;
            }
            case hl2demo::FRAME_TYPE_DEM_USERCMD:{
                auto usercmd = dynamic_cast<hl2demo::frame_usercmd_t *>(frame.body());
                break;
            }
            case hl2demo::FRAME_TYPE_DEM_DATATABLES:{
                auto datatable = dynamic_cast<hl2demo::frame_datatables_t *>(frame.body());
                break;
            }
            case hl2demo::FRAME_TYPE_DEM_STOP:{
                auto stop = dynamic_cast<hl2demo::frame_stop_t *>(frame.body());
                break;
            }
            case hl2demo::FRAME_TYPE_DEM_CUSTOMDATA:{
                auto customdata = dynamic_cast<hl2demo::frame_customdata_t *>(frame.body());
                break;
            }
            case hl2demo::FRAME_TYPE_DEM_STRINGTABLES: {
                auto string_table = dynamic_cast<hl2demo::frame_stringtables_t *>(frame.body());
                for (auto const table : string_table->tables()) {
                    std::cout << "table: " << table->name() << std::endl;
                }
                break;
            }
            default:
                std::cout << demo.tellg() << ": frame_type=" << frame.frame_type() << ", previous_frame_type=" << previous_frame_type << std::endl;
                throw std::runtime_error("invalid frame type");
        }
        previous_frame_type = frame.frame_type();
    }
    auto end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    std::cout << "frames: " << frames << ", time taken: " << end - start << "ms" << std::endl;
}