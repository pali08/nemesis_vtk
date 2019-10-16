#include "EventQtSlotConnect.h"

#include <vtkLineSource.h>
#include "vtkGenericOpenGLRenderWindow.h"
#include <vtkEventQtSlotConnect.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkVersion.h>

#include "HighlightWithLighting.cxx"

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

  // Sphere1
  vtkNew<vtkSphereSource> sphereSource;
  sphereSource->Update();
  vtkNew<vtkPolyDataMapper> sphereMapper;
  sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

  vtkNew<vtkActor> sphereActor;
  sphereActor->SetMapper(sphereMapper);
  sphereActor->GetProperty()->SetColor(colors->GetColor4d("Tomato").GetData());

  // Sphere2
  vtkNew<vtkSphereSource> sphereSource2;
  sphereSource2->Update();
  vtkNew<vtkPolyDataMapper> sphereMapper2;
  sphereMapper2->SetInputConnection(sphereSource2->GetOutputPort());

  vtkNew<vtkActor> sphereActor2;
  sphereActor2->SetMapper(sphereMapper2);
  sphereActor2->GetProperty()->SetColor(colors->GetColor4d("Green").GetData());
  sphereActor2->SetPosition(1.0, 1.0, 1.0);

  // Line
  vtkNew<vtkLineSource> lineSource;
  lineSource->SetPoint1(0.0, 0.0, 0.0);
  lineSource->SetPoint2(1.0, 1.0, 1.0);
  lineSource->Update();
  vtkNew<vtkPolyDataMapper> lineMapper;
  lineMapper->SetInputConnection(lineSource->GetOutputPort());
  vtkNew<vtkActor> lineActor;
  lineActor->GetProperty()->SetRenderLinesAsTubes(true);
  lineActor->GetProperty()->SetLineWidth(6.0);
  lineActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
  lineActor->SetMapper(lineMapper);


  // VTK Renderer
  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(sphereActor);
  renderer->AddActor(sphereActor2);
  renderer->AddActor(lineActor);
  renderer->SetBackground(colors->GetColor3d("DarkGray").GetData());

#if VTK890
  this->qvtkWidget->renderWindow()->AddRenderer(renderer);
#else
  this->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
#endif
  auto style =
      vtkSmartPointer<MouseInteractorHighLightActor>::New();
#if VTK890
  this->qvtkWidget->renderWindow()->GetInteractor()->SetInteractorStyle(style);
#else
  this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
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
