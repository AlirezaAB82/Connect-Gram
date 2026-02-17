#include "User.h"

User::User(std::string uname, std::string pwd)
    : username(uname), password(pwd), followersCount(0) {
}

std::string User::getUsername() const 
{ return username; }

bool User::validatePassword(const std::string& pwd) const 
{return password == pwd;}

void User::follow(User* other) 
{
    if (isFollowing(other)) 
        std::cout << "You already followed him before.\n";
    else if (other == this) 
        std::cout << "You can't follow yourself :)\n";
    else {
        following.push_back(other);
        other->incrementFollowers();
        std::cout << other->username << " succesfully followed.\n";
    }
}

void User::unfollow(User* other) 
{
    auto it = std::find(following.begin(), following.end(), other);
    if (it != following.end()) 
    {
        following.erase(it);
        other->decrementFollowers();
        std::cout << other->username << " succesfully unfollowed.\n";
    }
    else 
        std::cout << "You can't Unfollow " << other->username << " cause You didn't followed him before.\n";
}

bool User::isFollowing(User* other) const 
{
    for (User* u : following) 
        if (u == other) return true;
    return false;
}

std::vector<User*> User::getFollowing() const 
{ return following; }

void User::incrementFollowers() 
{ followersCount++; }

void User::decrementFollowers()
{ 
    if (followersCount > 0)
        followersCount--; 
}
int User::getFollowersCount() const 
{ return followersCount; }

void User::addToHistory(std::string term) 
{
    if (searchHistory.size() >= 5) 
        searchHistory.pop_front();
    searchHistory.push_back(term);
}

void User::showHistory() const 
{
    if (searchHistory.empty()) 
    {
        std::cout << "Your search history is empty.\n";
        return;
    }

    std::cout << "--- Your Recent Search History ---\n";
    int count = 1;

    // استفاده از Reverse Iterator برای پیمایش از جدیدترین به قدیمی‌ترین
    // این روش بسیار بهینه است چون هیچ کپی از داده‌ها نمی‌گیرد
    for (auto it = searchHistory.rbegin(); it != searchHistory.rend(); ++it) 
    {
        std::cout << count << ". " << *it << "\n";
        count++;
    }
}

void User::addPost(Post* post) 
{ myPosts.push_back(post);}

const std::vector<Post*>& User::getPosts() const 
{ return myPosts;}
