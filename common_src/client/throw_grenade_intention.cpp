#include "throw_grenade_intention.h"

IntentionThrowGrenade::IntentionThrowGrenade(int id):
    id(id)
{
}

int IntentionThrowGrenade::get_intention()
{
    return id;
}
