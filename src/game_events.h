#ifndef DEMOSTATS_GAME_EVENTS_H
#define DEMOSTATS_GAME_EVENTS_H

#include <string>
#include <netmessages.pb.h>

struct player_death_t {
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

    static player_death_t fromGameEvent(const CSVCMsg_GameEvent &gameEvent) {
        return player_death_t {
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
    }
};

struct player_connect_full_t {
    int userid;
    int index;

    static player_connect_full_t fromGameEvent(const CSVCMsg_GameEvent &gameEvent) {
        return player_connect_full_t {
                gameEvent.keys()[0].val_short(),
                gameEvent.keys()[1].val_byte()
        };
    }
};

struct player_say_t {
    int userid;         // user ID on server
    std::string msg;    // the say text

    static player_say_t fromGameEvent(const CSVCMsg_GameEvent &gameEvent) {
        return player_say_t {
                gameEvent.keys()[0].val_short(),
                gameEvent.keys()[1].val_string()
        };
    }
};


#endif //DEMOSTATS_GAME_EVENTS_H
