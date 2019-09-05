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
  static MoleculeInteractorStyleAppendCH3* New();
  vtkTypeMacro(MoleculeInteractorStyleAppendCH3, vtkInteractorStyleTrackballCamera);

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
  
  double* picked = this->Interactor->GetPicker()->GetPickPosition();
  //TODO bonded atoms to current atom je potrebne niekde vyprazdnit 
  std::vector<std::array<long long int, 3>> Bonds = molecule->GetAtomBonds();
  long long int NumberOfAtoms = molecule->GetNumberOfAtoms();
  long long int NumberOfBonds = molecule->GetNumberOfBonds();
  for (long long int i=0; i<NumberOfAtoms; i++)
  {
    std::vector<long long int> BondedAtomsToCurrentAtom;
    double AtomX = molecule->GetAtom(i).GetPosition().GetX();
    double AtomY = molecule->GetAtom(i).GetPosition().GetY();
    double AtomZ = molecule->GetAtom(i).GetPosition().GetZ();
    double DistX = AtomX - picked[0];
    double DistY = AtomY - picked[1];
    double DistZ = AtomZ - picked[2];
    for (long long int j = 0; j < NumberOfBonds; j++)
    {
      for (int k = 0; k < 2; k++) //first and second Id - for now we dont't check bond order
      {
        if (Bonds[j][k] == i)   // we find that this atom has bonds
        { 
          int FoundAtomFirstOrSecond = k ^ 1; // if matched atom is on position [j][0] we need to get atom on position [j][1] and vice versa
          BondedAtomsToCurrentAtom.push_back(Bonds[j][FoundAtomFirstOrSecond]);
        }
      }
    }
    if (VectorMagnitude(DistX, DistY, DistZ) < 2.0) //need to play a little bit and relaize, how numbers work (different sizes of window etc)  
    {
      molecule->AppendAtom(12, GetNewAtomPosition(BondedAtomsToCurrentAtom, i));
      molecule->AppendBond(molecule->GetAtom(i), molecule->GetAtom(NumberOfAtoms), 1);
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

  std::array<double, 3> AddVector(double a[3], double b[3])
  {
    std::array<double, 3> c;
    for(int i = 0; i < 3; i++)
    {
      c[i] = a[i] + b[i];
    }
    return c;
  }

  std::array<double, 3> SubtractVector(double a[3], double b[3])
  {
    std::array<double, 3> c;
    for(int i = 0; i < 3; i++)
    {
      c[i] = a[i] - b[i];
    }
    return c;
  }

  private:
  MoleculeAtomBoundCount* molecule = new MoleculeAtomBoundCount();
  vtkSmartPointer<vtkRenderWindow> RenderWindow;

  std::array<float, 3> GetNewAtomPosition(std::vector<long long int> BondedAtomsToCurrentAtom, vtkIdType AtomId)
  {
    /**
    @param AtomId current Atom Id to which we want to append atom 
    @param BondedAtomsToCurrentAtom vector of atom Ids that are connected to current atom
    @return position of appended 
    */
    //cout << AtomId;
    for (int j = 0; j < BondedAtomsToCurrentAtom.size(); j++)
    {
      cout << BondedAtomsToCurrentAtom[j] << ',';
    }
    cout << endl;
    std::array<float, 3> ResultData = {0.0, 0.0, 0.0};
    if(BondedAtomsToCurrentAtom.size() == 1)
    {
      vtkVector3f BondedAtomPosition = molecule->GetAtom(BondedAtomsToCurrentAtom[0]).GetPosition();
      vtkVector3f CurrentAtomPosition = molecule->GetAtom(AtomId).GetPosition();
      float* BondedAtomData = BondedAtomPosition.GetData();
      float* CurrentAtomData = CurrentAtomPosition.GetData();
      for (int i = 0; i < 3; i++)
      {
        ResultData[i] = 2*CurrentAtomData[i] - BondedAtomData[i];
        //cout << ResultData[i];
        //cout << ',';
        //ResultData[i] = 2 * BondedAtomData[i] - CurrentAtomData[i];
        //ResultData[i] = ResultData[i] + 1.0;
      }
    }
    return ResultData;
  }
};
vtkStandardNewMacro(MoleculeInteractorStyleAppendCH3);
}

