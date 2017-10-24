#ifndef WUSHI_H
#define WUSHI_H

#include <QPoint>

void Startup(void);
bool getPos(QPoint &src, QPoint &tar);
bool wushiMoveQizi(const QPoint &tar);
#endif // WUSHI_H

