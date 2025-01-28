#include <iostream>
#include <vector>

#define treeOrder 2

struct StockItem {
  int id;
  std::string name;
  int quantityInStock;
};

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

  void insert(int key){
    if (isLeaf){
      int i = 0;
      while (i < keys.size() && key > keys[i]) i++;
      keys.insert(keys.begin() + i, key);
      return;
    }
    int i = 0;
    while (i < keys.size() && key > keys[i]) i++;
    children[i]->insert(key);
    if (children[i]->keys.size() > 2 * treeOrder) split(i);
  }

  void split(int i){
    BTNode* newChild = new BTNode();
    BTNode* child = children[i];
    int keyToElevate = child->keys[treeOrder];
    newChild->isLeaf = child->isLeaf;
    newChild->keys = std::vector<int>(child->keys.begin() + treeOrder + 1, child->keys.end());
    child->keys = std::vector<int>(child->keys.begin(), child->keys.begin() + treeOrder);
    if (!child->isLeaf){
      newChild->children = std::vector<BTNode*>(child->children.begin() + treeOrder + 1, child->children.end());
      child->children = std::vector<BTNode*>(child->children.begin(), child->children.begin() + treeOrder + 1);
    }
    keys.insert(keys.begin() + i, keyToElevate);
    children.insert(children.begin() + i + 1, newChild);
  }
};



int main() {

  BTNode* root = new BTNode();
  root->keys.push_back(50);
  root->isLeaf = false;

  root->children.push_back(new BTNode());
  root->isLeaf = false;
  root->children[0]->keys.push_back(9);
  root->children[0]->keys.push_back(16);
  root->children[0]->keys.push_back(30);
  root->children[0]->keys.push_back(40);

  root->children[0]->children.push_back(new BTNode());
  root->children[0]->children[0]->isLeaf = true;;
  root->children[0]->children[0]->keys.push_back(1);
  root->children[0]->children[0]->keys.push_back(2);
  root->children[0]->children[0]->keys.push_back(3);
  root->children[0]->children[0]->keys.push_back(4);

  root->children[0]->children.push_back(new BTNode());
  root->children[0]->children[1]->isLeaf = true;
  root->children[0]->children[1]->keys.push_back(12);
  root->children[0]->children[1]->keys.push_back(13);

  root->children[0]->children.push_back(new BTNode());
  root->children[0]->children[2]->isLeaf = true;
  root->children[0]->children[2]->keys.push_back(20);
  root->children[0]->children[2]->keys.push_back(25);

  root->children[0]->children.push_back(new BTNode());
  root->children[0]->children[3]->isLeaf = true;
  root->children[0]->children[3]->keys.push_back(33);
  root->children[0]->children[3]->keys.push_back(38);

  root->children[0]->children.push_back(new BTNode());
  root->children[0]->children[4]->isLeaf = true;
  root->children[0]->children[4]->keys.push_back(43);
  root->children[0]->children[4]->keys.push_back(45);

  root->children.push_back(new BTNode());
  root->children[1]->isLeaf = false;
  root->children[1]->keys.push_back(60);
  root->children[1]->keys.push_back(80);

  root->children[1]->children.push_back(new BTNode());
  root->children[1]->children[0]->isLeaf = true;
  root->children[1]->children[0]->keys.push_back(52);
  root->children[1]->children[0]->keys.push_back(56);
  root->children[1]->children[0]->keys.push_back(58);

  root->children[1]->children.push_back(new BTNode());
  root->children[1]->children[1]->isLeaf = true;
  root->children[1]->children[1]->keys.push_back(65);
  root->children[1]->children[1]->keys.push_back(70);

  root->children[1]->children.push_back(new BTNode());
  root->children[1]->children[2]->isLeaf = true;
  root->children[1]->children[2]->keys.push_back(85);
  root->children[1]->children[2]->keys.push_back(90);

  // BTNode* result = root->search(7);
  // if (result != nullptr) std::cout << "Key found!" << std::endl;
  // else std::cout << "Key not found!" << std::endl;

  root->insert(7);

  // result = root->search(7);
  // if (result != nullptr) std::cout << "Key found!" << std::endl;
  // else std::cout << "Key not found!" << std::endl;

  return 0;
}