#include <vtkMolecule.h>
#include <map>
#include <set>
#include <iterator>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>
#include <queue>
#include <functional>

class MoleculeAtomBoundCount : public vtkMolecule
{
  private:
    std::vector<int> AtomIds; //this will be useful for handling bonds
    std::vector<std::array<double,3>> AtomPoints;
    std::vector<long long int> AtomNumbers;
    std::vector<std::array<long long int, 3>> AtomBonds; //atomid1, atomid2, bond
    
    vtkNew<vtkPoints> AtomPointsVtk;
    vtkNew<vtkIntArray> AtomNumbersVtk;


  public:

    std::vector<std::array<long long int, 3>> GetAtomBonds()
    {
      return this->AtomBonds;
    }

    //tu zjavne kreslim viac atomov na jedno miesto.
    //pomocne pole ???
    vtkAtom AppendAtom(int AtomNumber, double x, double y, double z)
    {
      AtomPoints.push_back({x, y, z});
      AtomNumbers.push_back(AtomNumber);
      //cout << AtomPoints.size();
      return vtkMolecule::AppendAtom(AtomNumber, x, y, z);
    }
    
    vtkAtom AppendAtom(int AtomNumber, std::array<float, 3> xyz)
    {
      double x = xyz[0];
      double y = xyz[1];
      double z = xyz[2];
      return this->AppendAtom(AtomNumber, x, y, z);
    }

    vtkBond AppendBond(vtkAtom Atom1, vtkAtom Atom2, int BondOrder)
    {
      AtomBonds.push_back({Atom1.GetId(), Atom2.GetId(), BondOrder});
      return vtkMolecule::AppendBond(Atom1, Atom2, BondOrder);
    }

    void RemoveBond(long long int BondId)
    {
      //TODO Append bond prida bonds naviac, niekde treba vymazat cely AtomBonds vector
      AtomBonds.erase(AtomBonds.begin() + BondId);
      std::vector<std::array<long long int, 3>> AtomBondsTemp;
      AtomBondsTemp = AtomBonds;
      AtomBonds.clear(); 
      long long int NumberOfBonds = AtomBondsTemp.size();
      long long int NumberOfAtoms = this->GetNumberOfAtoms();
      Initialize();
      for (int i = 0; i < NumberOfAtoms; i++) // we removed point and atom number from array, but GetNumberOfAtoms returns previous value
      {
        //Atoms does notrchange, so we keep therm
        AppendAtom(AtomNumbers.at(i),AtomPoints.at(i)[0], AtomPoints.at(i)[1], AtomPoints.at(i)[2]); 
      }
      for (int i = 0; i < NumberOfBonds; i++)
      {
        cout << AtomBonds[i][0] << " " << AtomBonds[i][1] << " " << AtomBonds[i][2] << endl;
        AppendBond(GetAtom(AtomBondsTemp[i][0]), GetAtom(AtomBondsTemp[i][1]), AtomBondsTemp[i][2]);
      }
      cout << endl;
    }

    virtual void Initialize()
    {
      vtkMolecule::Initialize();
    }

    void RemoveAtom(vtkIdType AtomId)
    {
      long long int NumberOfAtoms = this->GetNumberOfAtoms();
      long long int NumberOfBonds = this->GetNumberOfBonds();
      long long int NumberOfBondsAfterRemove;

      std::vector<std::array<double,3>> AtomPointsTemp;
      std::vector<long long int> AtomNumbersTemp;
      std::vector<std::array<long long int, 3>> AtomBondsTemp;

      AtomPoints.erase(AtomPoints.begin() + AtomId);
      AtomNumbers.erase(AtomNumbers.begin() + AtomId);
      AtomBonds.erase(std::remove_if(AtomBonds.begin(), AtomBonds.end(), 
        [&](std::array<long long int, 3> BondBetweenAtoms){return BondBetweenAtoms[0] == AtomId || BondBetweenAtoms[1] == AtomId;}),
        AtomBonds.end());

      AtomPointsTemp = AtomPoints;
      AtomNumbersTemp = AtomNumbers;
      AtomBondsTemp = AtomBonds;
      
      AtomPoints.clear();
      AtomNumbers.clear();
      AtomBonds.clear();
      //cout << AtomBonds << endl;
      NumberOfBondsAfterRemove = AtomBondsTemp.size();
      //cout << "Num of bonds:" << NumberOfBondsAfterRemove << endl;
      //Atoms that are after removed atom must have smaller id
      for (int i = 0; i < NumberOfBondsAfterRemove; i++) 
      {
        for (int j = 0; j < 2; j++)
        {
          if (AtomBondsTemp[i][j] > AtomId)
          {
            AtomBondsTemp[i][j] = AtomBondsTemp[i][j] - 1; 
          }          
        }
        //cout << AtomBonds[i][0] << " " << AtomBonds[i][1] << endl;
      }
      Initialize();
      for (int i = 0; i < NumberOfAtoms - 1; i++) // we removed point and atom number from array, but GetNumberOfAtoms returns previous value
      {
        AppendAtom(AtomNumbersTemp.at(i),AtomPointsTemp.at(i)[0], AtomPointsTemp.at(i)[1], AtomPointsTemp.at(i)[2]);
      }
      for (int i = 0; i < NumberOfBondsAfterRemove; i++)
      {
        AppendBond(GetAtom(AtomBondsTemp[i][0]), GetAtom(AtomBondsTemp[i][1]), AtomBondsTemp[i][2]);       
      }
    }
    std::array<double, 3> GetPositionAsStdArray(vtkIdType AtomId, vtkVector3f Position)
    {
      double x = this->GetAtom(AtomId).GetPosition().GetX();
      double y = this->GetAtom(AtomId).GetPosition().GetY();
      double z = this->GetAtom(AtomId).GetPosition().GetZ();
      std::array<double, 3> result = {x, y, z};
      return result;
    }

};
