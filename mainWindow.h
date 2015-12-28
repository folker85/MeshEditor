
#pragma once

 // QT
#include <QMainWindow>

 // VTK
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>

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

private:
	void initConnections();

private:	
	Ui::MainWindow* m_ui;

	vtkSmartPointer<vtkRenderer> m_renderer;
};
