#ifdef guard
#include "main.h"

int main() {
    try {
        MenuSystem app;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown fatal error occurred." << std::endl;
        return 1;
    }

    return 0;
}
#endif