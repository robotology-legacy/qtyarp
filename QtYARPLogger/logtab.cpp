#include "logtab.h"
#include "ui_logtab.h"

LogTab::LogTab(yarp::os::YarprunLogger::LoggerEngine*  _theLogger, MessageWidget* _system_message, std::string _portName, QWidget *parent, int refreshRate) :
    QFrame(parent),
    ui(new Ui::LogTab)
{ 
    system_message = _system_message;
    theLogger= _theLogger;
    portName =_portName;
    displayYarprunTimestamp_enabled=true;
    displayLocalTimestamp_enabled=true;
    displayColors_enabled=true;
    displayGrid_enabled=true;
    displayErrorLevel_enabled=true;
    ui->setupUi(this);
    model_logs = new QStandardItemModel();
    proxyModelButtons = new LogSortFilterProxyModel(this);
    proxyModelSearch = new LogSortFilterProxyModel(this);
#define USE_FILTERS 1
#if USE_FILTERS
    proxyModelButtons->setSourceModel(model_logs);
    proxyModelSearch->setSourceModel(proxyModelButtons);
    proxyModelSearch->setFilterKeyColumn(-1); 
    ui->listView->setModel(proxyModelSearch);
#else
    ui->listView->setModel(model_logs);
#endif
    ui->listView->verticalHeader()->setVisible(false);
    ui->listView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->listView->setAutoScroll(true);

    logTimer = new QTimer(this);
    connect(logTimer, SIGNAL(timeout()), this, SLOT(updateLog()));
    logTimer->start(refreshRate);

    model_logs->setHorizontalHeaderItem(0,new QStandardItem("yarprun timestamp"));
    model_logs->setHorizontalHeaderItem(1,new QStandardItem("local timestamp"));
    model_logs->setHorizontalHeaderItem(2,new QStandardItem("level"));
    model_logs->setHorizontalHeaderItem(3,new QStandardItem("message"));
    ui->listView->setColumnWidth(0,120);
    ui->listView->setColumnWidth(1,120);
    ui->listView->setColumnWidth(2,120);
    ui->listView->setColumnWidth(3,100);
    ui->listView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->listView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->listView->verticalHeader()->setDefaultSectionSize(20);
    
    updateLog(true);
}

LogTab::~LogTab()
{
    delete logTimer;
    delete model_logs;
    delete proxyModelButtons;
    delete proxyModelSearch;
    delete ui;
}

void LogTab::clear_model_logs()
{
    mutex.lock();
    if (model_logs) model_logs->removeRows(0,model_logs->rowCount());
    mutex.unlock();
}

void LogTab::updateLog(bool from_beginning)
{
    /*
    //@@@@ performance test, to be removed later
    static double to=yarp::os::Time::now();
    static double prev;
    double tt = yarp::os::Time::now()-to;
    char buf [50];
    double tt3 = tt-prev;
    prev = tt;
    sprintf(buf,"%3.5f %3.5f %3.5f",tt, tt3);
    system_message->addMessage(QString(buf),LOGWIDGET_WARNING);
    */

    mutex.lock();
    std::list<yarp::os::YarprunLogger::MessageEntry> messages;
    this->theLogger->get_messages_by_port_complete(portName,messages, from_beginning);
    int size_messages= messages.size();
    std::list<yarp::os::YarprunLogger::MessageEntry>::iterator it;
    QStandardItem *rootNode = model_logs->invisibleRootItem();
    for (it=messages.begin(); it!=messages.end(); it++)
    {
        QList<QStandardItem *> rowItem;
        QColor rowcolor = QColor(Qt::white);
        std:: string error_level;
        if      (it->level==yarp::os::YarprunLogger::LOGLEVEL_ERROR)     { rowcolor = QColor("#FF7070");  error_level=ERROR_STRING;}
        else if (it->level==yarp::os::YarprunLogger::LOGLEVEL_WARNING)   { rowcolor = QColor("#FFFF70");  error_level=WARNING_STRING; }
        else if (it->level==yarp::os::YarprunLogger::LOGLEVEL_INFO)      { rowcolor = QColor("#70FF70");  error_level=INFO_STRING; }
        else if (it->level==yarp::os::YarprunLogger::LOGLEVEL_DEBUG)     { rowcolor = QColor("#7070FF");  error_level=DEBUG_STRING;}
        else if (it->level==yarp::os::YarprunLogger::LOGLEVEL_UNDEFINED) { rowcolor = QColor(Qt::white);  error_level="";     }
        else                                                             { rowcolor = QColor(Qt::white);  error_level="";     }

        //using numbers seems not to work. Hence I'm using strings.
        rowItem << new QStandardItem(it->yarprun_timestamp.c_str()) << new QStandardItem(it->local_timestamp.c_str()) << new QStandardItem(error_level.c_str()) << new QStandardItem(it->text.c_str());

        if (displayColors_enabled)
        {
            for (QList<QStandardItem *>::iterator col_it = rowItem.begin(); col_it != rowItem.end(); col_it++)
            {
                (*col_it)->setBackground(rowcolor);
            }
        }
        rootNode->appendRow(rowItem);
    }
    ui->listView->setColumnHidden(0,!displayYarprunTimestamp_enabled);
    ui->listView->setColumnHidden(1,!displayLocalTimestamp_enabled);
    ui->listView->setColumnHidden(2,!displayErrorLevel_enabled);
    ui->listView->setShowGrid(displayGrid_enabled);
    mutex.unlock();
}

void LogTab::displayYarprunTimestamp  (bool enabled)
{
    displayYarprunTimestamp_enabled=enabled;
    ui->listView->setColumnHidden(0,!displayYarprunTimestamp_enabled);
}

void LogTab::displayLocalTimestamp  (bool enabled)
{
    displayLocalTimestamp_enabled=enabled;
    ui->listView->setColumnHidden(1,!displayLocalTimestamp_enabled);
}

void LogTab::displayErrorLevel (bool enabled)
{
    displayErrorLevel_enabled=enabled;
    ui->listView->setColumnHidden(2,!displayErrorLevel_enabled);
}

void LogTab::displayColors     (bool enabled)
{
    displayColors_enabled=enabled;
    //ui->listView->
}

void LogTab::displayGrid       (bool enabled)
{
    displayGrid_enabled=enabled;
    ui->listView->setShowGrid(displayGrid_enabled);
}
