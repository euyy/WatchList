#include "list.h"
#include "ui_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <unistd.h>
string name(int i){
    switch (i) {
    case 0:return "aaa";
    case 1:return "aaa";
    case 2:return "abc";
    case 3:return "bbb";
    case 4:return "bcd";
    case 5:return "fuc";
    case 6:return "i";
    case 7:return "have";
    case 8:return "an";
    case 9:return "apple";
    default:return "error";
    }
}

string name1(int i){
    char ch = i + 'A';
    string res;
    res.push_back(ch);
    return res;
}


int saveWatchList(vector<Process>watchList)
{
    FILE * file;
    file=fopen("E:\\ProgramData\\QT\\myTestList_1\\watchlist.txt","w");
    for(size_t i = 0; i < watchList.size(); i++)
    {
        fprintf(file,"%d\n",watchList[i].pid);
    }
    fclose(file);
    return 0;
}

int openWatchList(vector<Process> &watchList)
{
    FILE * file;
    file=fopen("E:\\ProgramData\\QT\\myTestList_1\\watchList.txt","r");
    if(file == NULL){
        printf("Open file failed!\n");
    }
    int pid=0;
    watchList.clear();
    srand(time(0));
    while(fscanf(file,"%d",&pid)!=EOF)
    {
        Process proc;
        proc.pid=pid;
        proc.comm = name(pid);
        proc.task_state = name1(pid + rand()%15);
        proc.cpuUsage=rand()%100+rand()%10/10.0;
        proc.memoryUsage=rand()%100+rand()%10/10.0;
        watchList.push_back(proc);
    }

    fclose(file);
    return 0;
}


int updateWatchList(vector<Process>&watchList,vector<Process>&emailList)
{
    emailList.clear();
    for(size_t i = 0; i < watchList.size(); i++)
    {
        Process proc;
        proc.pid=watchList[i].pid;

        proc.cpuUsage=(double)(proc.cpuTime - watchList[i].cpuTime) /
               (double)(proc.cpuTotalTime - watchList[i].cpuTotalTime);
        if(watchList[i].task_state!=proc.task_state)
        {
            emailList.push_back(watchList[i]);
        }
        watchList[i]=proc;
    }

    return 0;
}


//void openWatchList(vector<struct Process> &WatchList){
//    srand(time(0));
//    for(int j=0;j<10;j++){
//        int i =rand()%100;
//        struct Process oneProcess;
//        oneProcess.pid=i;

//        oneProcess.comm = name(j);
//        oneProcess.task_state = std::to_string(i*i);
//        oneProcess.cpuUsage=50+i;
//        oneProcess.memoryUsage=51.5+i;
//        WatchList.push_back(oneProcess);
//    }
//}


List::List(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::List)
{
    ui->setupUi(this);

    //设置默认列宽
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,100);
    ui->tableWidget->setColumnWidth(3,100);
    //隐藏行表头
    ui->tableWidget->verticalHeader()->setVisible(false);
    //设置内容不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置背景交替颜色
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    ui->deleteBtn->setEnabled(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    firstShowWatchList();

    timer = new QTimer;
    timer->start(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshWatchList()));
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshListInfo()));

}

void List::firstShowWatchList(){
    getWatchList();
    showWatchList();
}

void List::getWatchList(){
    vector<struct Process> nowWatchList;
    openWatchList(nowWatchList);
    watchList = nowWatchList;
}

void List::showWatchList()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setSortingEnabled(false);
    sz_type row = watchList.size();

    ui->tableWidget->setRowCount(int(row));

    double cpulevel1=70,cpulevel2=100,cpulevel3=150;

    for(sz_type i = 0;i < row;i++){

       // item->setData(Qt::EditRole, QVariant(num));
        QTableWidgetItem *processName = new QTableWidgetItem();
        processName->setText(watchList[i].comm.c_str());
        ui->tableWidget->setItem(int(i), 0, processName);

        QTableWidgetItem *processID = new QTableWidgetItem();
        processID->setData(Qt::EditRole, QVariant(watchList[i].pid));
        ui->tableWidget->setItem(int(i), 1, processID);
        ui->tableWidget->item(int(i),1)->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *taskState = new QTableWidgetItem();
        taskState->setText(watchList[i].task_state.c_str());
        ui->tableWidget->setItem(int(i), 2, taskState);
        ui->tableWidget->item(int(i),2)->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *cpuUsage = new QTableWidgetItem();
        cpuUsage->setData(Qt::EditRole, QVariant(watchList[i].cpuUsage));
        ui->tableWidget->setItem(int(i), 3, cpuUsage);
        ui->tableWidget->item(int(i),3)->setTextAlignment(Qt::AlignCenter);

        if(watchList[i].cpuUsage < cpulevel1){
            ui->tableWidget->item(int(i),3)->setBackgroundColor(QColor(255,244,196));
        }
        else if(watchList[i].cpuUsage < cpulevel2){
            ui->tableWidget->item(int(i),3)->setBackgroundColor(QColor(249,236,168));
        }
        else if(watchList[i].cpuUsage < cpulevel3){
            ui->tableWidget->item(int(i),3)->setBackgroundColor(QColor(255,228,135));
        }
        else {
            ui->tableWidget->item(int(i),3)->setBackgroundColor(QColor(255,198,61));
        }

        QTableWidgetItem *memoryUsage = new QTableWidgetItem();
        memoryUsage->setData(Qt::EditRole, QVariant(watchList[i].memoryUsage));
        ui->tableWidget->setItem(int(i), 4, memoryUsage);
        ui->tableWidget->item(int(i),4)->setTextAlignment(Qt::AlignCenter);

        if(watchList[i].memoryUsage < cpulevel1){
            ui->tableWidget->item(int(i),4)->setBackgroundColor(QColor(255,244,196));
        }
        else if(watchList[i].memoryUsage < cpulevel2){
            ui->tableWidget->item(int(i),4)->setBackgroundColor(QColor(249,236,168));
        }
        else if(watchList[i].memoryUsage < cpulevel3){
            ui->tableWidget->item(int(i),4)->setBackgroundColor(QColor(255,228,135));
        }
        else {
            ui->tableWidget->item(int(i),4)->setBackgroundColor(QColor(255,198,61));
        }
    }
    ui->tableWidget->setSortingEnabled(true);
}

