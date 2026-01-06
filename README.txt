# SocialNet

This project includes the use of Data Structures like AVL Trees, Graphs, Hashmaps, Queue, etc. in C++.
1. AVL Trees for storing posts sorted by the time at which they were posted. (Latest first)
2. Graphs (Adjacency Lists) for managing users

It supports adding users, managing friendships, making posts, suggesting friends, and finding degrees of separation between users.


Commands and their descriptions:
1. ADD_USER <username> : Adds a new user to the network.
2. ADD_FRIEND <user1> <user2> : Creates a bidirectional friendship between two users.
3. LIST_FRIENDS <username> : Lists all friends (sorted alphabetically).
4. SUGGEST_FRIENDS <username> <N> : Suggests up to N friends based on mutual connections.
5. DEGREES_OF_SEPARATION <user1> <user2> : Prints the degree of separation between two users.
6. ADD_POST <username> <content> : Adds a post for a user.
7. OUTPUT_POSTS <username> <N> : Displays up to N most recent posts for a user. Use -1 for all posts.
8. EXIT or QUIT : Exits the program.


Running the program in PowerShell:
1. Open Windows PowerShell.
2. In PowerShell, allow scripts for this session:
   Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
3. Run the command .\run.ps1

If running on terminal:
g++ -std=c++17 -O2 -Wall main.cpp -o main.exe to create the main.exe file and ./main.exe to run the program in terminal.

Thank you!