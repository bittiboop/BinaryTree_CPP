#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <algorithm>

using namespace std;

struct DictionaryNode {
    string english;
    string ukrainian;
    int accessCount;
    DictionaryNode* left;
    DictionaryNode* right;
    
    DictionaryNode(string eng, string ukr, int count = 0) 
        : english(eng), ukrainian(ukr), accessCount(count), left(nullptr), right(nullptr) {}
};

class Dictionary {
private:
    DictionaryNode* root;
    
    DictionaryNode* insertNode(DictionaryNode* node, string eng, string ukr, int count) {
        if (node == nullptr) {
            return new DictionaryNode(eng, ukr, count);
        }
        
        if (eng < node->english) {
            node->left = insertNode(node->left, eng, ukr, count);
        } else if (eng > node->english) {
            node->right = insertNode(node->right, eng, ukr, count);
        } else {
            node->ukrainian = ukr;
            node->accessCount = count;
        }
        
        return node;
    }
    
    DictionaryNode* findNode(DictionaryNode* node, string eng) {
        if (node == nullptr || node->english == eng) {
            return node;
        }
        
        if (eng < node->english) {
            return findNode(node->left, eng);
        } else {
            return findNode(node->right, eng);
        }
    }
    
    DictionaryNode* deleteNode(DictionaryNode* node, string eng) {
        if (node == nullptr) {
            return nullptr;
        }
        
        if (eng < node->english) {
            node->left = deleteNode(node->left, eng);
        } else if (eng > node->english) {
            node->right = deleteNode(node->right, eng);
        } else {
            
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr) {
                DictionaryNode* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                DictionaryNode* temp = node->left;
                delete node;
                return temp;
            }
            else {
                DictionaryNode* temp = node->right;
                while (temp->left != nullptr) {
                    temp = temp->left;
                }
                
                node->english = temp->english;
                node->ukrainian = temp->ukrainian;
                node->accessCount = temp->accessCount;
                
                node->right = deleteNode(node->right, temp->english);
            }
        }
        
        return node;
    }

    
    void collectWords(DictionaryNode* node, vector<pair<string, int>>& words) {
        if (node == nullptr) {
            return;
        }
        
        collectWords(node->left, words);
        words.push_back({node->english, node->accessCount});
        collectWords(node->right, words);
    }
    
    void inorderTraversal(DictionaryNode* node) {
        if (node == nullptr) {
            return;
        }
        
        inorderTraversal(node->left);
        cout << node->english << " - " << node->ukrainian << " (Access count: " << node->accessCount << ")" << endl;
        inorderTraversal(node->right);
    }
    
    void destroyTree(DictionaryNode* node) {
        if (node == nullptr) {
            return;
        }
        
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
    
public:
    Dictionary() : root(nullptr) {}
    
    ~Dictionary() {
        destroyTree(root);
    }
    
    void addWord(string eng, string ukr, int count = 0) {
        root = insertNode(root, eng, ukr, count);
    }
    
    bool findAndDisplayWord(string eng) {
        DictionaryNode* node = findNode(root, eng);
        if (node == nullptr) {
            cout << "Word '" << eng << "' not found in dictionary." << endl;
            return false;
        }
        
        node->accessCount++;
        
        cout << "Word: " << node->english << endl;
        cout << "Translation: " << node->ukrainian << endl;
        cout << "Access count: " << node->accessCount << endl;
        return true;
    }
    
    bool deleteWord(string eng) {
        DictionaryNode* node = findNode(root, eng);
        if (node == nullptr) {
            cout << "Word '" << eng << "' not found in dictionary." << endl;
            return false;
        }
        
        root = deleteNode(root, eng);
        cout << "Word '" << eng << "' deleted from dictionary." << endl;
        return true;
    }
    
    void displayDictionary() {
        if (root == nullptr) {
            cout << "Dictionary is empty." << endl;
            return;
        }
        
        cout << "Dictionary contents:" << endl;
        inorderTraversal(root);
    }
    
    void displayTopPopular() {
        vector<pair<string, int>> words;
        collectWords(root, words);
        
        if (words.empty()) {
            cout << "Dictionary is empty :(" << endl;
            return;
        }
        
        sort(words.begin(), words.end(), 
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });
        
        cout << "Top 3 most popular words:" << endl;
        int count = min(3, (int)words.size());
        for (int i = 0; i < count; i++) {
            DictionaryNode* node = findNode(root, words[i].first);
            cout << i + 1 << ". " << node->english << " - " << node->ukrainian 
                 << " (Access count: " << node->accessCount << ")" << endl;
        }
    }
    
    void displayLeastPopular() {
        vector<pair<string, int>> words;
        collectWords(root, words);
        
        if (words.empty()) {
            cout << "Dictionary is empty :(" << endl;
            return;
        }
        
        sort(words.begin(), words.end(), 
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second < b.second;
             });
        
        cout << "Top 3 least popular words:" << endl;
        int count = min(3, (int)words.size());
        for (int i = 0; i < count; i++) {
            DictionaryNode* node = findNode(root, words[i].first);
            cout << i + 1 << ". " << node->english << " - " << node->ukrainian 
                 << " (Access count: " << node->accessCount << ")" << endl;
        }
    }
};


int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Dictionary dict;
    int choice;
    string english, ukrainian;
    int count;

    
    cout << "Initializing dictionary with sample words..." << endl;
    dict.addWord("hello", "привіт", 5);
    dict.addWord("world", "світ", 3);
    dict.addWord("book", "книга", 7);
    dict.addWord("computer", "комп'ютер", 10);
    dict.addWord("university", "університет", 2);
    dict.addWord("student", "студент", 8);
    dict.addWord("professor", "професор", 1);
    
    while (true)
    {
        cout << "\n=== English-Ukrainian Dictionary ===" << endl;
        cout << "1. Display dictionary" << endl;
        cout << "2. Find and display word" << endl;
        cout << "3. Add or update word" << endl;
        cout << "4. Delete word" << endl;
        cout << "5. Show top 3 most popular words" << endl;
        cout << "6. Show top 3 least popular words" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
        case 1:
            dict.displayDictionary();
            break;
                
        case 2:
            cout << "Enter English word: ";
            cin >> english;
            dict.findAndDisplayWord(english);
            break;
                
        case 3:
            cout << "Enter English word: ";
            cin >> english;
            cout << "Enter Ukrainian translation: ";
            cin >> ukrainian;
            cout << "Enter initial access count (0 for new words): ";
            cin >> count;
            dict.addWord(english, ukrainian, count);
            cout << "Word added/updated successfully." << endl;
            break;
                
        case 4:
            cout << "Enter English word to delete: ";
            cin >> english;
            dict.deleteWord(english);
            break;
                
        case 5:
            dict.displayTopPopular();
            break;
                
        case 6:
            dict.displayLeastPopular();
            break;
                
        case 0:
            cout << "Exiting program. Goodbye!" << endl;
            return 0;
                
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    
    return 0;   
}
