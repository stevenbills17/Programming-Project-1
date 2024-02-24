#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <fstream>
using namespace std;

// Node structure
struct TreeNode {
    string key; // for holding the data
    TreeNode *left, *right; // left and right child

    // Constructor for initializing the fields using initializer list
    TreeNode(string k): key(k), left(nullptr), right(nullptr) {}
};

TreeNode* CreateExpressionTree(string& exp) {

    // Stacks to hold TreeNodes and Operators
    stack<TreeNode*> nodeStack;
    stack<char> operatorStack;

    // Setting the priority according to the table given in the problem
    // We can use a dictionary (or map in C++) or just use an array as a dictionary
    // Ascii value of (, ), +, -, *, / are under 50
    // so we can use an array of 50 elements, and assign them any number greater than 2 because
    // in the given table 0 is highest so more the value, weaker will be its priority and since we're using values
    // upto 2, we can assign them any number from 3 onwards, I have taken 100
    int priority[50]; //
    for(int i=0; i<50; i++) priority[i] = 100; // 100 shows weakest priority, we could have used 3 or onwards

    // Now assigning priority according to the table
    priority['('] = priority[')'] = 0;
    priority['*'] = priority['/'] = 1;
    priority['+'] = priority['-'] = 2;

    // Now start making our expression tree
    for (int i = 0; i < exp.length(); i++) {

        // If we encounter '(' add that to operatorStack
        if (exp[i] == '(')
            operatorStack.push(exp[i]);

            // If the character is a number then push to nodeStack
        else if (isdigit(exp[i])) {
            string number = "";
            number += exp[i];
            while(isdigit(exp[i+1]) || exp[i+1] == '.') {
                number += exp[i+1];
                i++;
            }
            TreeNode* node = new TreeNode(number);
            nodeStack.push(node);
        }

            // If we encounter ')', then we must remove the operator from operatorStack until we find its
            // corresponding '(' counter part or stack becomes empty, and in this removal process, if we find any other operators (+,-,*,/)
            // then we must attach it to two Nodes. We can do it by remove two nodes from nodeStack, then attach them
            // using removed operator and put back that modified node again.
        else if (exp[i] == ')') {
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                // get operator and create a node from this operator
                string o (1, operatorStack.top());
                TreeNode* operator_root = new TreeNode(o);
                operatorStack.pop();

                // Get two nodes from nodeStack
                TreeNode* node1 = nullptr, *node2 = nullptr;
                if (nodeStack.size() >= 2) {
                    node1 = nodeStack.top();
                    nodeStack.pop();
                    node2 = nodeStack.top();
                    nodeStack.pop();
                } else {
                    cout << "Invalid expression due to operator " << '\n';
                    return nullptr;
                }

                // Create tree from operator as root and nodes as its children
                operator_root->left = node2;
                operator_root->right = node1;
                nodeStack.push(operator_root);
            }
            if (!operatorStack.empty())
                operatorStack.pop();
            else {
                cout << "Invalid expression due to brackets mismatch" << '\n';
                return nullptr;
            }

        }

            // We come here means, character, exp[i] is not number or '(' so it must
            // one of the operator from +,-,*,/,)
            // First checking for main four operator
        else if (priority[exp[i]] <= 2) {
            // If the current operator priority is is lower than or equal the priority of operator
            // at the top of operatorStack, then remove the operator until it becomes less than the priority of the current operator
            // at top and create it as root of TreeNode and add two nodes from nodeStack and put back them
            while (!operatorStack.empty() && operatorStack.top() != '(' && priority[operatorStack.top()] <= priority[exp[i]]) {
                // get operator and create a node from this operator

                // Check for invalid case
                if (operatorStack.top() == ')' || operatorStack.top() == '(') {
                    cout << "Invalid Expression due to operator!" << operatorStack.top() << '\n';
                    return nullptr;
                }

                string o (1, operatorStack.top());
                TreeNode* operator_root = new TreeNode(o);
                operatorStack.pop();

                // Get two nodes from nodeStack
                TreeNode* node1 = nullptr, *node2 = nullptr;
                if (nodeStack.size() >= 2) {
                    node1 = nodeStack.top();
                    nodeStack.pop();
                    node2 = nodeStack.top();
                    nodeStack.pop();
                } else {
                    cout << "Invalid expression due to operator " << '\n';
                    return nullptr;
                }

                // Create tree from operator as root and nodes as its children
                operator_root->left = node2;
                operator_root->right = node1;
                nodeStack.push(operator_root);
            }
            // We come here means either the current operator has more priority now, or stack is empty or top element in stack is '('
            // In either case we can push now
            operatorStack.push(exp[i]);
        }
    }

    // If operator is still there in the stack, remove them and add nodes
    while (!operatorStack.empty()) {
        // First check if we have any invalid operator
        if (operatorStack.top() == ')' || operatorStack.top() == '(') {
            cout << "Invalid Expression due to brackets!" << '\n';
            return nullptr;
        }

        // get operator and create a node from this operator
        string o (1, operatorStack.top());
        TreeNode* operator_root = new TreeNode(o);
        operatorStack.pop();

        TreeNode* node1 = nullptr, *node2 = nullptr;
        if (nodeStack.size() >= 2) {
            node1 = nodeStack.top();
            nodeStack.pop();
            node2 = nodeStack.top();
            nodeStack.pop();
        } else {
            cout << "Invalid expression due to operator " << '\n';
            return nullptr;
        }

        // Create tree from operator as root and nodes as its children
        operator_root->left = node2;
        operator_root->right = node1;
        nodeStack.push(operator_root);
    }

    TreeNode* root = nodeStack.top();
    return root;
}

