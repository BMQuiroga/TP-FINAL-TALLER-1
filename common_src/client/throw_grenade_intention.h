#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "intention.h"

class IntentionThrowGrenade: public Intention {
private:
    int id;
public:
    explicit IntentionThrowGrenade(int id);
    int get_intention() override;
};
