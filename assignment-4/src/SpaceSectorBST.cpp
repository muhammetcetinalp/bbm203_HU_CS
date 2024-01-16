#include "SpaceSectorBST.h"
#include "stack"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory.
    if (root == nullptr){
        return;
    }
    std::stack<Sector*> node_stack;
    std::stack<Sector*> delete_stack;

    Sector* current = root;

    while (current != nullptr || !node_stack.empty()) {
        while (current != nullptr) {
            node_stack.push(current);
            delete_stack.push(current);
            current = current->left;
        }

        current = node_stack.top();
        node_stack.pop();

        current = current->right;
    }

    while (!delete_stack.empty()) {
        current = delete_stack.top();
        delete_stack.pop();
        delete current;
    }
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
    std::ifstream input_file(filename);
    std::string line;
    bool cont = true;
    while (std::getline(input_file, line)) {
        if (cont){
            cont = false;
            continue;
        }
        std::stringstream ss(line);
        std::string token;
        std::getline(ss, token, ',');
        int x = std::stoi(token);
        std::getline(ss, token, ',');
        int y = std::stoi(token);
        std::getline(ss, token, ',');
        int z = std::stoi(token);

        insertSectorByCoordinates(x, y, z);



    }
    input_file.close();


}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    if(root == nullptr){
        root = new Sector(x,y,z);
    }
    else
        insert(root,Sector(x,y,z));

}
void SpaceSectorBST::insert(Sector* &current, const Sector &sector) {
    if (sector.x < current->x || (sector.x == current->x && sector.y < current->y) ||
            (sector.x == current->x && sector.y == current->y && sector.z < current->z)){
        if(current->left == nullptr){
            current->left = new Sector(sector);
            current->left->parent = current;
        }
        else{
            insert(current->left,sector);
        }
    }

    else if (sector.x > current->x || (sector.x == current->x && sector.y > current->y) ||
             (sector.x == current->x && sector.y == current->y && sector.z > current->z)) {
        if (current->right == nullptr) {
            current->right = new Sector(sector);
            current->right->parent = current;
        } else {
            insert(current->right, sector);

        }
    }
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.
    Sector* found = find(root,sector_code);

    if (found != nullptr){
        if(found->sector_code == sector_code){
            delete_Sector(found);}
    }
}
Sector* SpaceSectorBST::find(Sector *treePtr,std::string sector_code) {
    std::stack<Sector*> path_stack;
    if (treePtr == nullptr){
        return nullptr;
    }
    while (treePtr != nullptr || !path_stack.empty()){
        while (treePtr!= nullptr){
            path_stack.push(treePtr);
            treePtr = treePtr->left;
        }
        treePtr = path_stack.top();
        path_stack.pop();
        if (treePtr->sector_code == sector_code){
            return treePtr;
        }
        treePtr = treePtr->right;
    }
    return nullptr;

}
void SpaceSectorBST::delete_Sector(Sector* &tree_ptr) {
    if(tree_ptr->left == nullptr && tree_ptr->right == nullptr){
        if (tree_ptr->parent != nullptr) {
            if (tree_ptr->parent->left == tree_ptr) {
                tree_ptr->parent->left = nullptr;
            }
            else{
                tree_ptr->parent->right = nullptr;
            }
            tree_ptr->parent = nullptr;
        }
        delete tree_ptr;
        tree_ptr = nullptr;
    }
    else if (tree_ptr->left == nullptr && tree_ptr->right != nullptr){
        tree_ptr->right->parent = tree_ptr->parent;
        if (tree_ptr->parent != nullptr) {
            if (tree_ptr->parent->left == tree_ptr) {
                tree_ptr->parent->left = tree_ptr->right;
            } else {
                tree_ptr->parent->right = tree_ptr->right;
            }
        } else {
            tree_ptr = tree_ptr->right;
            tree_ptr->parent = nullptr;
        }
        tree_ptr->parent = nullptr;
        delete tree_ptr;
        tree_ptr = nullptr;
    }
    else if (tree_ptr->right == nullptr && tree_ptr->left != nullptr){
        tree_ptr->left->parent = tree_ptr->parent;
        if (tree_ptr->parent != nullptr) {
            if (tree_ptr->parent->left == tree_ptr) {
                tree_ptr->parent->left = tree_ptr->left;
            } else {
                tree_ptr->parent->right = tree_ptr->left;
            }
        } else {
            tree_ptr = tree_ptr->left;
            tree_ptr->parent = nullptr;
        }
        tree_ptr->parent = nullptr;
        delete tree_ptr;
        tree_ptr = nullptr;
    }
    else if(tree_ptr->right != nullptr && tree_ptr->left != nullptr){
        Sector* right_of_most_left_of_tree = tree_ptr->right;
        while (right_of_most_left_of_tree->left != nullptr){
            right_of_most_left_of_tree = right_of_most_left_of_tree->left;
        }
        deleteSectorWithTwoChildren(tree_ptr,right_of_most_left_of_tree);
    }
}

