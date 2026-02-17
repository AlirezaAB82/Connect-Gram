#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>

class Post;

class User {
private:
    std::string username;
    std::string password;
    std::vector<Post*> myPosts;
    std::vector<User*> following; // لیست مجاورت 
    std::deque<std::string> searchHistory; 
    int followersCount;

public:
    User(std::string uname, std::string pwd);
    ~User() {};
    // ممنوع کردن کپی برای جلوگیری از نشت حافظه
    User(const User&) = delete;
    User& operator=(const User&) = delete;

    std::string getUsername() const;
    bool validatePassword(const std::string& pwd) const;

    // متدهای گراف روابط
    void follow(User* other);
    void unfollow(User* other);
    bool isFollowing(User* other) const;
    std::vector<User*> getFollowing() const;

    
    void incrementFollowers();
    void decrementFollowers();
    int getFollowersCount() const;

    
    void addToHistory(std::string term);
    void showHistory() const;

    
    void addPost(Post* post); 
    const std::vector<Post*>& getPosts() const; 
   
};

