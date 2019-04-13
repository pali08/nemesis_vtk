#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkMolecule.h>
#include <vtkAtom.h>
#include <vtkMoleculeMapper.h>
#include <vtkNew.h>
#include "MoleculeAtomBoundCount.h"

#include <array>

int main(int, char *[]) {
  vtkSmartPointer<vtkMolecule> molecule =
    vtkSmartPointer<vtkMolecule>::New();

  molecule->AppendAtom(12, 0.0, 0.0, 0.0);
  molecule->AppendAtom(12, 1.0, 1.0, 1.0);
  molecule->AppendAtom(12, 2.0, 2.0, 2.0);

  molecule->AppendBond(molecule->GetAtom(0), molecule->GetAtom(1), 1);
  molecule->AppendBond(molecule->GetAtom(1), molecule->GetAtom(2), 1);
  
  molecule->ClearEdgePoints(1);
  
  //molecule->vtkGraph::RemoveVertexInternal(1, true);

  vtkSmartPointer<vtkMoleculeMapper> moleculeMapper = 
    vtkSmartPointer<vtkMoleculeMapper>::New();
  moleculeMapper->SetInputData(molecule);

  vtkNew<vtkActor> actor;
  actor->SetMapper(moleculeMapper);

  vtkSmartPointer<vtkRenderer> renderer = 
    vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);

  vtkSmartPointer<vtkRenderWindow> window = 
    vtkSmartPointer<vtkRenderWindow>::New();
  window->AddRenderer(renderer);
  
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(window);
  
  vtkNew<vtkNamedColors> colors;
  
  renderer->SetBackground(0.0,0.0,0.0);
  window->SetSize(640, 480);
  window->Render();
  renderer->GetActiveCamera()->Zoom(2.0);
  renderer->SetBackground(colors->GetColor3d("Silver").GetData());

  window->SetMultiSamples(0);
  window->GetInteractor()->Initialize();
  window->GetInteractor()->Start(); 

  return EXIT_SUCCESS;


}
