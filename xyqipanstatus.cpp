#include "xyqipanstatus.h"
#include <QDebug>

XYQipanStatus::XYQipanStatus()
    :hongIsSmall(true)
{
    memset(qipan, 0, sizeof(TYPE) * 9 * 10);
}

XYQipanStatus::~XYQipanStatus()
{

}

bool XYQipanStatus::isOver()
{
    bool hong = false;
    bool hei = false;
    for (int row = 0; row <= 2; ++row)
    {
        for (int column = 3; column <= 5; ++column)
        {
            if (qipan[row][column] == TYPE(HONG_JIANG))
            {
                hong = true;
            }
            else if (qipan[row][column] == TYPE(HEI_JIANG))
            {
                hei = true;
            }
        }
    }

    for (int row = 7; row <= 9; ++row)
    {
        for (int column = 3; column <= 5; ++column)
        {
            if (qipan[row][column] == TYPE(HONG_JIANG))
            {
                hong = true;
            }
            else if (qipan[row][column] == TYPE(HEI_JIANG))
            {
                hei = true;
            }
        }
    }

    return !(hong && hei);
}

void XYQipanStatus::switchViews()
{
    TYPE temp[10][9];
    memcpy(temp, qipan, sizeof(TYPE) * 9 * 10);
    memset(qipan, 0, sizeof(TYPE) * 9 * 10);

    for (int row = 0; row < 10; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            if (temp[row][column] != TYPE(0))
            {
                QPoint switchPoint = XYQiziWidget::getSwitchViewsPos(QPoint(row, column));
                qipan[switchPoint.x()][switchPoint.y()] = temp[row][column];
            }
        }
    }
    allHongMovablePoints.clear();
    allHeiMovablePoints.clear();
    hongIsSmall = !hongIsSmall;
}

XYQipanStatus *XYQipanStatus::moveQizi(XYQipanStatus::TYPE qizi, int r, int c)
{
    XYQipanStatus *temp = new XYQipanStatus();
    temp->hongIsSmall = hongIsSmall;
    memcpy(temp->qipan, qipan, sizeof(TYPE) * 9 * 10);
    for (int row = 0; row < 10; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            if (temp->qipan[row][column] == qizi)
            {
                temp->qipan[row][column] = TYPE(0);
                temp->qipan[r][c] = qizi;
            }
        }
    }
    return temp;
}

QMap<int, QList<QPoint> > XYQipanStatus::getAllQiziMovablePoints(XYQiziWidget::SIDETYPE type)
{
    if (type == XYQiziWidget::RED && !allHongMovablePoints.isEmpty())
    {
        return allHongMovablePoints;
    }
    else if (type == XYQiziWidget::BLACK && !allHeiMovablePoints.isEmpty())
    {
        return allHeiMovablePoints;
    }

    for (int row = 0; row < 10; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            TYPE curType = qipan[row][column];
            if (curType >= HONG_ZU1 && curType <= HONG_JIANG)
            {
                allHongMovablePoints.insert(curType, getQiziMovablePoints(curType, row, column));
            }
            else if (curType != TYPE(0))
            {
                allHeiMovablePoints.insert(curType, getQiziMovablePoints(curType, row, column));
            }
        }
    }

    if (type == XYQiziWidget::RED)
    {
        return allHongMovablePoints;
    }
    else
    {
        return allHeiMovablePoints;
    }
}

QList<QPoint> XYQipanStatus::getQiziMovablePoints(XYQipanStatus::TYPE type) const
{
    QList<QPoint> allPoints;
    for (int row = 0; row < 10; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            if (qipan[row][column] == type)
            {
                allPoints = getQiziMovablePoints(type, row, column);
                return allPoints;
            }
        }
    }

    return allPoints;
}

