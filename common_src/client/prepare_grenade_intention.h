#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "intention.h"

class IntentionPrepareGrenade: public Intention {
private:
    int id;
public:
    explicit IntentionPrepareGrenade(int id);
    int get_intention() override;
};
