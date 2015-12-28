
// QT
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

// PROJECT
#include "mainWindow.h"     // Class implementation
#include "ui_mainWindow.h"  // UI


MainWindow::MainWindow(QWidget *parent /*=0*/)
	: QMainWindow(parent)
{
	m_ui = new Ui::MainWindow;
	m_ui->setupUi(this);
}

MainWindow::~MainWindow()
{	
    delete m_ui;
}
