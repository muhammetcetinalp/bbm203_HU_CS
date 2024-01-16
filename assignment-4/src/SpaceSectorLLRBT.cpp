#include "SpaceSectorLLRBT.h"
#include "stack"
#include "queue"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
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

SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
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
            current = current->right;
        }

        current = node_stack.top();
        node_stack.pop();

        current = current->left;
    }

    while (!delete_stack.empty()) {
        current = delete_stack.top();
        delete_stack.pop();
        delete current;
    }


}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map
    if(root == nullptr){
        root = new Sector(x,y,z);
        root->color = false;
        sector_code_of_earth = "0SSS";
    }
    else{
        insert(root,x,y,z);
    }
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors
    cout << "Space sectors inorder traversal:" << endl;
    inOrder(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print
    cout << "Space sectors preorder traversal:" << endl;
    PreOrder(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print
    cout << "Space sectors postorder traversal:" << endl;
    PostOrder(root);
    cout << endl;
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its

    std::stack<Sector*> path_stack_for_arrive;
    std::stack<Sector*> path_for_earth;
    Sector* current = root;
    if (root == nullptr){
        return path;
    }
    while (current != nullptr || !path_stack_for_arrive.empty()){
        while (current!= nullptr){
            path_stack_for_arrive.push(current);
            current = current->left;
        }
        current = path_stack_for_arrive.top();
        if (current->sector_code == sector_code){
            break;
        }
        path_stack_for_arrive.pop();
        current = current->right;
    }
    if(path_stack_for_arrive.empty()){
        return path;
    }

    Sector* current2 = root;
    while (current2 != nullptr || !path_for_earth.empty()){
        while (current2!= nullptr){
            path_for_earth.push(current2);
            current2 = current2->left;
        }
        current2 = path_for_earth.top();
        if (current2->sector_code == sector_code_of_earth){
            break;
        }
        path_for_earth.pop();
        current2 = current2->right;
    }
    if(path_for_earth.empty()){
        return path;
    }

    std::stack<Sector*> distance;
    while (current != nullptr){
        distance.push(current);
        current = current->parent;
    }
    std::stack<Sector*> world;
    while (current2 != nullptr){
        world.push(current2);
        current2 = current2->parent;
    }

    queue<Sector*> q;
    while (!world.empty() && !distance.empty()){
        Sector* sec1 = world.top();
        Sector* sec2 = distance.top();
        if(sec1->sector_code == sec2->sector_code){
            if(!q.empty()){
                q.pop();
            }
            q.push(world.top());
            world.pop();
            distance.pop();
        }
        else{
            break;
        }
    }
    deque<Sector*> deque_for_path;
    deque_for_path.push_back(q.front());

    while (!distance.empty()){
        deque_for_path.push_back(distance.top());
        distance.pop();
    }
    while(!world.empty()){
        deque_for_path.push_front(world.top());
        world.pop();
    }

    while (!deque_for_path.empty()){
        path.push_back(deque_for_path.front());
        deque_for_path.pop_front();
    }
    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function
    if (path.empty()){
        cout << "A path to Dr. Elara could not be found."<<endl;
    }
    else {
        cout << "The stellar path to Dr. Elara: ";
        bool first = true;
        for (const auto& sector : path) {
            if (first){
                cout << sector->sector_code;
                first = false;
                continue;
            }
            cout << "->"<< sector->sector_code;
        }
        cout << endl;
    }
}

Sector* SpaceSectorLLRBT::insert(Sector* &current,const int x,const int y,const int z) {
    if (x < current->x || (x == current->x && y < current->y) ||
        (x == current->x && y == current->y && z < current->z)){
        if(current->left == nullptr){
            current->left = new Sector(x,y,z);
            current->left->parent = current;
            current->left->color = true;
        }else{
            insert(current->left,x,y,z);
        }
        if(current->left != nullptr && current->left->left != nullptr){
            if(current->left->color && current->left->left->color){
                rotate_right(current);
            }
        }
    }

    else if (x > current->x || (x == current->x && y > current->y) ||
             (x == current->x && y == current->y && z > current->z)) {
        if (current->right == nullptr) {
            current->right = new Sector(x,y,z);
            current->right->parent = current;
            current->right->color = true;
        } else {
            insert(current->right, x,y,z);
        }
        flip_colors(current);
        if (current->right->color){
            rotate_left(current);
        }
    }
    return current;
}
void SpaceSectorLLRBT::flip_colors(Sector *&treePtr) {
    if (treePtr != nullptr && treePtr->right != nullptr && treePtr->left != nullptr ){
        if (!treePtr->color && treePtr->left->color && treePtr->right->color){
        treePtr->color = true;
            if(treePtr->parent == nullptr){
                treePtr->color = false;
            }
            treePtr->left->color = false;
            treePtr->right->color = false;
        }
    }
}


void SpaceSectorLLRBT::rotate_left(Sector *&treePtr) {
    Sector* x = treePtr->right;
    treePtr->right = x->left;
    if (x->left != nullptr){
        x->left->parent = treePtr;
    }
    x->parent = treePtr->parent;
    x->left = treePtr;
    x->color = treePtr->color;
    treePtr->color = true;
    treePtr->parent = x;
    treePtr = x;

}
void SpaceSectorLLRBT::rotate_right(Sector *&treePtr) {
    Sector* x = treePtr->left;
    treePtr->left = x->right;
    if(x->right != nullptr){
        x->right->parent = treePtr;
    }
    x->right = treePtr;
    x->parent = treePtr->parent;
    treePtr->parent = x;
    treePtr->color = x->color;
    x->color = false;
    treePtr = x;
    flip_colors(treePtr);


}

void SpaceSectorLLRBT::inOrder(Sector* &treePtr) {
    if (treePtr != nullptr){
        inOrder(treePtr->left);
        if(!treePtr->color){
            cout <<"BLACK sector: " <<treePtr->sector_code << endl;
        }
        else
            cout <<"RED sector: " <<treePtr->sector_code << endl;
        inOrder(treePtr->right);
    }
}


void SpaceSectorLLRBT::PreOrder(Sector* &treePtr) {
    if (treePtr != nullptr){
        if(!treePtr->color){
            cout <<"BLACK sector: " <<treePtr->sector_code << endl;
        }
        else
            cout <<"RED sector: " <<treePtr->sector_code << endl;
        PreOrder(treePtr->left);
        PreOrder(treePtr->right);
    }
}


void SpaceSectorLLRBT::PostOrder(Sector* &treePtr) {
    if (treePtr != nullptr){
        PostOrder(treePtr->left);
        PostOrder(treePtr->right);
        if(!treePtr->color){
            cout <<"BLACK sector: " <<treePtr->sector_code << endl;
        }
        else
            cout <<"RED sector: " <<treePtr->sector_code << endl;
    }
}