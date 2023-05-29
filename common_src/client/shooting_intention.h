#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "intention.h"

class IntentionShooting: public Intention {
private:
    bool is_shooting;
    int id;
public:
    explicit IntentionShooting(
            bool is_shooting, int id);
    int get_intention() override;
};
