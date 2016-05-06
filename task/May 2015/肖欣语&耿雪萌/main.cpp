/*#include "llkwin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LLKWin w;
    w.show();

    return a.exec();
}
*/

#include <QApplication>
#include "llkwin.h"
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    LLKWin *win = new LLKWin();
    win->show();
    return app.exec();
}
