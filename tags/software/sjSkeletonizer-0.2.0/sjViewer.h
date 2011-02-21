//Viewer class for qglviewer

#ifndef SJVIEWER_H
#define SJVIEWER_H

#include <QGLViewer/qglviewer.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Polyhedron_scan_OFF.h> 
#include <CGAL/HalfedgeDS_vector.h>
#include <vector>
#include <fstream>
#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Polyhedron_3.h>
#include "sjLaplacianSmoothing.h"

class QWidget;

class sjViewer : public QGLViewer {

  Q_OBJECT

public:

    
  
  sjViewer(QWidget * parent, bool antialiasing = false);

  // overload several QGLViewer virtual functions
  void draw();
  void initializeGL();
  virtual void init();
  void drawWithNames();
  void postSelection(const QPoint&);
  
  void setScene();
  bool antiAliasing() const { return antialiasing; }

  void setVerticesFaces(sjPolyhedron p1){
	  polyhedron = p1;
  }

  virtual QString helpString() const;

signals:
  void selected(int);

public slots:
  void setAntiAliasing(bool b);
  void setTwoSides(bool b);
  void LaplacianSmoothing();

private:
  void drawModel();
  void draw_aux(bool with_names);

  bool antialiasing;
  bool twosides;
  sjPolyhedron polyhedron;
 
  sjPoint_3 normalize(sjPoint_3 p);
  sjPoint_3 normalVector(sjPoint_3 a, sjPoint_3 b, sjPoint_3 c);
};


#endif