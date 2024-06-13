#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->listView_explorer, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(interactWithObject(QModelIndex)));
    connect(ui->pushButton_back, SIGNAL(clicked()), this, SLOT(changeDirectoryUp()));
    connect(ui->lineEdit_absolutePath, SIGNAL(returnPressed()), this, SLOT(jumpToLineEditPath()));
    connect(ui->pushButton_enter, SIGNAL(clicked()), this, SLOT(jumpToLineEditPath()));

    initializeListView();
    jumpToMyComputer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeListView()
{
    m_explorerModel = new QFileSystemModel(this);
    m_explorerModel->setRootPath("");
    ui->listView_explorer->setModel(m_explorerModel);
}

void MainWindow::jumpToMyComputer()
{
    m_currentPath = "";
    ui->lineEdit_absolutePath->setText(m_currentPath);
    QModelIndex index = m_explorerModel->setRootPath(m_currentPath);
    ui->listView_explorer->setRootIndex(index);
}

void MainWindow::jumpTo(const QString &path)
{
    if (path == ""){
        jumpToMyComputer();
        return;
    }
    QDir dir{path};
    if(!dir.exists()){
        QMessageBox::critical(this, "File Explorer", "Diretory do not exist", QMessageBox::Ok);
        ui->lineEdit_absolutePath->setText(m_currentPath);
        return;
    }
    m_currentPath = dir.absolutePath();
    ui->lineEdit_absolutePath->setText(m_currentPath);
    QModelIndex index = m_explorerModel->setRootPath(m_currentPath);
    ui->listView_explorer->setRootIndex(index);
}

void MainWindow::jumpToLineEditPath()
{
    jumpTo(ui->lineEdit_absolutePath->text());
}

void MainWindow::interactWithObject(const QModelIndex &index)
{
    QFileInfo fileInfo = m_explorerModel->fileInfo(index);
    if (fileInfo.isDir()){
        jumpTo(fileInfo.absoluteFilePath());
    }
    else if (fileInfo.isFile()){
        openFileByDefaultDesktopApp(fileInfo.absoluteFilePath());
    }
}

//BUG doesn't open a file/directory named on russian language
//or if directory have space
void MainWindow::openFileByDefaultDesktopApp(const QString &filePath)
{
    if(!QDesktopServices::openUrl(QUrl(filePath))){
        QMessageBox::critical(this, "File Explorer", "File dont open", QMessageBox::Ok);
        return;
    }
}

//TODO need clean
void MainWindow::changeDirectoryUp()
{
    //need to jump on Root folder
    if (m_currentPath == "C:/" || m_currentPath == "D:/"){ //TODO need a better template
        jumpToMyComputer();
    }
    //need to not loop cd
    //mb better block button
    if (m_currentPath == ""){
        return;
    }
    QDir dir{m_currentPath};
    dir.cdUp();
    jumpTo(dir.absolutePath());
}
