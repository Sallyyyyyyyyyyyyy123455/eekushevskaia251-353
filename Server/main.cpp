#include "server.h"

int main() {
    initDatabase();
    Server server(8080);
    server.start();
    return 0;
}
