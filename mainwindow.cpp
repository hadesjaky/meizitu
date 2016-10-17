#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QByteArray>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    uiControl(false);
    pos = 0;
    statusLabel = new QLabel(ui->statusBar);

    manager = new QNetworkAccessManager(this);
    process = new QProcess(this);
    currentPicture = new QPixmap;

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    QString exe = QApplication::applicationDirPath().append("/pa.py");
    connect(process, SIGNAL(started()), this, SLOT(start_process()));
//    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(start_read_output()));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(start_read_err_output()));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finish_process(int, QProcess::ExitStatus)));
    if(!isFileExist(urlTxt)){
        process->start(exe);
        if (false == process->waitForStarted())
            ui->label->setText("爬虫没有执行，请检查脚本文件位置。");
    }
    else{
        readTxt();
        uiControl(true);
    }
}
void MainWindow::readTxt(){
    //读文件
    QFile file(urlTxt);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "文件不存在";
        return;
    }
    QTextStream stream(&file);
    QString line;
    while (!stream.atEnd()) {
        line = stream.readLine();
        if(line.isEmpty() || line.size() == 0 || "#" == line.at(0))
            continue;
        QStringList s = line.split('|');
        girlDict girl;
        girl.tag = s[0];
        girl.url = s[1];
        allGirl.append(girl);
    }
}

void MainWindow::uiControl(bool b){
    ui->label->setEnabled(b);
    ui->nextBtn->setEnabled(b);
    ui->preBtn->setEnabled(b);
}

void MainWindow::start_process()
{
    ui->label->setText("正在执行脚本文件...");
}
void MainWindow::start_read_output()
{
    QByteArray ba = process->readAllStandardOutput();
    qDebug(ba);
}
void MainWindow::start_read_err_output()
{
    QByteArray ba = process->readAllStandardError();
    qDebug()  << "error: "  << ba;
}
void MainWindow::finish_process(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit)
    {
        QMessageBox msgBox;
        msgBox.setText("脚本执行完成，生成url.txt");
        ui->label->setText("处理完成 ");
        msgBox.exec();
        readTxt();
        uiControl(true);
    }
    else
    {
        qDebug() << "process exit crash";
    }
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        //获取字节流构造 QPixmap 对象
        currentPicture->loadFromData(reply->readAll());
        //        QDateTime now;
        //        QString filename = now.currentDateTime().toString("yyMMddhhmmss.jpg");
        //        qDebug() << filename;
        //        currentPicture->save(filename);//保存图片
        //        currentFileName = filename;
        ui->label->setPixmap(*currentPicture);
        ui->label->update();
        adjustSize();
    }
    else
        qDebug() << reply->error();
}

bool MainWindow::isFileExist(QString _name){
    QFile file(_name);
    return file.exists();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_nextBtn_clicked()
{
    qDebug() << "next" << pos << allGirl.size();
    if(pos >= 0 ){
        if(!allGirl.isEmpty()){
            qDebug() <<allGirl[pos].url ;
            if(pos >= allGirl.size())
                return;
            manager->get(QNetworkRequest(QUrl(allGirl[pos].url)));
            statusLabel->setText(allGirl[pos].tag);
            statusLabel->adjustSize();
            //manager->get(QNetworkRequest(QUrl("http://pic.meizitu.com/wp-content/uploads/2016a/08/10/01.jpg")));
            pos +=1;
        }
    }
}

void MainWindow::on_preBtn_clicked()
{
    if(pos >= 0 ){
        if(!allGirl.isEmpty()){
            manager->get(QNetworkRequest(QUrl(allGirl[pos-1].url)));
            pos -=1;
            if(pos <= 0 )
                return;
        }
    }
}
