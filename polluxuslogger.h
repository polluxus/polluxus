#ifndef POLLUXUSLOGGER_H
#define POLLUXUSLOGGER_H

#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <QPlainTextEdit>



class PolluxusLogger : public QWidget
{
    Q_OBJECT
public:
    explicit PolluxusLogger(QWidget *parent = 0);
    virtual ~PolluxusLogger();
    void loadWorkSpace();
    void saveWorkSpace();

    QString iniFileString;
    QSettings *wsSettings;

private:
    QFile m_logFile;

    QToolBar *pToolBar;
    QPlainTextEdit *pMsgTextEdit;
    QPushButton *btnClear;

    void createToolBar();
    void createMsgTextEdit();


signals:

public slots:
    void onOrderUpdated(QString msg);

};

#endif // POLLUXUSLOGGER_H
