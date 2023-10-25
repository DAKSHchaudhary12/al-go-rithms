#include <iostream>
#include <vector>

const int MAX_KEYS = 3;

struct Node {
    bool isLeaf;
    std::vector<int> keys;
    std::vector<Node*> children;

    Node(bool leaf = true) : isLeaf(leaf) {}

    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }
};

class BPlusTree {
public:
    BPlusTree() : root(new Node()) {}

    ~BPlusTree() {
        delete root;
    }

    void insert(int key) {
        if (root->keys.size() == MAX_KEYS) {
            Node* newRoot = new Node(false);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0);
            root = newRoot;
        }
        insertNonFull(root, key);
    }

    bool search(int key) {
        return searchKey(root, key);
    }

private:
    Node* root;

    void insertNonFull(Node* node, int key) {
        int i = node->keys.size() - 1;
        if (node->isLeaf) {
            while (i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
        } else {
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;
            if (node->children[i]->keys.size() == MAX_KEYS) {
                splitChild(node, i);
                if (key > node->keys[i]) {
                    i++;
                }
            }
            insertNonFull(node->children[i], key);
        }
    }

    void splitChild(Node* parent, int childIndex) {
        Node* child = parent->children[childIndex];
        Node* newChild = new Node(child->isLeaf);
        parent->keys.insert(parent->keys.begin() + childIndex, child->keys[MAX_KEYS / 2]);
        parent->children.insert(parent->children.begin() + childIndex + 1, newChild);

        newChild->keys.assign(child->keys.begin() + MAX_KEYS / 2 + 1, child->keys.end());
        child->keys.resize(MAX_KEYS / 2);

        if (!child->isLeaf) {
            newChild->children.assign(child->children.begin() + MAX_KEYS / 2 + 1, child->children.end());
            child->children.resize(MAX_KEYS / 2 + 1);
        }
    }

    bool searchKey(Node* node, int key) {
        int i = 0;
        while (i < node->keys.size() && key > node->keys[i]) {
            i++;
        }
        if (i < node->keys.size() && key == node->keys[i]) {
            return true;
        } else if (node->isLeaf) {
            return false;
        } else {
            return searchKey(node->children[i], key);
        }
    }
};

int main() {
    BPlusTree tree;
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    tree.insert(9);
    tree.insert(5);

    std::cout << "Search 5: " << (tree.search(5) ? "Found" : "Not Found") << std::endl;
    std::cout << "Search 8: " << (tree.search(8) ? "Found" : "Not Found") << std::endl;

    return 0;
}
