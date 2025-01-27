#include <iostream>
#include <vector>

#define treeOrder 2

struct BTNode {
  std::vector<int> keys;
  std::vector<BTNode*> children;
  bool isLeaf;

  BTNode* search(int key){
    int i = 0;
    while (i < keys.size() && key > keys[i]) i++;
    if (i < keys.size() && key == keys[i]) return this;
    if (isLeaf) return nullptr;
    return children[i]->search(key);
  }
};



int main() {

  BTNode* root = new BTNode();
  root->keys.push_back(4);
  root->keys.push_back(8);
  root->isLeaf = false;

  BTNode* p1 = new BTNode();
  p1->keys.push_back(0);
  p1->keys.push_back(1);
  p1->isLeaf = true;

  BTNode* p2 = new BTNode();
  p2->keys.push_back(5);
  p2->keys.push_back(6);
  p2->isLeaf = true;

  BTNode* p3 = new BTNode();
  p3->keys.push_back(10);
  p3->keys.push_back(11);
  p3->isLeaf = true;

  root->children.push_back(p1);
  root->children.push_back(p2);
  root->children.push_back(p3);

  BTNode* result = root->search(5);
  if (result != nullptr) std::cout << "Key found!" << std::endl;
  else std::cout << "Key not found!" << std::endl;

  return 0;
}