QList<QPoint> XYQipanStatus::getQiziMovablePoints(TYPE type, int row, int column) const
{
    QList<QPoint> allPoints;

    switch (type)
    {
    case HONG_ZU1:
    case HONG_ZU2:
    case HONG_ZU3:
    case HONG_ZU4:
    case HONG_ZU5:
        if (hongIsSmall)
        {
            if (row >= 5)
            {
                allPoints.append(QPoint(row, column + 1));
                allPoints.append(QPoint(row, column - 1));
                allPoints.append(QPoint(row + 1, column));
            }
            else
            {
                allPoints.append(QPoint(row + 1, column));
            }
        }
        else
        {
            if (row >= 5)
            {
                allPoints.append(QPoint(row - 1, column));
            }
            else
            {
                allPoints.append(QPoint(row, column + 1));
                allPoints.append(QPoint(row, column - 1));
                allPoints.append(QPoint(row - 1, column));
            }
        }
        break;
    case HEI_ZU1:
    case HEI_ZU2:
    case HEI_ZU3:
    case HEI_ZU4:
    case HEI_ZU5:
        if (hongIsSmall)
        {
            if (row >= 5)
            {
                allPoints.append(QPoint(row + 1, column));
            }
            else
            {
                allPoints.append(QPoint(row, column + 1));
                allPoints.append(QPoint(row, column - 1));
                allPoints.append(QPoint(row + 1, column));
            }
        }
        else
        {
            if (row >= 5)
            {
                allPoints.append(QPoint(row, column + 1));
                allPoints.append(QPoint(row, column - 1));
                allPoints.append(QPoint(row - 1, column));
            }
            else
            {
                allPoints.append(QPoint(row - 1, column));
            }
        }
        break;
    case HONG_PAO1:
    case HONG_PAO2:
    case HEI_PAO1:
    case HEI_PAO2:
        {
            int qiziAddCount = 0;
            int qiziSubCount = 0;
            // y 正方向
            for (int i = 1; i < 9 && (row + i) <= 9; ++i)
            {
                int temRow = row + i;
                if (qipan[temRow][column] == 0)
                {
                    if (qiziAddCount == 0)
                    {
                        allPoints.append(QPoint(temRow, column));
                    }
                }
                else
                {
                    if (qiziAddCount == 1)
                    {
                        allPoints.append(QPoint(temRow, column));
                        break;
                    }
                    qiziAddCount++;
                }
            }
            // y 负方向
            qiziSubCount = 0;
            for (int i = 1; i < 9 && (row - i) >= 0; ++i)
            {
                int temRow = row - i;
                if (qipan[temRow][column] == 0)
                {
                    if (qiziSubCount == 0)
                    {
                        allPoints.append(QPoint(temRow, column));
                    }
                }
                else
                {
                    if (qiziSubCount == 1)
                    {
                        allPoints.append(QPoint(temRow, column));
                        break;
                    }
                    qiziSubCount++;
                }
            }

            // x 正方向
            qiziAddCount = 0;
            for (int i = 1; i < 8 && (column + i) <= 8; ++i)
            {
                int tmpColumn = column + i;
                if (qipan[row][tmpColumn] == 0)
                {
                    if (qiziAddCount == 0)
                    {
                        allPoints.append(QPoint(row, tmpColumn));
                    }
                }
                else
                {
                    if (qiziAddCount == 1)
                    {
                        allPoints.append(QPoint(row, tmpColumn));
                        break;
                    }
                    qiziAddCount++;
                }
            }
            // x 负方向
            qiziSubCount = 0;
            for (int i = 1; i < 8 && (column - i) >= 0; ++i)
            {
                int tmpColumn = column - i;
                if (qipan[row][tmpColumn] == 0)
                {
                    if (qiziSubCount == 0)
                    {
                        allPoints.append(QPoint(row, tmpColumn));
                    }
                }
                else
                {
                    if (qiziSubCount == 1)
                    {
                        allPoints.append(QPoint(row, tmpColumn));
                        break;
                    }
                    qiziSubCount++;
                }
            }
        }
        break;
    case HONG_CHE1:
    case HONG_CHE2:
    case HEI_CHE1:
    case HEI_CHE2:
        {
            // y 正方向
            for (int i = 1; i < 9 && (row + i) <= 9; ++i)
            {
                int temRow = row + i;
                if (qipan[temRow][column] == 0)
                {
                    allPoints.append(QPoint(temRow, column));
                }
                else
                {
                    allPoints.append(QPoint(temRow, column));
                    break;
                }
            }
            // y 负方向
            for (int i = 1; i < 9 && (row - i) >= 0; ++i)
            {
                int temRow = row - i;
                if (qipan[temRow][column] == 0)
                {
                    allPoints.append(QPoint(temRow, column));
                }
                else
                {
                    allPoints.append(QPoint(temRow, column));
                    break;
                }
            }

            // x 正方向
            for (int i = 1; i < 8 && (column + i) <= 8; ++i)
            {
                int tmpColumn = column + i;
                if (qipan[row][tmpColumn] == 0)
                {
                    allPoints.append(QPoint(row, tmpColumn));
                }
                else
                {
                    allPoints.append(QPoint(row, tmpColumn));
                    break;
                }
            }
            // x 负方向
            for (int i = 1; i < 8 && (column - i) >= 0; ++i)
            {
                int tmpColumn = column - i;
                if (qipan[row][tmpColumn] == 0)
                {
                    allPoints.append(QPoint(row, tmpColumn));
                }
                else
                {
                    allPoints.append(QPoint(row, tmpColumn));
                    break;
                }
            }
        }
        break;
    case HONG_MA1:
    case HONG_MA2:
    case HEI_MA1:
    case HEI_MA2:
        {
            // y 正方向
            if (row + 1 <= 9 && qipan[row + 1][column] == 0)
            {
                allPoints.append(QPoint(row + 2, column + 1));
                allPoints.append(QPoint(row + 2, column - 1));
            }

            // y 负方向
            if (row - 1 >= 0 && qipan[row - 1][column] == 0)
            {
                allPoints.append(QPoint(row - 2, column + 1));
                allPoints.append(QPoint(row - 2, column - 1));
            }

            // x 正方向
            if (column + 1 <= 8 && qipan[row][column + 1] == 0)
            {
                allPoints.append(QPoint(row + 1, column + 2));
                allPoints.append(QPoint(row - 1, column + 2));
            }

            // x 负方向
            if (column - 1 >= 0 && qipan[row][column - 1] == 0)
            {
                allPoints.append(QPoint(row + 1, column - 2));
                allPoints.append(QPoint(row - 1, column - 2));
            }
        }
        break;
    case HONG_XIANG1:
    case HONG_XIANG2:
    case HEI_XIANG1:
    case HEI_XIANG2:
        {
            // y 正方向
            if (row + 2 <= 9 && column + 2 <= 8 && qipan[row + 1][column + 1] == 0)
            {
                allPoints.append(QPoint(row + 2, column + 2));
            }
            if (row + 2 <= 9 && column - 2 >= 0 && qipan[row + 1][column - 1] == 0)
            {
                allPoints.append(QPoint(row + 2, column - 2));
            }

            // y 负方向
            if (row - 2 >= 0 && column + 2 <= 8 && qipan[row - 1][column + 1] == 0)
            {
                allPoints.append(QPoint(row - 2, column + 2));
            }
            if (row - 2 >= 0 && column - 2 >= 0 && qipan[row - 1][column - 1] == 0)
            {
                allPoints.append(QPoint(row - 2, column - 2));
            }
        }
        break;
    case HONG_SI1:
    case HONG_SI2:
        {
            // y 正方向
            allPoints.append(QPoint(row + 1, column + 1));
            allPoints.append(QPoint(row + 1, column - 1));

            // y 负方向
            allPoints.append(QPoint(row - 1, column + 1));
            allPoints.append(QPoint(row - 1, column - 1));

            int min = 0;
            int max = 0;
            if (hongIsSmall)
            {
                min = 0;
                max = 2;
            }
            else
            {
                min = 7;
                max = 9;
            }
            QList<QPoint>::iterator it = allPoints.begin();
            while (it != allPoints.end())
            {
                if ((*it).x() < min
                        || (*it).x() > max
                        || (*it).y() < 3
                        || (*it).y() > 5)
                {
                    it = allPoints.erase(it);
                    continue;
                }
                ++it;
            }
        }
        break;
    case HEI_SI1:
    case HEI_SI2:
        {
            // y 正方向
            allPoints.append(QPoint(row + 1, column + 1));
            allPoints.append(QPoint(row + 1, column - 1));

            // y 负方向
            allPoints.append(QPoint(row - 1, column + 1));
            allPoints.append(QPoint(row - 1, column - 1));

            int min = 0;
            int max = 0;
            if (hongIsSmall)
            {
                min = 7;
                max = 9;
            }
            else
            {
                min = 0;
                max = 2;
            }
            QList<QPoint>::iterator it = allPoints.begin();
            while (it != allPoints.end())
            {
                if ((*it).x() < min
                        || (*it).x() > max
                        || (*it).y() < 3
                        || (*it).y() > 5)
                {
                    it = allPoints.erase(it);
                    continue;
                }
                ++it;
            }
        }
        break;
    case HONG_JIANG:
        {
            // y 不变
            allPoints.append(QPoint(row, column + 1));
            allPoints.append(QPoint(row, column - 1));

            // x 不变
            allPoints.append(QPoint(row + 1, column));
            allPoints.append(QPoint(row - 1, column));

            int min = 0;
            int max = 0;
            if (hongIsSmall)
            {
                min = 0;
                max = 2;
            }
            else
            {
                min = 7;
                max = 9;
            }
            QList<QPoint>::iterator it = allPoints.begin();
            while (it != allPoints.end())
            {
                if ((*it).x() < min
                        || (*it).x() > max
                        || (*it).y() < 3
                        || (*it).y() > 5)
                {
                    it = allPoints.erase(it);
                    continue;
                }
                ++it;
            }
        }
        break;
    case HEI_JIANG:
        {
            // y 不变
            allPoints.append(QPoint(row, column + 1));
            allPoints.append(QPoint(row, column - 1));

            // x 不变
            allPoints.append(QPoint(row + 1, column));
            allPoints.append(QPoint(row - 1, column));

            int min = 0;
            int max = 0;
            if (hongIsSmall)
            {
                min = 7;
                max = 9;
            }
            else
            {
                min = 0;
                max = 2;
            }
            QList<QPoint>::iterator it = allPoints.begin();
            while (it != allPoints.end())
            {
                if ((*it).x() < min
                        || (*it).x() > max
                        || (*it).y() < 3
                        || (*it).y() > 5)
                {
                    it = allPoints.erase(it);
                    continue;
                }
                ++it;
            }
        }
        break;
    }

    // 筛选掉不满足的
    QList<QPoint>::iterator it = allPoints.begin();
    while (it != allPoints.end())
    {
        // 去掉出界的位置
        if ((*it).x() < 0
                || (*it).x() > 9
                || (*it).y() < 0
                || (*it).y() > 8)
        {
            it = allPoints.erase(it);
            continue;
        }
        // 去掉是己方棋子的位置
        if (qipan[(*it).x()][(*it).y()] != 0)
        {
            if ((qipan[(*it).x()][(*it).y()] <= HONG_JIANG
                    && type <= HONG_JIANG)
                    || (qipan[(*it).x()][(*it).y()] > HONG_JIANG
                        && type > HONG_JIANG))
            {
                it = allPoints.erase(it);
                continue;
            }
        }
        ++it;
    }

    return allPoints;
}

