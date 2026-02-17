#include "SearchEngine.h"


int SearchEngine::calculateCommandHammingDistance(const std::string& s1, const std::string& s2) 
{
	if (s1.length() != s2.length()) 
		return 999; // اگر طول برابر نبود، فاصله را زیاد برگردان

	int distance = 0;
	for (size_t i = 0; i < s1.length(); i++) 
		if (s1[i] != s2[i]) 
			distance++;
	return distance;
}

void SearchEngine::runEnginge()
{
	std::cout << " << Wellcome to ConnectGram >>" << std::endl;
	std::string commandPrompt;
	std::string username, pass, temp;

	while (true)
	{
		std::cout << "Command: ";
		std::getline(std::cin, commandPrompt);
		temp = commandPrompt;

		if (commandPrompt.empty()) 
			continue;

		std::stringstream command(commandPrompt);
		std::string mainPartOfCommand;
		command >> mainPartOfCommand;
		std::transform(mainPartOfCommand.begin(), mainPartOfCommand.end(), mainPartOfCommand.begin(), [](unsigned char c) {
			return std::tolower(c);
			});

		if (mainPartOfCommand == "close")
			return; // بستن برنامه

		if (mainPartOfCommand == "login") {
			command >> username;
			command >> pass;
			usermanager.login(username, pass); // داخل این تابع چک می شود که کسی لاگین شده باشد یا نه و اگر کسی لاگین کرده بود ارور نمایش می ده
		}
		else if (mainPartOfCommand == "logout") {
			usermanager.logout(); // در این تابع هم این که لاگین کرده باشه یا نه چک میشه اگه لاگین نبوده باشه اررو میده و اگه لاگین بوده باشه لاگ اوت میشه
		}
		else if (mainPartOfCommand == "signup") {
			// وقتی داخل باشی می تونی حساب های دیگه ای هم بسازی
			command >> username;
			command >> pass;
			usermanager.signup(username, pass);
		}
		else if (usermanager.isAnyOneLoggedin() )
		{

			if (mainPartOfCommand == "search_user") {
				std::string preFixOfUsername;
				command >> preFixOfUsername;
				usermanager.searchUserPrefix(preFixOfUsername);
				usermanager.getCurrentUser()->addToHistory(commandPrompt);
			}
			else if (mainPartOfCommand == "follow") {
				command >> username;
				User* other = usermanager.findUser(username);
				if (other) // چک کردن وجود داشتن این یوزر
					usermanager.getCurrentUser()->follow(other);
				else
					std::cout << "There is no such a Username.\n";
				usermanager.getCurrentUser()->addToHistory(commandPrompt);
			}
			else if (mainPartOfCommand == "unfollow") {
				command >> username;
				User* other = usermanager.findUser(username);
				if (other) // چک کردن وجود داشتن این یوزر
					usermanager.getCurrentUser()->unfollow(other);
				else
					std::cout << "There is no such a Username.\n";
				usermanager.getCurrentUser()->addToHistory(commandPrompt);
			}
			else if (mainPartOfCommand == "post") {
				std::string content;
				std::getline(command, content);
				size_t firstChar = content.find_first_not_of(" \t");
				if (firstChar != std::string::npos) {
					content = content.substr(firstChar);
				}
				else {
					// اگر کاربر فقط post نوشته و متنی ننیوشته باشد
					content = "";
				}
				
				if (!content.empty()) {
					postmanager.createPost(usermanager.getCurrentUser(), content);
				}
				else {
					std::cout << "Error: You cannot post an empty content.\n";
				}
				usermanager.getCurrentUser()->addToHistory(commandPrompt);
			}
			else if (mainPartOfCommand == "trend") {
				std::string hashtags;
				command >> hashtags; // 
				postmanager.searchByHashtag(hashtags);
				// چون از trie استفاده کردم الان یه قابلیت auto compelete هم داره به شکلی که اگه کاربر #pro بزنه اول پست هایی که #pro دارند و بعد پست هایی  که هشتگ هایی دارند که pro شروع میشه رو نشون میده
				usermanager.getCurrentUser()->addToHistory(commandPrompt);
			}
			else if (mainPartOfCommand == "like") {
				std::string postID;
				command >> postID;
				try {
					postmanager.likePost(std::stoi(postID));
				}
				catch (std::exception) {
					std::cout << "Error: Wrong format of ID(it must be 8 digit number without any space).\n";
				}
				usermanager.getCurrentUser()->addToHistory(commandPrompt);
			}
			else if (mainPartOfCommand == "show_feed") {
				postmanager.showFeed(usermanager.getCurrentUser());
				usermanager.getCurrentUser()->addToHistory(commandPrompt);
			}
			else if (mainPartOfCommand == "smart_search") {
				// اگر بعد از متن از هشتگ(اولین کاراکتر) استفاده کرده باشه اسمارت سرچ برای هشتگ ها انجام میشه و اگر هشتگ نباشه اسمارت سرچ بین یوزر ها انجام میشه
				std::string usernameOrHashtag;
				command >> usernameOrHashtag;
				if (usernameOrHashtag[0] == '#')
					postmanager.smartSearchHashtags(usernameOrHashtag);
				else
					usermanager.smartSearch(usernameOrHashtag);
				usermanager.getCurrentUser()->addToHistory(commandPrompt);
			}
			else if (mainPartOfCommand == "show_history") {
				usermanager.getCurrentUser()->showHistory();
			}
			else {
				
				bool suggestionFound = false;
				std::cout << "Error: Undefined Command '" << mainPartOfCommand << "'.\n";

				for (const std::string& validCmd : validCommands) 
					if (validCmd.length() == mainPartOfCommand.length()) {
						// شرط ۲: فاصله هامینگ کمتر از ۳ باشد
						int dist = calculateCommandHammingDistance(validCmd, mainPartOfCommand);
						if (dist <= 3) {
							std::cout << "Did you mean \"" << validCmd << "\"?\n";
							suggestionFound = true;
						}
					}
				if (!suggestionFound) 
					commandGuider(); // اگر هیچ پیشنهادی پیدا نشد، راهنما را چاپ کن
			}
		}
		else {
			std::cout << "Pleas login or signup first.\n";
		}
	}
}

void SearchEngine::commandGuider() {
	std::cout << "Error:Undefined Command.\n";
	std::cout << "<< Command Guide >>\nsignup [Username] [Password]\nlogin [Username] [Password]\nlogout\nsearch_user [Username]\nfollow [Username]\nunfollow [Username]\npost [Content]\ntrend [#....](like:#pro#well_done)\nlike [PostID]\nshow_feed\nsmart_search [term](if your content starts with # it would show you hashtage otherwisw would search for the most similar usernames)\n";
}
