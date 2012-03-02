//#include "sjConfig.h"
//#include "ui_sjViewerUI.h"
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include "sjViewer.h"




using namespace std;
//using namespace qglviewer;

int main(int argc, char **argv)
{
	//Q_INIT_RESOURCE(sjviewer);

  QApplication app(argc, argv);
  QWidget * window = new QWidget;
  window->setWindowTitle("San Jeronimo Viewer");
  QPushButton *button = new QPushButton("Laplacian Smoothing.");
  sjViewer * myviewer = new sjViewer(window, false);

  Polyhedron P;
  ifstream myfile ("elephant.off");
  myfile>>P;
  cout<<"Parece que lo abrio "<<P.size_of_vertices();
  myviewer->setVerticesFaces(P);

  
  QObject::connect(button, SIGNAL(clicked()), myviewer, SLOT(LaplacianSmoothing()));
  QObject::connect(button, SIGNAL(clicked()), myviewer, SLOT(updateGL()));
  

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addWidget(button);
  mainLayout->addWidget(myviewer);

   window->setLayout(mainLayout);

   myviewer->draw();
   window->show();


  /*app.setOrganizationDomain("San Jeronimo");
  app.setOrganizationName("San Jeronimo");
  app.setApplicationName("San Jeronimo Viewer");*/

  /*Q_INIT_RESOURCE(File);
  Q_INIT_RESOURCE(Triangulation_2); 
  Q_INIT_RESOURCE(Input);
  Q_INIT_RESOURCE(CGAL);*/



  /*Polyhedron P;


  ifstream myfile ("elephant.off");
  

  myfile>>P;

  cout<<"Parece que lo abrio "<<P.size_of_vertices();*/
  

  /*

  for ( f = P.facets_begin(); f != P.facets_end(); ++f){
		Halfedge_facet_circulator j = f->facet_begin();
		cout<<"Cara: ";
		do{
			cout<<j->vertex()->point()<<", ";
		}while(++j != f->facet_begin());
		cout<<"\n";
  }*/
  


  /*QMainWindow *window = new QMainWindow;

  Ui::DvonnWindow ui;
        ui.setupUi(window);

		

		ui.viewer->setVerticesFaces(P);*/
		
		//window->
		//connect(ui.actionLaplacian_Smoothing, SIGNAL(triggered()), ui, SLOT(ui.viewer->LaplacianSmoothing()));
		
		//QObject::connect(& (ui.actionLaplacian_Smoothing), SIGNAL(triggered()), &app, SLOT(ui.viewer->LaplacianSmoothing()));
		/*ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();*/
		//ui.viewer->draw();

        //window->show();

  /*MainWindow mainWindow;
  mainWindow.show();
  QStringList args = app.arguments();
  args.removeAt(0);

  if(!args.empty() && args[0] == "--use-meta")
  {
    mainWindow.setAddKeyFrameKeyboardModifiers(::Qt::MetaModifier);
    args.removeAt(0);
  }

  Q_FOREACH(QString filename, args) {
    mainWindow.open(filename);
  }*/
  return app.exec();
}