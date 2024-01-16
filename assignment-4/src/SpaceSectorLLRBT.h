#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    Sector* insert(Sector* &current,const int x,const int y,const int z);

    void inOrder(Sector* &treePtr);
    void PreOrder(Sector* &treePtr);
    void PostOrder(Sector* &treePtr);

    void rotate_left(Sector* &treePtr);
    void rotate_right(Sector* &treePtr);
    void flip_colors(Sector* &treePtr);

    std::string sector_code_of_earth;

};

#endif // SPACESECTORLLRBT_H
