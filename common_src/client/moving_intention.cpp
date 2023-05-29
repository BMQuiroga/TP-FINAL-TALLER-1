#include "moving_intention.h"
#include <tuple>

IntentionMove::IntentionMove(
    const int x, const int y, const int id):
    x(x),
    y(y),
    id(id)
{
}

std::tuple<int, int> IntentionMove::get_direction() {
    return std::make_tuple(this->x, this->y);
}

int IntentionMove::get_intention(){
    return id;
}
