#include "draw.h"
#include "client.h"
#include "egg_ui.h"
int main() {
    if (port_init()) {
        return 1;
    }
    return 0;
}