#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "intention.h"

class IntentionReload: public Intention {
private:
    int id;
public:
    explicit IntentionReload(int id);
    int get_intention() override;
};
