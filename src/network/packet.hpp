#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <typeinfo>

#include "../json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;


namespace packet{
    enum pckt_t {identity = 42, file_transfer = 43, ping_transfer = 44, text_transfer = 45, empty = 1};
    struct resp {
        json data;
        pckt_t type = empty;
    };

    inline int generate_indentity_packet(std::string nickname, std::string tcpIp, int tcpPort, std::string os, json* out){
        pckt_t pck = identity;
        (*out)["pckt"] = pck;
        (*out)["name"] = nickname;
        (*out)["ip"] = tcpIp;
        (*out)["port"] = tcpPort;
        (*out)["os"] = os;

        return 1;
    }

    inline int validate_packet(json pkt_j){
        static std::array<std::string, 5> arr = {"name", "pckt", "ip", "port", "os"};
        
        for (int i=0; i < arr.size(); i++){
            if (pkt_j[arr[i]] == NULL){
                return 0;
            }
        }

        if(
            ((int) pkt_j["pckt"] != (int) identity) &
            ((int) pkt_j["pckt"] != (int) file_transfer) &
            ((int) pkt_j["pckt"] != (int) ping_transfer) &
            ((int) pkt_j["pckt"] != (int) text_transfer)
        )
        {
            return 0;
        }

        return 1;
    }

    inline struct resp udp_parse_packet(std::string packet){
        auto pkt_j = json::parse(packet);
        struct resp response = {};

        if (!validate_packet(pkt_j)){
            return response;
        }
        
        if((int) pkt_j["pckt"] == identity){
            response = {pkt_j, identity};
            return response;
        }
        
        return response; 
    }
}
#endif