// Function to print post-order
void postorder(TreeNode* root) {
    if (root) {
        postorder(root->left);
        postorder(root->right);
        cout << root->key << " ";
    }
}

// Function to print pre-order
void preorder(TreeNode* root) {
    if (root) {
        cout << root->key << " ";
        preorder(root->left);
        preorder(root->right);
    }
}

// Function to print in-order
void inorder(TreeNode* root) {
    if (root) {
        inorder(root->left);
        cout << root->key << " ";
        inorder(root->right);
    }
}

// Evaluate tree
double evaluateTree(TreeNode* root) {

    if (!root) return 0.0;

    // For leaf node, just return its value
    if (!root->left && !root->right)
        return stod(root->key); // string to double

    // Evaluate left subtree
    double left_answer = evaluateTree(root->left);

    // Evaluate right subtree
    double right_answer = evaluateTree(root->right);

    // Check which operator to apply
    if (root->key == "+")
        return (double)left_answer + right_answer;

    if (root->key == "-")
        return (double)left_answer - right_answer;

    if (root->key == "*")
        return (double )left_answer * right_answer;

    return (double )left_answer/right_answer;
}

// Driver code
int main() {

    vector<string> exps; // for storing expr
    string exp;
    ifstream reader("sample.txt");

    if(reader.is_open()) {
        while(reader) {
            reader >> exp;
            exps.push_back(exp);
        }
    } else {
        cout << "Couldn't open the file!" << '\n';
        return 0;
    }

    cout << "-------------------------------------------\n";
    for(auto& exp : exps) {
        cout << "Input expression : " << exp << '\n';
        TreeNode *root = CreateExpressionTree(exp);
        // Function call
        cout << "Inorder: ";
        inorder(root);
        cout << '\n';

        cout << "Preorder: ";
        preorder(root);
        cout << '\n';

        cout << "Postorder: ";
        postorder(root);
        cout << '\n';

        if (!root) {
            cout << "Cannot evaluate invalid expression!" << '\n';
        } else {
            cout << "Evaluation: " << evaluateTree(root) << '\n';
        }
        cout << "-------------------------------------------\n";
    }

    return 0;
}