void List::refreshWatchList(){
    vector<struct Process> emailList;
    updateWatchList(watchList,emailList);
    showWatchList();
    //如果监视列表进程状态无变化，不操作
    //如果监视列表进程状态变化，发邮件
    if(emailList.empty()){
        return;
    }
    else{
        //对每一个进程
        //首先把邮件内容保存到文件中，
        //然后进行发邮件操作
    }
}


void List::updateListData(){

}


void List::getListInfo(QString &listInfo){
//    QModelIndexList indexList1 = ui->tableWidget->selectionModel()->selectedIndexes();
//    int count = indexList1.size();
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    int count = items.count()/ui->tableWidget->columnCount();
    QString WatchProcessCount = "Process count: " + QString::number(watchList.size());
    QString SeletedProcessCount = "Seleted Process Count: " + QString::number(count);
    QString ListState;
    QString text = ui->stopBtn->text();
    QString stop = "Stop";
    if(text == stop){
        ListState = "The Watch List is refreshing";
    }
    else {
        ListState = "The Watch List has stopped refresh";
    }
    listInfo = WatchProcessCount + "   |   " + SeletedProcessCount + "   |   " + ListState;
}

void List::showListInfo(QString listInfo){
    ui->listInfo->setText(listInfo);
}

void List::refreshListInfo(){
    QString listInfo;
    getListInfo(listInfo);
    showListInfo(listInfo);
}

//void List::refreshList(){
//    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
//    refreshWatchList();
//    refreshListInfo();
//}

//void List::refreshListInfo(){
//    QString listInfo;
//    getListInfo(listInfo);
//    showListInfo(listInfo);
//}


void List::deleteSeleted()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    int count = items.count();
    for(int i = 0;i < count;i++){
        int index = items.at(i)->row();
        deleteFromWatchList(index);
        deleteRow(index);
    }
}

int List::deleteRow(int row){
    if (row != -1){
         ui->tableWidget->removeRow(row);
         return 0;
    }
    else {
        return 1;
    }
}

void List::deleteFromWatchList(int index){
    if (index != -1){
        bool ok;
        int pid = ui->tableWidget->item(index,1)->text().toInt(&ok,10);
        int cnt = watchList.size();
        for(int i = 0;i < cnt;i++){
            if(watchList[i].pid == pid){
                watchList.erase(watchList.begin()+i);
                return;
            }
        }
        printf("Delete pid %d error!\n",pid);
    }
    else printf("Index %d does not exist!\n",index);
}


void List::on_deleteBtn_clicked()
{
    deleteSeleted();
    saveWatchList(watchList);
    refreshWatchList();
    refreshListInfo();
}

List::~List()
{
    delete ui;
}

void List::on_stopBtn_clicked()
{
    QString str = "Stop";
    QString text = ui->stopBtn->text();
    if(str == text){
        ui->stopBtn->setText(QApplication::translate("List", "Refresh", nullptr));
        //整行选中的方式
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        //多个选中
        ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        disconnect(timer, SIGNAL(timeout()), this, SLOT(refreshList()));
        connect(timer, SIGNAL(timeout()), this, SLOT(refresh_List_Info()));

        ui->deleteBtn->setEnabled(true);
    }
    else {
        ui->stopBtn->setText(QApplication::translate("List", "Stop", nullptr));
        ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        connect(timer, SIGNAL(timeout()), this, SLOT(refreshList()));
        ui->deleteBtn->setEnabled(false);
    }
}
