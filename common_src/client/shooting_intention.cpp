#include "shooting_intention.h"

IntentionShooting::IntentionShooting(bool is_shooting, int id):
    is_shooting(is_shooting),
    id(id)
{
}

int IntentionShooting::get_intention(){
    return id;
}

