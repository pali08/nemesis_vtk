#include "EventQtSlotConnect.h"
#include "MoleculeAtomBoundCount.h"
#include "MoleculeInteractorStyleAppendCH3.h"
#include "vtkGenericOpenGLRenderWindow.h"

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

#include <array>
#if VTK_VERSION_NUMBER >= 89000000000ULL
#define VTK890 1
#endif

// Constructor
EventQtSlotConnect::EventQtSlotConnect()
{
  this->setupUi(this);

  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
#if VTK890
  this->qvtkWidget->setRenderWindow(renderWindow);
#else
  this->qvtkWidget->SetRenderWindow(renderWindow);
#endif

  vtkNew<vtkEventQtSlotConnect> slotConnector;
  this->Connections = slotConnector;

  // Sphere copied from CreateMolecule.cxx
  MoleculeAtomBoundCount* molecule = new MoleculeAtomBoundCount();
  vtkNew<vtkPoints> AtomPoints;
  vtkNew<vtkIntArray> AtomNumbers;
  vtkNew<vtkDataSetAttributes> Attributes;
  molecule->AppendAtom(12, 0.0, 0.0, 0.0);
  molecule->AppendAtom(12, 0.0, 2.0, 0.0);
  molecule->AppendBond(molecule->GetAtom(0), molecule->GetAtom(1), 1);
  vtkSmartPointer<vtkMoleculeMapper> moleculeMapper = 
    vtkSmartPointer<vtkMoleculeMapper>::New();
  moleculeMapper->SetInputData(molecule);
  vtkNew<vtkActor> moleculeActor;
  moleculeActor->SetMapper(moleculeMapper);

  // VTK Renderer
  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(moleculeActor);
  renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());

#if VTK890
  this->qvtkWidget->renderWindow()->AddRenderer(renderer);
  //vtkNew<vtkRenderWindowInteractor> iren;
  //this->qvtkWidget->renderWindow()->SetInteractor(iren);
#else
  this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
  //vtkNew<vtkRenderWindowInteractor> iren;
  //this->qvtkWidget->GetRenderWindow()->SetInteractor(iren);
#endif
  vtkSmartPointer<MoleculeInteractorStyleAppendCH3> removeAtomByClick = 
    vtkSmartPointer<MoleculeInteractorStyleAppendCH3>::New();
  removeAtomByClick->SetMolecule(molecule);
  //iren->SetInteractorStyle(removeAtomByClick);
#if VTK890
  this->qvtkWidget->renderWindow()->GetInteractor()->SetInteractorStyle(removeAtomByClick);
  //removeAtomByClick->SetRenderWindow(this->qvtkWidget->renderWindow());
#else
  this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(removeAtomByClick);
  //removeAtomByClick->SetRenderWindow(this->qvtkWidget->GetRenderWindow());
#endif

#if VTK890
  this->Connections->Connect(
      this->qvtkWidget->renderWindow()->GetInteractor(),
      vtkCommand::LeftButtonPressEvent, this,
      SLOT(slot_clicked(vtkObject*, unsigned long, void*, void*)));
#else
  this->Connections->Connect(
      this->qvtkWidget->GetRenderWindow()->GetInteractor(),
      vtkCommand::LeftButtonPressEvent, this,
      SLOT(slot_clicked(vtkObject*, unsigned long, void*, void*)));
#endif
};

void EventQtSlotConnect::slot_clicked(vtkObject*, unsigned long, void*, void*)
{
  std::cout << "Clicked." << std::endl;
}
