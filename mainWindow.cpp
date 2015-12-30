
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
#include <vtkAppendPolyData.h>
#include <vtkProperty.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPointData.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkIdTypeArray.h>

// PROJECT
#include "mainWindow.h"     // Class implementation
#include "ui_mainWindow.h"  // UI

#include <map>


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
 
	vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
 	//vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
 	reader->SetFileName(fullMeshName.toStdString().c_str());
 	reader->Update();

	vtkSmartPointer<vtkPolyData> mesh = vtkSmartPointer<vtkPolyData>::New();
	mesh->DeepCopy(reader->GetOutput());
	m_meshes.push_back(mesh);
 
 	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(mesh);
  
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

	m_actors.push_back(actor);
 
 	m_renderer->AddActor(actor);
 
 	m_renderer->ResetCamera();
 	m_ui->mainRenderWindow->update();
}

void MainWindow::initConnections()
{
	connect(m_ui->loadMeshBtn, SIGNAL(clicked()), this, SLOT(onLoadMesh()));
	connect(m_ui->appendBtn, SIGNAL(clicked()), this, SLOT(onAppendMeshes()));
	connect(m_ui->splitBtn, SIGNAL(clicked()), this, SLOT(onSplitMesh()));
}

void MainWindow::onAppendMeshes()
{
	if (m_meshes.size() >= 2)
	{
		vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
		
		for (int i = 0; i < m_meshes.size(); ++i)
		{
			appendFilter->AddInputData(m_meshes[i]);
		}

		appendFilter->Update();

		removeAllMeshes();

		vtkSmartPointer<vtkPolyData> mesh = vtkSmartPointer<vtkPolyData>::New();
	    mesh->DeepCopy(appendFilter->GetOutput());
	    m_meshes.push_back(mesh);

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(mesh);
  
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
		actor->GetProperty()->SetColor(0.5, 0.0, 0.1);

	    m_actors.push_back(actor);
 
 	    m_renderer->AddActor(actor);
 
 	    m_renderer->ResetCamera();
 	    m_ui->mainRenderWindow->update();
	}
	
	
}

void MainWindow::onSplitMesh()
{
	//std::cout << "number of meshes: " <<  m_meshes.size() << std::endl;
	if (m_meshes.size() == 1)
	{
		vtkSmartPointer<vtkPolyDataConnectivityFilter> splitterFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
		splitterFilter->SetInputData(m_meshes[0]);
		splitterFilter->SetExtractionModeToAllRegions();
		//splitterFilter->SetExtractionModeToLargestRegion();
		//splitterFilter->SetExtractionModeToSpecifiedRegions(); 
		//splitterFilter->AddSpecifiedRegion(1);
		//splitterFilter->ColorRegionsOn();
		//splitterFilter->ScalarConnectivityOn();
		//splitterFilter->ReleaseDataFlagOn();
		splitterFilter->Update();

		int numOfExtractedRegions = splitterFilter->GetNumberOfExtractedRegions();
		std::cout << "Number of independent regions: " << numOfExtractedRegions << std::endl;		
		//vtkIdTypeArray *sizes = splitterFilter->GetRegionSizes();
		//std::cout << "Components: " <<sizes->GetNumberOfComponents();

		/*for (vtkIdType i = 0; i < sizes->GetNumberOfTuples(); ++i)
		{
			std::cout << "Value for " << i << ": " << sizes->GetValue(i) << std::cout;
		}*/

		
		//vtkDataArray* ids = splitterFilter->GetOutput()->GetPointData()->GetArray("RegionId");
		//std::cout << "Output points number: " << splitterFilter->GetOutput()->GetNumberOfPoints() << std::endl;
		//std::cout << "Number of independent regions: " << ids->GetNumberOfTuples() << std::endl;		
		//std::cout << "Components: " <<ids->GetNumberOfComponents();
		/*for (vtkIdType i = 0; i < ids->GetNumberOfTuples(); ++i)
		{
			std::cout << "Value for " << i << ": " << ids->GetTuple(i) << std::cout;
		}*/

		splitterFilter->SetExtractionModeToSpecifiedRegions();
		splitterFilter->Modified();
		splitterFilter->Update();

		std::map<int, int> idsMap;
		for (int i = 0; i < numOfExtractedRegions; ++i)
		{
			splitterFilter->InitializeSpecifiedRegionList();
			splitterFilter->AddSpecifiedRegion(i);
			splitterFilter->Modified();
		    splitterFilter->Update();

			
			vtkSmartPointer<vtkPolyData> data = vtkSmartPointer<vtkPolyData>::New();
			data->DeepCopy(splitterFilter->GetOutput());

			std::cout << i << "   " << data->GetNumberOfCells() << std::endl;

			idsMap.insert(std::pair<int, int>(data->GetNumberOfCells(), i));
		}

		std::cout << "===============================================" << std::endl;

		for (auto iter = idsMap.cbegin(); iter != idsMap.end(); ++iter)
		{
			std::cout << iter->first << "   " << iter->second << std::endl;
		}

		//for (int i = 0; i < splitterFilter->GetNumberOfExtractedRegions(); ++i)
		//{
		//	//vtkSmartPointer<vtkPolyDataConnectivityFilter> subSplitterFilter =
		//	//	vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();

		//	//subSplitterFilter->SetInputData(m_meshes[0]);
		//	splitterFilter->SetExtractionModeToSpecifiedRegions();


		//	//subSplitterFilter->ColorRegionsOn();
	 //    	//subSplitterFilter->ScalarConnectivityOn();
		//	splitterFilter->AddSpecifiedRegion(i);
		//	//subSplitterFilter->ReleaseDataFlagOn();
		//	splitterFilter->Update();

		//	vtkSmartPointer<vtkPolyData> data = vtkSmartPointer<vtkPolyData>::New();
		//	data->DeepCopy(splitterFilter->GetOutput());

		//	std::cout << i << "   " << data->GetNumberOfPoints() << std::endl;
		//}

		removeAllMeshes();
		 
		vtkSmartPointer<vtkPolyData> mesh = vtkSmartPointer<vtkPolyData>::New();
	    mesh->DeepCopy(splitterFilter->GetOutput());
	    m_meshes.push_back(mesh);

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(splitterFilter->GetOutputPort());
		mapper->SetScalarRange(splitterFilter->GetOutput()->GetPointData()->GetArray("RegionId")->GetRange());
		mapper->ScalarVisibilityOn();
  
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
		
		//actor->GetProperty()->SetColor(0.0, 0.5, 0.1);

	    m_actors.push_back(actor);
 
 	    m_renderer->AddActor(actor);
 
 	    m_renderer->ResetCamera();
 	    m_ui->mainRenderWindow->update();

	}
	
}

void MainWindow::removeAllMeshes()
{
	m_meshes.clear();

	for (int i = 0; i < m_actors.size(); ++i)	
		m_renderer->RemoveActor(m_actors[i]);
	
	m_actors.clear();
}
