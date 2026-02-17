#include <string>
#include <map>
#include <iostream>
#include "User.h"

struct TrieNode {
    std::map<char, TrieNode*> children;
    User* userPtr;
    bool isEndOfWord;
    TrieNode() : userPtr(nullptr), isEndOfWord(false) {}
};

class UserManager {
private:
    TrieNode* root;
    User* currentUser;

    void clearTrie(TrieNode* node);
    std::string toLower(std::string str);
    void searchRecursive(TrieNode* node, std::string prefix); // برای DFS 
    // متد های کمکی اسمارت سرچ
    int calculateHammingDistance(std::string s1, std::string s2);
    void findHammingMatches(TrieNode* node, std::string currentWord, const std::string& target, int threshold);

public:
    UserManager();
    ~UserManager();

    bool isValidUsername(const std::string& username);
    bool isValidPassword(const std::string& password);
    bool isAnyOneLoggedin();
    bool signup(std::string username, std::string password);
    bool login(std::string username, std::string password); 
    void logout();

    User* findUser(std::string username);
    void searchUserPrefix(std::string prefix); 
    void smartSearch(std::string term);
    User* getCurrentUser() const;
};