QMap<int, QList<QPoint> > XYQipanStatus::getAddedPoints(XYQipanStatus *other, XYQiziWidget::SIDETYPE type)
{
    QMap<int, QList<QPoint> > addedPoints;
    QMap<int, QList<QPoint> > thisPoints = this->getAllQiziMovablePoints(type);
    QMap<int, QList<QPoint> > otherPoints = other->getAllQiziMovablePoints(type);

    QMap<int, QList<QPoint> >::iterator it = thisPoints.begin();
    QMap<int, QList<QPoint> >::iterator find;
    while (it != thisPoints.end())
    {
        find = otherPoints.find(it.key());
        if (find == otherPoints.end())
        {
            addedPoints.insert(it.key(), it.value());
        }
        else
        {
            QList<QPoint> source = it.value();
            QList<QPoint>::iterator temp_it = source.begin();
            while (temp_it != source.end())
            {
                if (find.value().contains(*temp_it))
                {
                    temp_it = source.erase(temp_it);
                    continue;
                }
                ++temp_it;
            }
            if (!source.isEmpty())
            {
                addedPoints.insert(it.key(), source);
            }
        }
        ++it;
    }

    return addedPoints;
}

QMap<int, QList<QPoint> > XYQipanStatus::getReducedPoints(XYQipanStatus *other, XYQiziWidget::SIDETYPE type)
{
    QMap<int, QList<QPoint> > reducedPoints;
    QMap<int, QList<QPoint> > thisPoints = this->getAllQiziMovablePoints(type);
    QMap<int, QList<QPoint> > otherPoints = other->getAllQiziMovablePoints(type);

    QMap<int, QList<QPoint> >::iterator it = otherPoints.begin();
    QMap<int, QList<QPoint> >::iterator find;
    while (it != otherPoints.end())
    {
        find = thisPoints.find(it.key());
        if (find == thisPoints.end())
        {
            reducedPoints.insert(it.key(), it.value());
        }
        else
        {
            QList<QPoint> source = it.value();
            QList<QPoint>::iterator temp_it = source.begin();
            while (temp_it != source.end())
            {
                if (find.value().contains(*temp_it))
                {
                    temp_it = source.erase(temp_it);
                    continue;
                }
                ++temp_it;
            }
            if (!source.isEmpty())
            {
                reducedPoints.insert(it.key(), source);
            }
        }
        ++it;
    }

    return reducedPoints;
}
