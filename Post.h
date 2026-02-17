#pragma once
#include <string>
#include <chrono>
#include "User.h"

class Post {
private:
    int id;
    std::string content;
    User* author;
    int likes;
    std::chrono::system_clock::time_point timestamp;

public:
    Post(int post_id, std::string text, User* creator);
    ~Post() {};

    // ممنوع کردن کپی برای جلوگیری از نشت حافظه
    Post(const Post&) = delete;
    Post& operator=(const Post&) = delete;

    void addLike();
    int getLikes() const;
    int getId() const;
    std::string getContent() const;
    User* getAuthor() const;
    
    long long getTimeElapsed() const;
    double calculateScore() const;
};

