#include "PostManager.h"
#include <sstream>
#include <iostream>
#include <algorithm>

PostManager::PostManager() {
    hashtagRoot = new HashtagNode();
}

PostManager::~PostManager() 
{
    for (Post* p : allPosts) 
        delete p;
    clearTrie(hashtagRoot);
}

void PostManager::clearTrie(HashtagNode* node) 
{
    if (!node) 
        return;
    for (auto const& pair : node->children) 
        clearTrie(pair.second); // pair.first همان کاراکتر و pair.second همان پوینتر به فرزند است
    
    delete node;
}


void PostManager::insertHashtag(std::string tag, Post* post) 
{
    HashtagNode* curr = hashtagRoot;
    for (char c : tag) {
        if (curr->children.find(c) == curr->children.end()) 
            curr->children[c] = new HashtagNode();
        curr = curr->children[c];
    }
    curr->isEndOfWord = true;

    // جلوگیری از اضافه شدن تکراری یک پست به یک هشتگ
    if (std::find(curr->posts.begin(), curr->posts.end(), post) == curr->posts.end()) 
        curr->posts.push_back(post);
}

void PostManager::extractHashtags(std::string content, Post* post) 
{
    size_t pos = content.find('#');
    while (pos != std::string::npos) {
        size_t start = pos + 1;
        size_t end = start;

        // جدا کردن هشتگ تا رسیدن به فاصله، اینتر یا هشتگ بعدی
        while (end < content.length() && !isspace(content[end]) && content[end] != '#') 
            end++;

        if (end > start) {
            std::string tag = content.substr(start, end - start);
            std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
            insertHashtag(tag, post);
        }

        // جستجوی هشتگ بعدی از موقعیت فعلی
        pos = content.find('#', end);
    }
}


void PostManager::searchByHashtag(std::string hashtag)
{
    if (hashtag.empty()) return;
    if (hashtag[0] == '#')
        hashtag = hashtag.substr(1);
    std::transform(hashtag.begin(), hashtag.end(), hashtag.begin(), ::tolower);

    HashtagNode* curr = hashtagRoot;
    for (char c : hashtag)
    {
        if (curr->children.find(c) == curr->children.end()) {
            std::cout << "No hashtags found starting with #" << hashtag << "\n";
            return;
        }
        curr = curr->children[c];
    }

    std::vector<Post*> results;
    collectAllPosts(curr, results);

    if (results.empty()) {
        std::cout << "No posts found for #" << hashtag << ".\n";
    }
    else {
        std::cout << "--- Found " << results.size() << " posts for prefix #" << hashtag << " ---\n";
        for (Post* p : results) {
            std::cout << "ID: " << p->getId() << " | @" << p->getAuthor()->getUsername()
                << ": " << p->getContent() << " (Likes: " << p->getLikes() << ")\n";
        }
    }
}

const std::vector<Post*>& PostManager::getAllPostsOfMedia() const
{ return allPosts;}

void PostManager::collectAllPosts(HashtagNode* node, std::vector<Post*>& results) 
{
    if (!node) return;
    for (Post* p : node->posts) 
        if (std::find(results.begin(), results.end(), p) == results.end()) {
            results.push_back(p);
        }
    
    for (auto const& pair : node->children) 
        collectAllPosts(pair.second, results);
}


void PostManager::createPost(User* author, std::string content) 
{
    Post* newPost = new Post(generateIdFromIndex(allPosts.size()), content, author);
    author->addPost(newPost);
    allPosts.push_back(newPost);
    extractHashtags(content, newPost);
    std::cout << "Post created with ID: " << newPost->getId() << "\n";
}

int PostManager::generateIdFromIndex(int index) 
{return (static_cast<long long>(index) * P % M) + OFFSET;}

int PostManager::getIndexFromId(int id) 
{
    long long normalizedId = id - OFFSET;
    if (normalizedId < 0) 
        return -1;
    long long result = (normalizedId * P_INV) % M;
    return static_cast<int>(result);
}

void PostManager::likePost(int id) 
{
    int idx = getIndexFromId(id);
    if (idx >= 0 && idx < allPosts.size()) {
        allPosts[idx]->addLike();
        std::cout << "Post " << id << " liked!\n";
    }
    else 
        std::cout << "Invalid Post ID.\n";
}



