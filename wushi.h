#ifndef WUSHI_H
#define WUSHI_H

#include <QPoint>

void Startup(void);
bool getPos(QPoint &src, QPoint &tar);
bool wushiMoveQizi(const QPoint &tar);
void changeSide(int red);
void AIMove(void);
void InitWithXYQipan(int *qipan, int aiSide);
#endif // WUSHI_H

