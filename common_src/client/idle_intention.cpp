#include "idle_intention.h"

IntentionIdle::IntentionIdle(int id) :
    id(id)
{
}

int IntentionIdle::get_intention()
{
    return id;
}
