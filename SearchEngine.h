#include "Post.h"
#include "User.h"
#include "PostManager.h"
#include "UserManager.h"
#include <vector>
#include <string>
#include <sstream>


class SearchEngine {
private:
    PostManager postmanager;
    UserManager usermanager;
    const std::vector<std::string> validCommands =
    {
        "signup", "login", "logout", "search_user",
        "follow", "unfollow", "post", "trend",
        "like", "show_feed", "smart_search",
        "show_history", "close"
    };

    void commandGuider();
    int calculateCommandHammingDistance(const std::string& s1, const std::string& s2);
public:
    SearchEngine() {};
    ~SearchEngine() {};

    void runEnginge();
};

