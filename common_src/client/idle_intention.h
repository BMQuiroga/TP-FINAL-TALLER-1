#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "intention.h"

class IntentionIdle: public Intention {
private:
    int id;
public:
    explicit IntentionIdle(int id);
    int get_intention() override;
};
