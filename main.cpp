/* Red Black Tree Insertion: self-balancing binary tree which has red or black nodes
   Author: Ayan Masud
   Date: 4/10/2025

   Credit:
   - https://en.wikipedia.org/wiki/Red%E2%80%93black_tree (tables were useful for identifying insertion and deletion cases)
   - https://www.youtube.com/watch?v=CTvfzU_uNKE (cases on whiteboard for deletion was useful for visualization)

   Visualizer used for testing:
   - https://www.cs.usfca.edu/~galles/visualization/RedBlack.html (reason why i used in order predecessor)
 */

 #include <iostream>
 #include <cstring>
 #include <string>
 #include <fstream>
 #include <sstream>
 
 // define color codes
 #define RESET   "\033[0m"
 #define WHITE   "\033[37m"
 #define RED     "\033[31m"
 
 using namespace std;
 
 class btn { // binary tree node class
 private:
   int value;
   int color; // 0 -> black, 1 -> red
   btn* left;
   btn* right;
   btn* parent;
 public:
   // constructor requires setting its value
   btn(int val) {
     value = val;
     left = nullptr;
     right = nullptr;
     color = 0;
     parent = nullptr;
   }
 
   // destructor
   ~btn() {
     left = nullptr;
     right = nullptr;
     parent = nullptr;
   }
 
   // get parent
   btn* getParent() {
     return parent;
   }
 
   // set parent
   void setParent(btn* node) {
     parent = node;
   }
 
   // get color
   int getColor() {
     return color;
   }
 
   // set color
   void setColor(int newColor) {
     color = newColor;
   }
 
   // set left child
   void setLeft(btn* node) {
     left = node;
   }
 
   // set right child
   void setRight(btn* node) {
     right = node;
   }
 
   // get left
   btn* getLeft() {
     return left;
   }
 
   // get the right child
   btn* getRight() {
     return right;
   }
 
   // get the char value
   int getValue() {
     return value;
   }

   // get the char value
   void setValue(int newVal) {
    value = newVal;
   }
 };
 
 // prototypes
 void add(btn* &head, btn* current, btn* added);
 void print(btn* current, int depth);
 void cases(btn* &head, btn* node);
 void rotateLeft(btn* &head, btn* node);
 void rotateRight(btn* &head, btn* node);
 void search(btn* &head, btn* current, int val);
 void del(btn* &head, btn* current, int val);
 void delCases(btn* &head, btn* node);
 
 int main() {
  // instructions
  cout << "'ADD' to add a number" << endl;
  cout << "'DELETE' to delete a number" << endl;
  cout << "'SEARCH' to see if a number exists" << endl;
  cout << "'PRINT' to print the tree" << endl;
  cout << "'QUIT' to leave" << endl;

  btn* head = new btn(0); // head of the tree

  while (true) {
    char cmd[8];
    cin.getline(cmd, 8);

    if (strcmp(cmd, "ADD") == 0) { // add command
      cout << "through file (f) or manually (m)? ";
      char fm[2];
      cin.getline(fm, 2);

      if (strcmp(fm, "m") == 0) { // manually add numbers to the tree
        int num;
        cout << "add num: ";
        cin >> num;
        cin.ignore();
        btn* added = new btn(num);
        add(head, head, added);
      }
      else if (strcmp(fm, "f") == 0) { // add through file
        // prepare the tree using file
        ifstream file("nums.txt");
        if (file.is_open()) {
          string line;
          while (getline(file, line)) {
            stringstream ss(line);
            string word;
            while (ss >> word) { // string stream helps with the splitting by spaces
              int num = stoi(word); // convert the string to int
              btn* added = new btn(num);
              add(head, head, added);
            }
          }
          file.close();
        } else { // files not there
          cout << "Unable to open file";
        }
      }
    }
    else if (strcmp(cmd, "DELETE") == 0) { // delete command
      int num;
      cout << "which num: ";
      cin >> num;
      
      //if (head->getValue() == num) { // deleting the head. use next larger
      //btn* temp = head->getRight();
      //delHead(head, temp);
      //}
      //else { // deleting other than head
      del(head, head, num);
	//}
    }
    else if (strcmp(cmd, "SEARCH") == 0) { // delete command
      int num;
      cout << "which num: ";
      cin >> num;

      search(head, head, num);
    }
    else if (strcmp(cmd, "PRINT") == 0) { // print command
      print(head, 0);
    }
    else if (strcmp(cmd, "QUIT") == 0) { // quit command
      break;
    }
  }

  return 0;
}

