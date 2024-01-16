#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    void insert(Sector* &current,const Sector& sector);

    void inOrder(Sector* &treePtr);
    void PreOrder(Sector* &treePtr);
    void PostOrder(Sector* &treePtr);

    void delete_Sector(Sector* &treePtr);
    void deleteSectorWithTwoChildren(Sector *&old_sector, Sector *&new_sector);
    Sector* find(Sector* treePtr,std::string sector_code);

};

#endif // SPACESECTORBST_H
