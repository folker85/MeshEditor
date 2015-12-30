
#pragma once

 // QT
#include <QMainWindow>

 // VTK
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>


// FORWARD DECLARATIONS
namespace Ui
{
    class MainWindow;
}


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	


public slots:
	void onLoadMesh();
	void onAppendMeshes();
	void onSplitMesh();

private:
	void initConnections();
	void removeAllMeshes();

private:	
	Ui::MainWindow* m_ui;

	vtkSmartPointer<vtkRenderer> m_renderer;
	std::vector<vtkSmartPointer<vtkPolyData> > m_meshes;
	std::vector<vtkSmartPointer<vtkActor> > m_actors;
};
