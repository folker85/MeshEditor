
 // QT
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

 // VTK
#include <vtkRenderWindow.h>
#include <vtkOBJReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

// PROJECT
#include "mainWindow.h"     // Class implementation
#include "ui_mainWindow.h"  // UI


MainWindow::MainWindow(QWidget *parent /*=0*/)
	: QMainWindow(parent)
{
	m_ui = new Ui::MainWindow;
	m_ui->setupUi(this);

	m_renderer = vtkSmartPointer<vtkRenderer>::New();
 	m_renderer->SetBackground(0.1, 0.2, 0.3);
 
 	m_ui->mainRenderWindow->GetRenderWindow()->AddRenderer(m_renderer);

	initConnections();
}

MainWindow::~MainWindow()
{	
    delete m_ui;
}

void MainWindow::onLoadMesh()
{
    const QString fullMeshName = QFileDialog::getOpenFileName();
 
 	if (fullMeshName.isEmpty())
 		return;
 
 	vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
 	reader->SetFileName(fullMeshName.toStdString().c_str());
 	reader->Update();
 
 	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());
  
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
 
 	m_renderer->AddActor(actor);
 
 	m_renderer->ResetCamera();
 	m_ui->mainRenderWindow->update();
}

void MainWindow::initConnections()
{
	connect(m_ui->loadMeshBtn, SIGNAL(clicked()), this, SLOT(onLoadMesh()));
}
