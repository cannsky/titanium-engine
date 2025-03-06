#pragma once

#include <string>

struct RLRequest {
    float player_x;
    float player_z;
    float move_speed;
    float rot_x;
    float rot_speed;
    float player_health;
    float attack_cooldown;
    float opp_x;
    float opp_z;
    float opp_rot;
    float opp_health;

    float reward;
    bool done;
};