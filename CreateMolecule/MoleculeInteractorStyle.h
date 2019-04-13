#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>
#include <vtkWorldPointPicker.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>


namespace
{
class MoleculeInteractorStyle : public vtkInteractorStyleTrackballCamera
{

public:
  static MoleculeInteractorStyle* New();
  vtkTypeMacro(MoleculeInteractorStyle, vtkInteractorStyleTrackballCamera);

  double VectorMagnitude(double x, double y, double z)
  {
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
  }

  virtual void OnLeftButtonDown() override
  {
    //std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0] << " " << this->Interactor->GetEventPosition()[1] << std::endl;
    this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0], 
                                        this->Interactor->GetEventPosition()[1], 
                                        0,  // always zero.
                                        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
    double picked[3];
    this->Interactor->GetPicker()->GetPickPosition(picked);
    //std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;

    for (long long int i=0; i<molecule->GetNumberOfAtoms(); i++)
    {
      double AtomX = molecule->GetAtom(i).GetPosition().GetX();
      double AtomY = molecule->GetAtom(i).GetPosition().GetY();
      double AtomZ = molecule->GetAtom(i).GetPosition().GetZ();
      double DistX = AtomX - picked[0];
      double DistY = AtomY - picked[1];
      double DistZ = AtomZ - picked[2];
      if (VectorMagnitude(DistX, DistY, DistZ) < 2) //need to play a little bit and relaize, how numbers work (different sizes of window etc)
      {
        molecule->RemoveAtom(i);
        break;
      }
    }

    // Forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
  }
 
  void SetMolecule(MoleculeAtomBoundCount* molecule)
  {
    this->molecule = molecule;
  }

  void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> RenderWindow)
  {
    this->RenderWindow = RenderWindow;
  }

  private:
  MoleculeAtomBoundCount* molecule = new MoleculeAtomBoundCount();
  vtkSmartPointer<vtkRenderWindow> RenderWindow;
//private:
  //vtkMolecule molecule;
};
vtkStandardNewMacro(MoleculeInteractorStyle);
}

