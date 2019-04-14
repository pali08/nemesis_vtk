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
#include <vtkCylindricalTransform.h>
#include <vtkMath.h>

namespace
{
class MoleculeInteractorStyleAppendCH3 : public vtkInteractorStyleTrackballCamera
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
    this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0], 
                                        this->Interactor->GetEventPosition()[1], 
                                        0,  // always zero.
                                        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
    double picked[3];
    this->Interactor->GetPicker()->GetPickPosition(picked);

    std::vector<std::array<long long int, 3>> Bonds = molecule->GetAtomBonds();

    for (long long int i=0; i<molecule->GetNumberOfAtoms(); i++)
    {
      std::vector<long long int> BondedAtomsToCurrentAtom;
      double AtomX = molecule->GetAtom(i).GetPosition().GetX();
      double AtomY = molecule->GetAtom(i).GetPosition().GetY();
      double AtomZ = molecule->GetAtom(i).GetPosition().GetZ();
      double DistX = AtomX - picked[0];
      double DistY = AtomY - picked[1];
      double DistZ = AtomZ - picked[2];
      for (long long int j = 0; j < molecule->GetNumberOfBonds(); j++)
      {
        for (int k = 0; k < 2; k++) //first and second Id - for now we dont't check bond order
        {
          if (Bonds[j][k] == i){ // we find that this atom has bonds
            int FoundAtomId = k ^ 1; // if matched atom is on position [j][0] we need to get atom on position [j][1] and vice versa
            BondedAtomsToCurrentAtom.push_back(FoundAtomId);
        }
      }

      if (VectorMagnitude(DistX, DistY, DistZ) < 0.5) //need to play a little bit and relaize, how numbers work (different sizes of window etc)
      {
        molecule->AppendAtom(GetNewAtomPosition(BondedAtomsToCurrentAtom, i));
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

  std::array<3> AddVector(double[3] a, double[3] b)
  {
    double[3] c;
    for(int i = 0; i < 3; i++)
    {
      c[i] = a[i] + b[i];
    }
    return c;
  }

  std::array<3> SubtractVector(double[3] a, double[3] b)
  {
    double[3] c;
    for(int i = 0; i < 3; i++)
    {
      c[i] = a[i] - b[i];
    }
    return c;
  }

  private:
  MoleculeAtomBoundCount* molecule = new MoleculeAtomBoundCount();
  vtkSmartPointer<vtkRenderWindow> RenderWindow;

  std::array<int,3> GetNewAtomPosition(std::vector<long long int> BondedAtomsToCurrentAtom, vtkAtomId AtomId)
  {
    /**
    @param AtomId current Atom Id to which we want to append atom 
    @param BondedAtomsToCurrentAtom vector of atom Ids that are connected to current atom
    @return position of appended 
    */
    std::array<int, 3> AveragedAndReversedVector;
    if(BondedAtomsToCurrentAtom.size() == 1)
    {
      float BondedAtomPosition = molecule->GetAtom(BondedAtomsToCurrentAtom[0])->GetPosition();
      float CurrentAtomPosition = molecule->GetAtom(AtomId)->GetPosition();
      float ResultPosition = AddVector(CurrentAtomPosition, SubtractVector(CurrentAtomPosition, BondedAtomPosition));
    }
  }
};
vtkStandardNewMacro(MoleculeInteractorStyle);
}