// add a node to the tree
void add(btn* &head, btn* current, btn* added) {
  if (head->getValue() == 0) { // empty
    added->setColor(0); // set the heads color to black
    head = added;
    return;
  }

  if (added->getValue() < current->getValue()) { // value is bigger so go right
    if (current->getLeft() == nullptr) {
      added->setColor(1); // set the new node to red
      added->setParent(current); // set parent value
      current->setLeft(added);
      cases(head, added); // check cases
    } 
    else {
      add(head, current->getLeft(), added); // recurse right
    }
  } 
  else if (added->getValue() > current->getValue()) { // value is less so go left
    if (current->getRight() == nullptr) {
      added->setColor(1); // set the new node to red
      added->setParent(current); // set parent value
      current->setRight(added);
      cases(head, added); // check cases
    } 
    else {
      add(head, current->getRight(), added); // recurse left
    }
  }
}

// print the red black tree
void print(btn* current, int depth) {
  if (current == nullptr || current->getValue() == 0) { // stop
    return;
  }
  print(current->getRight(), depth + 1); // sideways tree so print rights
  for (int i = 0; i < depth; i++) { // add spaces based off the depth
    cout << "  ";
  }

  // print the value
  if (current->getColor() == 0 || current->getColor() == 3) { // black node
    cout << WHITE << current->getValue() << RESET << endl;
  }
  else { // red node
    cout << RED << current->getValue() << RESET << endl;
  }
  print(current->getLeft(), depth + 1); // then print lefts
}

// p -> parent
// x -> node we are rotating on
// y -> child that will replace x at the top
// a, b, c -> the children

// rotate clockwise
void rotateLeft(btn* &head, btn* x) {
  btn* p = x->getParent();
  btn* y = x->getRight(); // new root of this subtree
  btn* b = y->getLeft();  // this subtree moves under x

  // Reconnect the parent
  if (p == nullptr) {
    head = y; // y becomes the new root
    y->setParent(nullptr);
  } 
  else {
    if (p->getLeft() == x) {
      p->setLeft(y);
    } 
    else {
      p->setRight(y);
    }
    y->setParent(p);
  }

  // Rebuild subtree
  y->setLeft(x);
  x->setParent(y);

  x->setRight(b);
  if (b != nullptr) {
    b->setParent(x);
  }
}

// rotate counterclockwise
void rotateRight(btn* &head, btn* x) {
  btn* p = x->getParent();
  btn* y = x->getLeft();   // new root of this subtree
  btn* b = y->getRight();  // this subtree moves under x

  // Reconnect the parent
  if (p == nullptr) {
    head = y; // y becomes the new root
    y->setParent(nullptr);
  } 
  else {
    if (p->getLeft() == x) {
      p->setLeft(y);
    } 
    else {
      p->setRight(y);
    }
    y->setParent(p);
  }

  // Rebuild subtree
  y->setRight(x);
  x->setParent(y);

  x->setLeft(b);
  if (b != nullptr) {
    b->setParent(x);
  }
}

