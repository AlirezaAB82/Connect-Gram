# ConnectGram: Data Structure Driven Social Network 🚀

**ConnectGram** is a C++ backend engine designed to demonstrate the power of efficient data structures in a social media context. Instead of relying on traditional databases, this project implements custom memory-resident structures to handle thousands of users and posts with minimal latency.

## 🧠 Data Structures & Why They Matter

### 1. The Trie (Prefix Tree)
Used for **User Management** and **Hashtag Tracking**. 
- **Efficiency:** Search time is O(L), where L is the length of the name, regardless of the total number of users.
- **Features:** Supports prefix search (e.g., typing "ali" suggests "alireza", "alipour").



### 2. Max-Heap
Used for the **Smart Feed** algorithm.
- **The Logic:** Every post gets a score: Score = (Likes \times W_1) + (Recency \times W_2).
- **Efficiency:** Using a Heap allows us to extract the Top 10 most relevant posts in O(K \log N) time, ensuring a smooth "infinite scroll" experience.



### 3. Hamming Distance (Fuzzy Search)
The **Smart Search** feature detects typos. If a user searches for `farzid`, the system calculates the Hamming Distance against the Trie and suggests `farzad`.

## ✨ Key Features
- **Authentication:** Secure-style signup/login with password validation.
- **Social Graph:** Follow/Unfollow system with O(1) pointer-based access.
- **Trending:** Prefix-based hashtag search to see what's popular.
- **Memory Safe:** Optimized destructors to ensure zero memory leaks.

## 🛠️ Commands
| Command | Description |
| --- | --- |
| `signup [user] [pass]` | Register a new user with validation |
| `login [user] [pass]` | Authenticate and start a session |
| `post [content]` | Create a post with automated ID and hashtag extraction |
| `show_feed` | View top 10 ranked posts from your followings |
| `smart_search [term]` | Find users/hashtags even with typos |
| `trend [#tag]` | Search all posts containing a specific hashtag prefix |

## 🚀 Getting Started
1. Clone the repo: `git clone https://github.com/AlirezaAB82/ConnectGram.git`
2. Compile: `g++ -o connectgram main.cpp UserManager.cpp PostManager.cpp`
3. Run: `./connectgram`
