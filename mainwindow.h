#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QDir>

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
    void interactWithObject(const QModelIndex &index);
    void changeDirectoryUp();
    void jumpToLineEditPath();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_explorerModel;
    QString m_currentPath;

    void initializeListView();
    void jumpToMyComputer();
    void jumpTo(const QString &path);
    void openFileByDefaultDesktopApp(const QString &filePath);
};
#endif // MAINWINDOW_H
