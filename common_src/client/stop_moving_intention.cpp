#include "stop_moving_intention.h"
#include <tuple>

IntentionStopMoving::IntentionStopMoving(
    const int x, const int y, const int id):
    x(x),
    y(y),
    id(id)
{
}

std::tuple<int, int> IntentionStopMoving::get_direction() {
    return std::make_tuple(this->x, this->y);
}

int IntentionStopMoving::get_intention(){
    return id;
}