// check the insertion cases for individual nodes
void cases(btn* &head, btn* node) {
  // case 1: first node is root
  if (node->getParent() == nullptr) {
    node->setColor(0);
    head = node;
    return;
  }

  // case 2: parent is black, so do nothing
  if (node->getParent()->getColor() == 0) {
    return;
  }

  // make sure grandparent exists cause if it doesn't, no point in trying the next cases
  if (node->getParent()->getParent() == nullptr) {
    return;
  }

  // store the uncle to a variable
  btn* uncle = nullptr;
  if (node->getParent() == node->getParent()->getParent()->getLeft()) { // if nodes parent is the left of grandparent
    uncle = node->getParent()->getParent()->getRight(); // uncle is right of grandparent
  } else { // if nodes parent is the right of grandparent
    uncle = node->getParent()->getParent()->getLeft(); // uncle is left of grandparent
  }

  // case 3: parent and uncle are red
  if (uncle != nullptr && uncle->getColor() == 1) {
    node->getParent()->setColor(0);
    uncle->setColor(0);
    node->getParent()->getParent()->setColor(1);
    cases(head, node->getParent()->getParent()); // recursion on grandparent because the grandparent turning red might change things up
    return;
  }

  // case 4
  if (uncle == nullptr || uncle->getColor() == 0) { // uncle is black
    if (node->getParent() == node->getParent()->getParent()->getLeft()) { // parent is left of grandparent
      if (node == node->getParent()->getRight()) { // node is right child of parent
	rotateLeft(head, node->getParent()); // rotate to convert to line case
	node = node->getLeft(); // parent is now node
      }
      // case 5: node is left child of left parent
      node->getParent()->getParent()->setColor(1); // after rotation this makes sense
      rotateRight(head, node->getParent()->getParent());
    } 
    else { // parent is right of grandparent
      if (node == node->getParent()->getLeft()) { // node is left child of parent
	rotateRight(head, node->getParent());
	node = node->getRight(); // parent is now node
      }
      // case 5: node is right child of right parent
      node->getParent()->getParent()->setColor(1); // after rotation this makes sense
      rotateLeft(head, node->getParent()->getParent());
    }
  }

  node->getParent()->setColor(0); // ensure black parent after rotation
}

void search(btn* &head, btn* current, int val) {
  if (current == nullptr) { // stop
    return;
  }

  if (current->getValue() == val) { // found!
    cout << "This number exists." << endl;
  }

  if (current->getValue() < val) { // value is bigger so go right
    search(head, current->getRight(), val); // recurse right
  }

  if (current->getValue() > val) { // value is less so go left
    search(head, current->getLeft(), val); // recurse left
  }
}
 
 void del(btn* &head, btn* current, int val) {
  if (current == nullptr) {
    return;
  }

  if (current->getValue() == val) { // found!
    btn* nodeToDelete = current;
    
    // case 1: two children
    if (current->getLeft() != nullptr && current->getRight() != nullptr) {
      // using the in order predecessor
      btn* predecessor = current->getLeft();
      while (predecessor->getRight() != nullptr) {
        predecessor = predecessor->getRight();
      }
      
      // swap values of current and predecessor
      int temp = predecessor->getValue();
      predecessor->setValue(current->getValue());
      current->setValue(temp);
      
      // delete the predecessor
      del(head, current->getLeft(), val);
      return;
    }
    
    btn* child = nullptr;
    if (current->getLeft() != nullptr) {
      child = current->getLeft();
    }
    else if (current->getRight() != nullptr) {
      child = current->getRight();
    }
    
    // case 2: one child
    if (child != nullptr) {
      // replace current with child
      btn* parent = current->getParent();
      if (parent == nullptr) {
        head = child; // child becomes the new root
      }
      else {
        if (current == parent->getLeft()) {
          parent->setLeft(child);
        }
	else {
          parent->setRight(child);
        }
      }
      child->setParent(parent);
      child->setColor(0); // child turns black
      current->~btn();
      return;
    }
    
    // case 3: No children and is root
    if (current == head) {
      current->setValue(0); // this is where the tree goes empty
      return;
    }
    //cout << "before" << endl;
    // case 4: is leaf and red
    if (current->getColor() == 1) {
      //cout << "ran" << endl;
      btn* parent = current->getParent();
      if (parent->getLeft() == current) {
        parent->setLeft(nullptr);
      }
      else {
        parent->setRight(nullptr);
      }
      current->~btn();
      return;
    }
    else { // case 5: is leaf and black. double black happens
      //cout << "happened1" << endl;
      //current->setColor(2); // make it double black
      delCases(head, current);  // deletion fixer
    }

    // actually remove the node
    //cout << "happened2" << endl;
    btn* parent = current->getParent();
    cout << parent->getValue() << endl;
    if (parent->getLeft() == current) {
      parent->setLeft(nullptr);
    }
    else {
      parent->setRight(nullptr);
    }
    current->~btn();
    return;
  }

  if (current->getValue() < val) { // value is bigger so go right
    del(head, current->getRight(), val); // recurse right
  }
  
  if (current->getValue() > val) { // value is less so go left
    del(head, current->getLeft(), val); // recurse left
  }
}


