#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QPixmap>
#include <QDateTime>
#include <QMovie>
#include <QProcess>
#include <QFile>
#include  <QTextStream>
#include <QList>
#include <QLabel>

const QString urlTxt = "url.txt";

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void readTxt();

public slots:
    void replyFinished(QNetworkReply *reply);

    void start_process();
    void start_read_output();
    void start_read_err_output();
    void finish_process(int exitCode, QProcess::ExitStatus exitStatus);

    bool isFileExist(QString _name);//文件是否存在
    void uiControl(bool b);

private slots:
    void on_nextBtn_clicked();

    void on_preBtn_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap *currentPicture;
    QString currentFileName;
    QNetworkAccessManager *manager;
    QProcess *process ;

    struct girlDict{
        QString tag;
        QString url;
    };

    QList<girlDict> allGirl;
    int pos;
    QLabel *statusLabel;

};

#endif // MAINWINDOW_H
