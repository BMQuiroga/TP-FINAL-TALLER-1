#include "prepare_grenade_intention.h"

IntentionPrepareGrenade::IntentionPrepareGrenade(int id):
    id(id)
{
}

int IntentionPrepareGrenade::get_intention()
{
    return id;
}