void SpaceSectorBST::deleteSectorWithTwoChildren(Sector *&old_sector, Sector *&new_sector) {
    if(old_sector == nullptr || new_sector == nullptr){
        return;
    }
    if(new_sector->left == nullptr && new_sector->right == nullptr){
        if (new_sector->parent != nullptr) {
            if (new_sector->parent->left == new_sector) {
                new_sector->parent->left = nullptr;
            } else {
                new_sector->parent->right = nullptr;
            }
            new_sector->parent = nullptr;
        }


    }
    else if (new_sector->left == nullptr && new_sector->right != nullptr){
        new_sector->right->parent = new_sector->parent;
        if (new_sector->parent != nullptr) {
            if (new_sector->parent->left == new_sector) {
                new_sector->parent->left = new_sector->right;
            } else {
                new_sector->parent->right = new_sector->right;
            }
            new_sector->right->parent = new_sector->parent;
        }

    }

    new_sector->parent = old_sector->parent;
    new_sector->left = old_sector->left;
    new_sector->right = old_sector->right;
    if(old_sector->parent != nullptr){
        if (old_sector->parent->left == old_sector) {
            old_sector->parent->left = new_sector;
        } else {
            old_sector->parent->right = new_sector;
        }
    }
    else(root = new_sector);
    if(old_sector->right != nullptr){
        old_sector->right->parent = new_sector;
    }
    if(old_sector->left != nullptr){
        old_sector->left->parent = new_sector;
    }

    old_sector->parent = nullptr;
    old_sector->left = nullptr;
    old_sector->right = nullptr;

    delete old_sector;
    old_sector = nullptr;
}



void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors
    cout << "Space sectors inorder traversal:" << endl;
    inOrder(root);
    cout << endl;


}
void SpaceSectorBST::inOrder(Sector* &treePtr) {
    if (treePtr != nullptr){
        inOrder(treePtr->left);
        cout << treePtr->sector_code << endl;
        inOrder(treePtr->right);
    }
}


void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print
    cout << "Space sectors preorder traversal:" << endl;
    PreOrder(root);
    cout << endl;

}
void SpaceSectorBST::PreOrder(Sector* &treePtr) {
    if (treePtr != nullptr){
        cout << treePtr->sector_code << endl;
        PreOrder(treePtr->left);
        PreOrder(treePtr->right);
    }
}


void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print
    cout << "Space sectors postorder traversal:" << endl;
    PostOrder(root);
    cout << endl;
}
void SpaceSectorBST::PostOrder(Sector* &treePtr) {
    if (treePtr != nullptr){
        PostOrder(treePtr->left);
        PostOrder(treePtr->right);
        cout << treePtr->sector_code << endl;
    }
}
std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    std::stack<Sector*> path_stack;
    Sector* current = root;
    if (root == nullptr){
        return path;
    }
    while (current != nullptr || !path_stack.empty()){
        while (current!= nullptr){
            path_stack.push(current);
            current = current->left;
        }
        current = path_stack.top();
        path_stack.pop();
        if (current->sector_code == sector_code){
            break;
        }
        current = current->right;
    }
    if (current != nullptr){
        stack<Sector*> stack_for_path;
        while (current != nullptr){
            stack_for_path.push(current);
            current = current->parent;
        }
        while (!stack_for_path.empty()){
            path.push_back(stack_for_path.top());
            stack_for_path.pop();
        }
    }
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function
    if (path.empty()){
        cout << "A path to Dr. Elara could not be found." << endl;
    }
    else {
        cout << "The stellar path to Dr. Elara: ";
        bool first = true;
        for (const auto& sector : path) {
            if(first){
                first = false;
                cout << sector->sector_code;
                continue;
            }
            cout << "->"<< sector->sector_code;
        }
        cout << endl;
    }
}