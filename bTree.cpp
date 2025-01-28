#include <iostream>
#include <vector>
#include <sstream>
#include <ostream>

#define treeOrder 2

struct StockItem {
  int id;
  std::string name;
  int quantityInStock;

  std::string toString() const;
};
std::ostream& operator << (std::ostream &os, const StockItem &s) {
    return (os << "{" << s.id << ", " << s.name << ", " << s.quantityInStock << "}" << std::endl);
}

std::string StockItem::toString() const {
  std::ostringstream oss;
  oss << "{" << id << ", " << name << ", " << quantityInStock << "}";
  return oss.str();
}

struct BTNode {
  std::vector<StockItem*> data;
  std::vector<BTNode*> children;
  bool isLeaf;

  StockItem *search(int key){
    int i = 0;
    while (i < data.size() && key > data[i]->id) i++;
    if (i < data.size() && key == data[i]->id) return this->data[i];
    if (isLeaf) return nullptr;
    return children[i]->search(key);
  }

  void insert(StockItem *item){
    if (isLeaf){
      int i = 0;
      while (i < data.size() && item->id > data[i]->id) i++;
      data.insert(data.begin() + i, item);
      return;
    }
    int i = 0;
    while (i < data.size() && item->id > data[i]->id) i++;
    children[i]->insert(item);
    if (children[i]->data.size() > 2 * treeOrder) split(i);
  }

  void split(int i){
    BTNode *newChild = new BTNode();
    BTNode *child = children[i];
    StockItem *keyToElevate = child->data[treeOrder];
    newChild->isLeaf = child->isLeaf;
    newChild->data = std::vector<StockItem*>(child->data.begin() + treeOrder + 1, child->data.end());
    child->data = std::vector<StockItem*>(child->data.begin(), child->data.begin() + treeOrder);
    if (!child->isLeaf){
      newChild->children = std::vector<BTNode*>(child->children.begin() + treeOrder + 1, child->children.end());
      child->children = std::vector<BTNode*>(child->children.begin(), child->children.begin() + treeOrder + 1);
    }
    data.insert(data.begin() + i, keyToElevate);
    children.insert(children.begin() + i + 1, newChild);
  }
};



int main() {

  BTNode *root = new BTNode();
  root->data.push_back(new StockItem({50, "Item 50", 0}));
  root->isLeaf = false;

  root->children.push_back(new BTNode());
  root->isLeaf = false;
  root->children[0]->data.push_back(new StockItem({9, "Item 9", 0}));
  root->children[0]->data.push_back(new StockItem({16, "Item 16", 0}));
  root->children[0]->data.push_back(new StockItem({30, "Item 30", 0}));
  root->children[0]->data.push_back(new StockItem({40, "Item 40", 0}));

  root->children[0]->children.push_back(new BTNode());
  root->children[0]->children[0]->isLeaf = true;;
  root->children[0]->children[0]->data.push_back(new StockItem({1, "Item 1", 0}));
  root->children[0]->children[0]->data.push_back(new StockItem({2, "Item 2", 0}));
  root->children[0]->children[0]->data.push_back(new StockItem({3, "Item 3", 0}));
  root->children[0]->children[0]->data.push_back(new StockItem({4, "Item 4", 0}));

  root->children[0]->children.push_back(new BTNode());
  root->children[0]->children[1]->isLeaf = true;
  root->children[0]->children[1]->data.push_back(new StockItem({12, "Item 12", 0}));
  root->children[0]->children[1]->data.push_back(new StockItem({13, "Item 13", 0}));

  root->children[0]->children.push_back(new BTNode());
  root->children[0]->children[2]->isLeaf = true;
  root->children[0]->children[2]->data.push_back(new StockItem({20, "Item 20", 0}));
  root->children[0]->children[2]->data.push_back(new StockItem({25, "Item 25", 0}));

  root->children[0]->children.push_back(new BTNode());
  root->children[0]->children[3]->isLeaf = true;
  root->children[0]->children[3]->data.push_back(new StockItem({33, "Item 33", 0}));
  root->children[0]->children[3]->data.push_back(new StockItem({38, "Item 38", 0}));

  root->children[0]->children.push_back(new BTNode());
  root->children[0]->children[4]->isLeaf = true;
  root->children[0]->children[4]->data.push_back(new StockItem({43, "Item 43", 0}));
  root->children[0]->children[4]->data.push_back(new StockItem({45, "Item 45", 0}));

  root->children.push_back(new BTNode());
  root->children[1]->isLeaf = false;
  root->children[1]->data.push_back(new StockItem({60, "Item 60", 0}));
  root->children[1]->data.push_back(new StockItem({80, "Item 80", 0}));

  root->children[1]->children.push_back(new BTNode());
  root->children[1]->children[0]->isLeaf = true;
  root->children[1]->children[0]->data.push_back(new StockItem({52, "Item 52", 0}));
  root->children[1]->children[0]->data.push_back(new StockItem({56, "Item 56", 0}));
  root->children[1]->children[0]->data.push_back(new StockItem({58, "Item 58", 0}));

  root->children[1]->children.push_back(new BTNode());
  root->children[1]->children[1]->isLeaf = true;
  root->children[1]->children[1]->data.push_back(new StockItem({65, "Item 65", 0}));
  root->children[1]->children[1]->data.push_back(new StockItem({70, "Item 70", 0}));

  root->children[1]->children.push_back(new BTNode());
  root->children[1]->children[2]->isLeaf = true;
  root->children[1]->children[2]->data.push_back(new StockItem({85, "Item 85", 0}));
  root->children[1]->children[2]->data.push_back(new StockItem({90, "Item 90", 0}));

  StockItem *result = root->search(50);
  if (result != nullptr) std::cout << *result;
  else std::cout << "Key not found!" << std::endl;

  root->insert(new StockItem({7, "Item 7", 0}));

  result = root->search(7);
  if (result != nullptr) std::cout << *result;
  else std::cout << "Key not found!" << std::endl;

  return 0;
}