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

  // REMOVE FUNCTION AND NEEDED TOOLS

  void remove(int key) {
    int idx = 0;
    while (idx < data.size() && key > data[idx]->id) idx++;

    // Case 1: The key to be removed is present in this node
    if (idx < data.size() && data[idx]->id == key) {
      if (isLeaf) {
        // Case 1a: The node is a leaf, simply remove the key
        data.erase(data.begin() + idx);
      } else {
        // Case 1b: The node is not a leaf
        BTNode *pred = children[idx];
        BTNode *succ = children[idx + 1];

        // Case 1b-i: The predecessor child has at least treeOrder keys
        if (pred->data.size() >= treeOrder + 1) {
          StockItem *predItem = getPred(idx);
          data[idx] = predItem;
          children[idx]->remove(predItem->id);
        }
        // Case 1b-ii: The successor child has at least treeOrder keys
        else if (succ->data.size() >= treeOrder + 1) {
          StockItem *succItem = getSucc(idx);
          data[idx] = succItem;
          children[idx + 1]->remove(succItem->id);
        }
        // Case 1b-iii: Both predecessor and successor have less than treeOrder keys, merge them
        else {
          merge(idx);
          children[idx]->remove(key);
        }
      }
    } else {
      // Case 2: The key to be removed is not present in this node
      if (isLeaf) {
        std::cout << "Key not found!" << std::endl;
        return;
      }

      bool flag = (idx == data.size());

      // Case 2a: The child where the key is supposed to exist has less than treeOrder keys
      if (children[idx]->data.size() < treeOrder + 1) {
        fill(idx);
      }

      // If the last child has been merged, it must have merged with the previous child
      if (flag && idx > data.size()) {
        children[idx - 1]->remove(key);
      } else {
        children[idx]->remove(key);
      }
    }
  }

  StockItem* getPred(int idx) {
    BTNode *curr = children[idx];
    while (!curr->isLeaf) {
      curr = curr->children[curr->data.size()];
    }
    return curr->data[curr->data.size() - 1];
  }

  StockItem* getSucc(int idx) {
    BTNode *curr = children[idx + 1];
    while (!curr->isLeaf) {
      curr = curr->children[0];
    }
    return curr->data[0];
  }

  void fill(int idx) {
    // Borrow a key from the previous child
    if (idx != 0 && children[idx - 1]->data.size() >= treeOrder + 1) {
      borrowFromPrev(idx);
    }
    // Borrow a key from the next child
    else if (idx != data.size() && children[idx + 1]->data.size() >= treeOrder + 1) {
      borrowFromNext(idx);
    }
    // Merge with the previous or next child if borrowing is not possible
    else {
      if (idx != data.size()) {
        merge(idx);
      } else {
        merge(idx - 1);
      }
    }
  }

  void borrowFromPrev(int idx) {
    BTNode *child = children[idx];
    BTNode *sibling = children[idx - 1];

    child->data.insert(child->data.begin(), data[idx - 1]);
    data[idx - 1] = sibling->data[sibling->data.size() - 1];
    sibling->data.pop_back();

    if (!child->isLeaf) {
      child->children.insert(child->children.begin(), sibling->children[sibling->children.size() - 1]);
      sibling->children.pop_back();
    }
  }

  void borrowFromNext(int idx) {
    BTNode *child = children[idx];
    BTNode *sibling = children[idx + 1];

    child->data.push_back(data[idx]);
    data[idx] = sibling->data[0];
    sibling->data.erase(sibling->data.begin());

    if (!child->isLeaf) {
      child->children.push_back(sibling->children[0]);
      sibling->children.erase(sibling->children.begin());
    }
  }

  void merge(int idx) {
    BTNode *child = children[idx];
    BTNode *sibling = children[idx + 1];

    child->data.push_back(data[idx]);
    for (int i = 0; i < sibling->data.size(); i++) {
      child->data.push_back(sibling->data[i]);
    }

    if (!child->isLeaf) {
      for (int i = 0; i < sibling->children.size(); i++) {
        child->children.push_back(sibling->children[i]);
      }
    }

    data.erase(data.begin() + idx);
    children.erase(children.begin() + idx + 1);

    delete sibling;
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

  StockItem *result = root->search(52);
  if (result != nullptr) std::cout << "Found: " << *result;
  else std::cout << "Key 52 not found!" << std::endl;

  root->remove(52);
  result = root->search(52);
  if (result != nullptr) std::cout << "Found: " << *result;
  else std::cout << "Key 52 not found!" << std::endl;

  root->insert(new StockItem({7, "Item 7", 0}));

  result = root->search(7);
  if (result != nullptr) std::cout << *result;
  else std::cout << "Key not found!" << std::endl;

  return 0;
}