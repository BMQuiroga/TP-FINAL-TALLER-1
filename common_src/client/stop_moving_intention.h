#include <iostream>
#include <fstream>
#include <cstring>
#include <tuple>
#include <string>
#include "intention.h"

class IntentionStopMoving: public Intention {
private:
    const int x;
    const int y;
    const int id;
public:
    explicit IntentionStopMoving(
        const int x,
        const int y,
        const int id);
    int get_intention() override;
    std::tuple<int, int> get_direction();
};
