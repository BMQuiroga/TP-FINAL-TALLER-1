#include "reload_intention.h"

IntentionReload::IntentionReload(int id):
    id(id)
{
}

int IntentionReload::get_intention()
{
    return id;
}
