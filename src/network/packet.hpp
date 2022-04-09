#ifndef PACKET_H
#define PACKET_H

#include <iostream>

#include "../json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;


namespace packet{
    enum pckt_t {identity = 0x2b, file_transfer = 0x2c, ping_transfer = 0x2d, text_transfer = 0x2e};
    inline int generate_indentity_packet(std::string nickname, std::string tcpIp, int tcpPort, std::string os, json* out){
        pckt_t pck = identity;
        (*out)["pckt"] = pck;
        (*out)["name"] = nickname;
        (*out)["ip"] = tcpIp;
        (*out)["port"] = tcpPort;
        (*out)["os"] = os;

        std::cout << (*out)["os"] << std::endl;

        return 1;
    }

    inline int validate_packet(json pkt_j){
        static std::array<std::string, 5> arr = {"name", "pckt", "ip", "port", "os"};
        
        for (int i=0; i < 5; i++){
            if (pkt_j[arr[i]] == NULL){
                return 0;
            }
        }

        if(
            ((pkt_j["pckt"] != identity) &&
            (pkt_j["pckt"] != file_transfer) &&
            (pkt_j["pckt"] != ping_transfer) &&
            (pkt_j["pckt"] != text_transfer)) ||
            (((std::string) pkt_j["ip"]).length() != 15)
        ){
            return 0;
        }

        return 1;
    }

    inline int udp_parse_packet(std::string packet){
        auto pkt_j = json::parse(packet);
        if (!validate_packet(pkt_j)){
            return 0;
        }

        if(pkt_j["pckt_type"] == identity){
            // add them to group
        }

        return 1;
    }
}
#endif