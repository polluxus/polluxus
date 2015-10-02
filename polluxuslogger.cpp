#include "polluxuslogger.h"

PolluxusLogger::PolluxusLogger(QWidget *parent) : QWidget(parent)
{

    iniFileString = QDir::currentPath() + "/workspace.ini";
    wsSettings = new QSettings(iniFileString, QSettings::IniFormat);

    setWindowFlags(Qt::Window);
    setWindowTitle("Log");

    createToolBar();
    createMsgTextEdit();

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    vLayout->setContentsMargins(0,0,0,0);

    vLayout->addWidget(pToolBar);
    vLayout->addWidget(pMsgTextEdit);

    setLayout(vLayout);
    setMinimumSize(320, 200);

    loadWorkSpace();

}

PolluxusLogger::~PolluxusLogger()
{
    saveWorkSpace();
    if(!wsSettings) delete wsSettings;
}


void PolluxusLogger::createToolBar()
{
    pToolBar = new QToolBar;
    btnClear = new QPushButton(tr("Clear"));
    pToolBar->addWidget(btnClear);
}

void PolluxusLogger::createMsgTextEdit()
{
    pMsgTextEdit = new QPlainTextEdit;
    pMsgTextEdit->setReadOnly(true);
    pMsgTextEdit->setWordWrapMode(QTextOption::NoWrap);
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready------------------");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready-------------------------------");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");
    pMsgTextEdit->appendPlainText("We are ready, we are ready, we are ready, we are ready");

}


void PolluxusLogger::onOrderUpdated(QString msg)
{
    pMsgTextEdit->appendPlainText(msg);
}

void PolluxusLogger::saveWorkSpace()
{
    qDebug() << "PolluxusLogger::saveWorkSpace";

    wsSettings->beginGroup("polluxuslogger");
    wsSettings->setValue("geometry", saveGeometry());
    wsSettings->setValue( "maximized", isMaximized());
    if ( !isMaximized() ) {
            wsSettings->setValue( "pos", pos() + QPoint(0, 8) );
            wsSettings->setValue( "size", size() );
        }
    wsSettings->endGroup();

    wsSettings->sync();
}

void PolluxusLogger::loadWorkSpace()
{
    qDebug() << "loadWorkSpace";

    wsSettings->beginGroup("polluxuslogger");
    restoreGeometry(wsSettings->value( "geometry", saveGeometry() ).toByteArray());
    move(wsSettings->value( "pos", pos() ).toPoint());
    resize(wsSettings->value( "size", size()).toSize());
    if ( wsSettings->value( "maximized", isMaximized() ).toBool() )
    {
        showMaximized();
    }
    wsSettings->endGroup();
}
