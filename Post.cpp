#include "Post.h"

Post::Post(int post_id, std::string text, User* creator)
    : id(post_id), content(text), author(creator), likes(0) 
{
    timestamp = std::chrono::system_clock::now();
}

void Post::addLike() 
{ likes++; }
int Post::getLikes() const 
{ return likes; }
int Post::getId() const 
{ return id; }
std::string Post::getContent() const 
{ return content; }
User* Post::getAuthor() const 
{ return author; }

long long Post::getTimeElapsed() const 
{
    auto now = std::chrono::system_clock::now();
    long long seconds = std::chrono::duration_cast<std::chrono::seconds>(now - timestamp).count();
    return (seconds == 0) ? 1 : seconds; // جلوگیری از تقسیم بر صفر
}

double Post::calculateScore() const {
    double numerator = (likes * 2.0) + (author->getFollowersCount() * 0.5) + 1000.0;
    return numerator / getTimeElapsed();
}