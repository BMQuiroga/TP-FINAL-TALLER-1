#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <tuple>
#include "intention.h"

class IntentionMove: public Intention {
private:
    const int x;
    const int y;
    const int id;
public:
    explicit IntentionMove(
        const int x,
        const int y,
        const int id);
    int get_intention() override;
    std::tuple<int, int> get_direction();
};
