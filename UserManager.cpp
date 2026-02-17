#include "UserManager.h"
#include <cctype>

UserManager::UserManager() : currentUser(nullptr) 
{root = new TrieNode();}

UserManager::~UserManager() 
{clearTrie(root);}

void UserManager::clearTrie(TrieNode* node) 
{
    if (!node) 
        return;
    for (auto const& pair : node->children) 
        clearTrie(pair.second);
    if (node->userPtr) 
        delete node->userPtr; // پاک کردن یوزرها از حافظه
    delete node;
}

std::string UserManager::toLower(std::string str)
{
    for (char& c : str) 
        c = std::tolower(c);
    return str;
}

bool UserManager::isAnyOneLoggedin() 
{ return (currentUser) ? true : false; }

bool UserManager::isValidUsername(const std::string& username) 
{
    if (username.length() < 3 || username.length() > 30) 
    {
        std::cout << "Invalid username format.\n";
        std::cout << "Error: Username must be including least 3 and maximum 30 letter.\n";
        return false;
    }
    if (!std::isalnum(username[0])) 
    {
        std::cout << "Invalid username format.\n";
        std::cout << "Error: Username should start with alphabet or digit.\n";
        return false;
    }

    for (char c : username) 
        if (!std::isalnum(c) && c != '_' && c != '.') 
        {
            std::cout << "Invalid username format.\n";
            std::cout << "Error: Usernanem can only be including alphabets, numbers, '.', '_'.\n";
            return false;
        }
    
    return true;
}

bool UserManager::isValidPassword(const std::string& password) 
{
   
    if (password.length() < 8) {
        std::cout << "Error: Password must be at least 8 characters long.\n";
        return false;
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;

   
    for (char c : password) 
    {
        if (std::isupper(c)) hasUpper = true;
        else if (std::islower(c)) hasLower = true;
        else if (std::isdigit(c)) hasDigit = true;

        
        if (hasUpper && hasLower && hasDigit) break;
    }

    
    if (!hasUpper) 
    {
        std::cout << "Error: Password must contain at least one uppercase letter.\n";
        return false;
    }
    if (!hasLower) 
    {
        std::cout << "Error: Password must contain at least one lowercase letter.\n";
        return false;
    }
    if (!hasDigit) 
    {
        std::cout << "Error: Password must contain at least one digit.\n";
        return false;
    }

    return true; 
}


bool UserManager::signup(std::string username, std::string password) 
{
    if (!isValidUsername(username) || !isValidPassword(password)) 
    {
        // پیام مناسب داخل هر کدومشون هست
        return false;
    }
    std::string lowerName = toLower(username);
    TrieNode* curr = root;
    for (char c : lowerName) 
    {
        if (curr->children.find(c) == curr->children.end()) 
            curr->children[c] = new TrieNode();
        curr = curr->children[c];
    }
    if (curr->isEndOfWord) 
    {
        std::cout << "This Username already exists.\n";
        return false;
    }
    curr->isEndOfWord = true;
    curr->userPtr = new User(username, password); 
    currentUser = curr->userPtr; // already being logged in after signup immediatly
    std::cout << "Signed up successfully.\n";
    return true;
}

bool UserManager::login(std::string username, std::string password) 
{
    if (currentUser) 
    {
        std::cout << "Someone is already logged in.\n";
        return false;
    }
    User* u = findUser(username);
    if (u && u->validatePassword(password)) 
    {
        currentUser = u;
        std::cout << "Logged in as " << username << "\n";
        return true;
    }
    std::cout << "Wrong username or password.\nLogin failed.\n";
    return false;
}

void UserManager::logout() 
{
    if (currentUser) 
    {
        currentUser = nullptr;
        std::cout << "Logged out.\n";
    }
    else 
    {
        std::cout << "You already didn't logged in.\n";
    }
}

User* UserManager::findUser(std::string username) 
{
    std::string lowerName = toLower(username);
    TrieNode* curr = root;
    for (char c : lowerName) 
    {
        if (curr->children.find(c) == curr->children.end()) 
            return nullptr;
        curr = curr->children[c];
    }
    return (curr->isEndOfWord) ? curr->userPtr : nullptr;
}

void UserManager::searchUserPrefix(std::string prefix) 
{
    std::string lowerPrefix = toLower(prefix);
    TrieNode* curr = root;
    for (char c : lowerPrefix) 
    {
        if (curr->children.find(c) == curr->children.end()) 
        {
            std::cout << "No users found.\n";
            return;
        }
        curr = curr->children[c];
    }
    // DFS پیمایش برای پیدا کردن تمام یوزرها زیر این شاخه
    searchRecursive(curr, "");
}

//
void UserManager::findHammingMatches(TrieNode* node, std::string currentWord, const std::string& target, int threshold) {
    if (!node) return;

    // اگر به انتهای یک یوزرنیم رسیدیم، فاصله هامینگ را چک کن
    if (node->isEndOfWord) 
    {
        int dist = calculateHammingDistance(currentWord, target);
        if (dist <= threshold) 
            std::cout << " - " << currentWord << " (Distance: " << dist << ")\n";
    }

    // به صورت بازگشتی برای تمام فرزندان تکرار کن
    for (auto const& pair : node->children) 
        findHammingMatches(pair.second, currentWord + pair.first, target, threshold);
}

int UserManager::calculateHammingDistance(std::string s1, std::string s2) 
{
    int distance = 0;
    int len1 = s1.length();
    int len2 = s2.length();
    int minLen = std::min(len1, len2);

    //  شمارش کاراکترهای متفاوت در طول مشترک
    for (int i = 0; i < minLen; i++)
        if (s1[i] != s2[i])
            distance++;

    //  اضافه کردن تفاوت طول
    distance += std::abs(len1 - len2);

    return distance;
}

// تابع اصلی که توسط کاربر صدا زده می‌شود
void UserManager::smartSearch(std::string term) 
{
    std::cout << "Smart search results for '" << term << "':\n";
    // شروع پیمایش از ریشه (root)
    findHammingMatches(root, "", term, 2); // عدد 2 آستانه خطاست
}

void UserManager::searchRecursive(TrieNode* node, std::string suffix) 
{
    if (node->isEndOfWord) 
        std::cout << "- " << node->userPtr->getUsername() << "\n";
   
    for (auto const& pair : node->children) 
        searchRecursive(pair.second, suffix + pair.first);
}

User* UserManager::getCurrentUser() const 
{ return currentUser; }