void delCases(btn* &head, btn* node) {
  if (node == nullptr) {
    return;
  }
  
  // case 1: node is root
  if (node == head) {
    node->setColor(0);
      return;
  }
  
  btn* parent = node->getParent();
  btn* sibling = nullptr;
  if (node == parent->getLeft()) {
    sibling = parent->getRight();
  }
  else {
    sibling = parent->getLeft();
  }
  
  if (sibling == nullptr) {
    return; // shouldnt actually happen
  }
  
  // case 3: sibling is red
  if (sibling->getColor() == 1) {
    parent->setColor(1); // parent red
    sibling->setColor(0); // sibling red
    if (node == parent->getLeft()) { // rotation
      rotateLeft(head, parent);
    }
    else {
      rotateRight(head, parent);
    }
    delCases(head, node); // run through with the same node again
    return;
  }

  bool isLeftNephewRed = false;
  bool isRightNephewRed = false;
  if (sibling->getLeft() != nullptr && sibling->getLeft()->getColor() == 1) {
    isLeftNephewRed = true;
  }
  if (sibling->getRight() != nullptr && sibling->getRight()->getColor() == 1) {
    isRightNephewRed = true;
  }
  
  // case 2: parent, sibling, and nephews are black
  if (parent->getColor() == 0 && sibling->getColor() == 0 && 
      !isLeftNephewRed && !isRightNephewRed) {
    //cout << "happened3" << endl;
    cout << node->getValue() << endl;
    sibling->setColor(1);
    //node->setColor(0);
    //parent->setColor(2);
    delCases(head, parent); // parent is now double black
    return;
  }
  
  // case 4: parent is red, sibling and nephews are black
  if (parent->getColor() == 1 && sibling->getColor() == 0 && 
      !isLeftNephewRed && !isRightNephewRed) {
    sibling->setColor(1); // sibling red
    parent->setColor(0); // parent black
    return;
  }

  // case 5: prepare for case 6
  if (sibling->getColor() == 0) { // sibling black
    //cout << "ran" << endl;
    if (node == parent->getLeft() && 
	isLeftNephewRed && !isRightNephewRed) { // closer nephew is red
      sibling->setColor(1); // sibling red
      sibling->getLeft()->setColor(0); // closer nephew black
      rotateRight(head, sibling); // rotation
      sibling = parent->getRight(); // saved the new sibling in sibling variable
    }
    else if (node == parent->getRight() && 
	     !isLeftNephewRed && isRightNephewRed) { // basically the same stuff
      sibling->setColor(1);
      sibling->getRight()->setColor(0);
      rotateLeft(head, sibling);
      sibling = parent->getLeft();
    }
  }

  // case 6: sibling is black, with at least one red nephew in correct position
  sibling->setColor(parent->getColor()); // sibling is parents color
  parent->setColor(0); // parent black
  if (node == parent->getLeft()) {
    if (sibling->getRight() != nullptr) {
      sibling->getRight()->setColor(0); // set nephew to black
    }
    rotateLeft(head, parent); // rotation
  }
  else {
    if (sibling->getLeft() != nullptr) { // basically the same stuff
      sibling->getLeft()->setColor(0);
    }
    rotateRight(head, parent);
  }
}
