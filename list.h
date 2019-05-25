#ifndef LIST_H
#define LIST_H

#include <QWidget>
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <vector>
#include "processlist.h"
#define sz_type unsigned int

namespace Ui {
class List;
}

class List : public QWidget
{
    Q_OBJECT

public:
    explicit List(QWidget *parent = nullptr);
    //用于
    void getWatchList();
    void showWatchList();
    //void refreshWatchList();
    void firstShowWatchList();

    void getListInfo(QString &listInfo);
    void showListInfo(QString listInfo);
    //void refreshListInfo();
    void updateListData();
    void deleteSeleted();
    int  deleteRow(int row);
    void deleteFromWatchList(int index);

    void double2char(char str[],double dnum);

    ~List();
private:
    Ui::List *ui;
    vector<struct Process> watchList;
    QTimer *timer;
private slots:
    void refreshWatchList();
    void refreshListInfo();
    void on_deleteBtn_clicked();
    void on_stopBtn_clicked();
};

#endif // LIST_H
