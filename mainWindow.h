
#pragma once

// QT
#include <QMainWindow>


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

private:	
	Ui::MainWindow* m_ui;
};
