#include "menu.h"

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainMenu window;

    window.move(300, 300);
    window.setWindowTitle("Pràctica OpenMP");
    window.show();

    return app.exec();
}
