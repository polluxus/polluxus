#ifndef ORDERMANAGERVIEW_H
#define ORDERMANAGERVIEW_H

#include <QWidget>
#include "ordermanagermodel.h"

class QTableView;
class QSettings;

class OrderManagerView : public QWidget
{
    Q_OBJECT
public:
    explicit OrderManagerView(QWidget *parent = 0);
    virtual ~OrderManagerView();

    QTableView *pTableView;
    OrderManagerModel *pModel;

    void loadWorkSpace();
    void saveWorkSpace();

    QString iniFileString;
    QSettings *wsSettings;

signals:

public slots:
};

#endif // ORDERMANAGERVIEW_H
