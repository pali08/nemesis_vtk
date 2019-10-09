#ifndef EventQtSlotConnect_H
#define EventQtSlotConnect_H

#include "ui_EventQtSlotConnect.h"


#include <vtkIntArray.h>
#include <vtkActor.h>
#include <vtkAtom.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkDataSetAttributes.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkMolecule.h>
#include <vtkMoleculeMapper.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkVersion.h>


#include <QMainWindow>

#include <vtkSmartPointer.h>
#include <vtkObject.h>
class vtkEventQtSlotConnect;

class EventQtSlotConnect : public QMainWindow, private Ui::EventQtSlotConnect
{
  Q_OBJECT
public:

  EventQtSlotConnect();

public slots:

  void slot_clicked(vtkObject*, unsigned long, void*, void*);

private:

  vtkSmartPointer<vtkEventQtSlotConnect> Connections;
};

#endif
