#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->listView_explorer, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(interactWithObject(QModelIndex)));
    connect(ui->pushButton_Back, SIGNAL(clicked()), this, SLOT(changeDirectoryUp()));

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
    QDir dir{path};
    m_currentPath = dir.absolutePath();
    ui->lineEdit_absolutePath->setText(m_currentPath);
    QModelIndex index = m_explorerModel->setRootPath(m_currentPath);
    ui->listView_explorer->setRootIndex(index);
}

void MainWindow::interactWithObject(const QModelIndex &index)
{
    QFileInfo fileInfo = m_explorerModel->fileInfo(index);
    if (fileInfo.isDir()){
        jumpTo(fileInfo.absoluteFilePath());
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
