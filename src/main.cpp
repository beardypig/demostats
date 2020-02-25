#include <iostream>
#include <fstream>
#include <kaitai/kaitaistream.h>
#include <netmessages.pb.h>

#include "vlq_base128_le.h"
#include "message.h"
#include "frame.h"
#include "header.h"
#include "hl2demo.h"

int main() {

    std::ifstream is("003391530591110299847_1909302021.dem", std::ifstream::binary);
    kaitai::kstream ks(&is);
    hl2demo_t data(&ks);

    std::cout << "Map: " << data.header()->map_name() << std::endl;

    for (frame_t* frame : *data.frames()) {
        switch (frame->frame_type()) {
            case frame_t::FRAME_TYPE_DEM_PACKET:
            case frame_t::FRAME_TYPE_DEM_SIGNON:
                auto packet = dynamic_cast<frame_t::frame_packet_t *>(frame->body());
                std::cout << "Demo Packet: "<< packet->messages()->messages()->size() << " messages" << std::endl;
                for (message_t* message : *packet->messages()->messages()) {
                    auto message_type = message->msg_type_id()->value();
                    if (SVC_Messages_IsValid(message_type)) {
                        std::cout << SVC_Messages_Name(message_type) << std::endl;;
                        CSVCMsg_ServerInfo svc_msg;
                        if (svc_msg.ParseFromString(message->data())) {
                            std::cout << "HLTV: " << (svc_msg.is_hltv() ? "yes" : "no") << std::endl;
                        }
                    } else if (NET_Messages_IsValid(message_type)) {
                        std::cout << NET_Messages_Name(message_type) << std::endl;;
                    }
                    break;
                }
                break;
        }
        break;
    }

    return 0;
}