void PostManager::showFeed(const User* curUser) 
{
    std::vector<Post*> allPostsOfFollowings;
    for (auto following : curUser->getFollowing())
        allPostsOfFollowings.insert(allPostsOfFollowings.end(),
            following->getPosts().begin(), following->getPosts().end());

    if (allPostsOfFollowings.empty()) {
        std::cout << "Your feed is empty. Follow more people!\n";
        return;
    }

    buildMuxHeapOfPosts(allPostsOfFollowings);

    int postsToShow = std::min((int)allPostsOfFollowings.size(), 10);
    if (postsToShow != 10)
        std::cout << "<< There are only " << allPostsOfFollowings.size() << " posts to see >>" << std::endl;
    else
        std::cout << "<< Top 10 posts of followings baised on score >> " << std::endl;

    for (int i = 0; i < postsToShow; i++)
    {
        std::cout << "Content: " << allPostsOfFollowings[0]->getContent() << std::endl
            << "Score: " << allPostsOfFollowings[0]->calculateScore() << std::endl
            << "Likes: " << allPostsOfFollowings[0]->getLikes() << std::endl
            << "Author Username: " << allPostsOfFollowings[0]->getAuthor()->getUsername() << std::endl
            << "Post ID: " << allPostsOfFollowings[0]->getId() << std::endl;

        if (allPostsOfFollowings.size() > 1) {
            std::swap(allPostsOfFollowings[0], allPostsOfFollowings.back());
            allPostsOfFollowings.pop_back();
            maxHeapify(allPostsOfFollowings, 0);
        }
        else {
            allPostsOfFollowings.pop_back();
        }
    }
}


void PostManager::buildMuxHeapOfPosts(std::vector<Post*>& heapTree)
{
    int n = static_cast<int>(heapTree.size()); 
    if (n <= 1) 
        return;

    for (int i = n / 2 - 1; i >= 0; i--) 
        maxHeapify(heapTree, i);
}

void PostManager::maxHeapify(std::vector<Post*>& heapTree, int index) 
{
    int l = 2 * index + 1, r = 2 * index + 2, largest;
    int n = static_cast<int>(heapTree.size());

    if (l < n && heapTree[l]->calculateScore() > heapTree[index]->calculateScore())
        largest = l;
    else 
        largest = index;

    if (r < n && heapTree[r]->calculateScore() > heapTree[largest]->calculateScore())
        largest = r;

    if (largest != index)
    {
        std::swap(heapTree[index], heapTree[largest]);
        maxHeapify(heapTree, largest);
    }
}


int PostManager::calculateHammingDistance(std::string s1, std::string s2) 
{
    int distance = 0;
    int len1 = s1.length();
    int len2 = s2.length();
    int minLen = std::min(len1, len2);

    // ۱. شمارش کاراکترهای متفاوت در طول مشترک
    for (int i = 0; i < minLen; i++) 
        if (s1[i] != s2[i]) 
            distance++;
    // ۲. اضافه کردن تفاوت طول
    distance += std::abs(len1 - len2);
    return distance;
}

void PostManager::smartSearchHashtags(std::string term) 
{
    if (term[0] == '#') 
        term = term.substr(1);
    std::transform(term.begin(), term.end(), term.begin(), ::tolower);

    std::vector<std::string> allTags;
    collectAllHashtags(hashtagRoot, "", allTags);

    std::cout << "Smart search for #" << term << ":\n";
    for (const std::string& tag : allTags) 
    {
        int dist = calculateHammingDistance(term, tag);
        if (dist <= 2)  // آستانه شباهت
            std::cout << " - #" << tag << (dist == 0 ? " (Exact)" : " (Suggested)") << "\n";
    }
}

void PostManager::collectAllHashtags(HashtagNode* node, std::string currentTag, std::vector<std::string>& tags) 
{
    if (!node) 
        return;
    if (node->isEndOfWord) 
        tags.push_back(currentTag);

    for (auto const& pair : node->children) 
        collectAllHashtags(pair.second, currentTag + pair.first, tags);
}

