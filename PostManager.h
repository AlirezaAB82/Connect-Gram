#pragma once
#ifndef POST_MANAGER_H
#define POST_MANAGER_H

#include <vector>
#include <string>
#include <map>
#include "Post.h"
#include "User.h"

// ساختار گره برای درخت Trie هشتگ‌ها
struct HashtagNode {
    std::map<char, HashtagNode*> children;
    std::vector<Post*> posts; // پست‌هایی که دقیقاً این هشتگ را دارند
    bool isEndOfWord;

    HashtagNode() : isEndOfWord(false) {}
};

class PostManager {
private:
    std::vector<Post*> allPosts;
    HashtagNode* hashtagRoot; // ریشه درخت Trie برای هشتگ‌ها

    const long long M = 89999981;
    const long long P = 15485863;
    const long long P_INV = 25373803;
    const int OFFSET = 10000000;

    // متدهای کمکی Trie
    void insertHashtag(std::string tag, Post* post);
    void clearTrie(HashtagNode* node);
    void collectAllPosts(HashtagNode* node, std::vector<Post*>& results);
    void extractHashtags(std::string content, Post* post);

    // متد کمکی اسمارت سرچ
    void collectAllHashtags(HashtagNode* node, std::string currentTag, std::vector<std::string>& tags);
    int calculateHammingDistance(std::string s1, std::string s2);

    // متد های کمکی برای فید
    void buildMuxHeapOfPosts(std::vector<Post*>& heapTree);
    void maxHeapify(std::vector<Post*>& heapTree, int index);
public:
    PostManager();
    ~PostManager();

    void createPost(User* author, std::string content);
    int generateIdFromIndex(int index);
    int getIndexFromId(int id);
    void likePost(int id);

    // متد جستجو با قابلیت Prefix Search
    void searchByHashtag(std::string hashtag);
    
    const std::vector<Post*>& getAllPostsOfMedia() const;
    void showFeed(const User* curUser);
    void smartSearchHashtags(std::string term);
};

#endif