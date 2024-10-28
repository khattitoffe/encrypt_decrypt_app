#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clickToSelectFile_clicked();

    void on_BackupDestination_clicked();

    void on_createBackup_clicked();

    void on_encryptButton_clicked();

    void on_DecryptSelectFile_clicked();

    void on_DecryptButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H


