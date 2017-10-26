#ifndef WUSHI_H
#define WUSHI_H

#include <QPoint>

void Startup(void);
bool getPos(QPoint &src, QPoint &tar);
void changeSide(int red);
void AIMove(void);
void InitWithXYQipan(int *qipan, int aiSide);
void setAISearchDep(int dep);
#endif // WUSHI_H

