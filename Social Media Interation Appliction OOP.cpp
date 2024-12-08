// MUHAMMAD ZUNNOON WAHEED
//   22I-2418 SE-A
// OOP FINAL PROJECT
  
////////////////////  (In The Name of Allah, Most Gracious And Merciful.)   //////////////////////  


// Header Files
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include<Windows.h>

using namespace std;

// Maximum number of users for simplicity
const int MAX_USERS = 100; 
// Maximum number of friends and friend requests
const int MAX_FRIENDS = 10; 
// Maximum number of posts
const int MAX_POSTS = 100;
// Maximum number of comments
const int MAX_COMMENTS = 100; 
// Maximum number of likes
const int MAX_LIKES = 100; 
// Maximum number of messages
const int MAX_MESSAGES = 100; 

// Forward declaration for classes
class Comment;
class Like;
class Message;
class GroupChat; 

class Post
{
private:
    string postID;
    string content;
    string timestamp;
    string userID;
    // Composition: Post contains instances of Comment class as members
    Comment* comments[MAX_COMMENTS];  
    // Composition: Post contains instances of Like class as members
    Like* likes[MAX_LIKES];  
    int commentCount = 0;
    int likeCount = 0;

public:
    Post(const string& id, const string& content, const string& user);

    string getPostID() const
    {
        return postID; 
    }

    string getContent() const 
    {
        return content; 
    }

    string getUserID() const 
    {
        return userID;
    }

    void displayPost() const;

    void addComment(Comment* comment);

    void addLike(Like* like);

    void displayComments() const;

    void displayLikes() const;
};

class Comment
{
private:
    string commentText;
    string userID;

public:
    Comment(const string& comment, const string& user);

    void displayComment() const;
};

class Like 
{
private:
    string userID;

public:
    Like(const string& user);

    void displayLike() const;
};

class Message 
{
private:
    string senderID;
    string receiverID;
    string content;
    string timestamp;

public:
    Message(const string& sender, const string& receiver, const string& content);

    void displayMessage() const;
};

class GroupChat 
{
private:
    string chatID;
    string participants[MAX_FRIENDS];
    string messages[MAX_MESSAGES];
    int participantCount = 0;
    int messageCount = 0;

public:
    GroupChat(const string& id);

    void addParticipant(const string& participantID);

    void sendMessage(const string& senderID, const string& message);

    string getChatID() const; 
    void displayChat() const;

};

class User
{
private:
    string userID;
    string name;
    string email;
    string password;
    string birthdate;
    string friendRequests[MAX_FRIENDS];
    string friends[MAX_FRIENDS];
    // Aggregation: User class contains instances of Post class as members
    Post* posts[MAX_POSTS];  
    // Aggregation: User class contains instances of Message class as members
    Message* messages[MAX_MESSAGES];  
    // Aggregation: User class contains instances of GroupChat class as members
    GroupChat* groupChats[MAX_FRIENDS];  
    int friendRequestCount = 0;
    int friendCount = 0;
    int postCount = 0;
    int messageCount = 0;
    int groupChatCount = 0;

public:

    User() 
    {
        messageCount = 0;
    }

    ~User() 
    {
        for (int i = 0; i < postCount; ++i) 
        {
            delete posts[i];
        }

        for (int i = 0; i < messageCount; ++i)
        {
            delete messages[i];
        }

        for (int i = 0; i < groupChatCount; ++i) 
        {
            delete groupChats[i];
        }
    }

    void registerUser(const string& id, const string& name,
        const string& email, const string& password,
        const string& birthdate)
    {
        this->userID = id;
        this->name = name;
        this->email = email;
        this->password = password;
        this->birthdate = birthdate;
        saveToFile();
        cout << "Registration successful! Welcome, " << name << " (" << userID << ").\n";
    }

    bool login(const string& id, const string& password) 
    {
        loadFromFile(id);
        if (id == userID && password == this->password)
        {
            cout << "Login successful! Welcome back, " << name << " (" << userID << ").\n";
            return true;
        }

        else 
        {
            cout << "Login failed! Invalid credentials.\n";
            return false;
        }
    }

    void sendFriendRequest(const string& friendID)
    {
        if (friendRequestCount < MAX_FRIENDS) 
        {
            friendRequests[friendRequestCount++] = friendID;
            saveToFile();
            cout << "Friend request sent to user " << friendID << ".\n";
        }

        else 
        {
            cout << "Friend request limit reached.\n";
        }
    }

    void processFriendRequest(const string& friendID, const string& friendName)
    {
        cout << "You have received a friend request from " << friendName << " (" << friendID << ").\n";
        cout << "Do you want to accept the friend request? (Y/N): ";
        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            acceptFriendRequest(friendID, friendName);
        }

        else
        {
           cout << "Friend request declined.\n";
            // Remove the friend request
            for (int i = 0; i < friendRequestCount; ++i)
            {
                if (friendRequests[i] == friendID) 
                {
                    friendRequests[i] = friendRequests[--friendRequestCount];
                    saveToFile();
                    return;
                }
            }
        }
    }

    void acceptFriendRequest(const string& friendID, const string& friendName)
    {
        if (friendCount < MAX_FRIENDS)
        {
            friends[friendCount++] = friendID;
            saveToFile();
            cout << "Friend request from " << friendName << " (" << friendID << ") accepted.\n";
        }

        else
        {
            cout << "Friend limit reached. Cannot accept more friends.\n";
        }

        // Remove the friend request
        for (int i = 0; i < friendRequestCount; ++i)
        {
            if (friendRequests[i] == friendID) 
            {
                friendRequests[i] = friendRequests[--friendRequestCount];
                saveToFile();
                return;
            }
        }
    }

    void createPost(const std::string& content)
    {
        if (postCount < MAX_POSTS)
        {
            // Generate a unique post ID based on the number of posts created
            string postID = userID + "_" + std::to_string(postCount + 1);
            posts[postCount++] = new Post(postID, content, userID);
            cout << "Post created successfully with ID: " << postID << "\n";
        }

        else 
        {
            cout << "Post limit reached.\n";
        }
    }

    bool areFriends(const string& friendID) const
    {
        for (int i = 0; i < friendCount; ++i) 
        {
            if (friends[i] == friendID) 
            {
                return true;
            }
        }
        return false;
    }

    void likePost(const string& postID) 
    {
        for (int i = 0; i < postCount; ++i)
        {
            if (posts[i]->getPostID() == postID) 
            {
                if (userID == posts[i]->getUserID() || areFriends(posts[i]->getUserID()))
                {
                    Like* like = new Like(userID);
                    posts[i]->addLike(like);
                    cout << "You liked the post.\n";
                }

                else 
                {
                  cout << "You can't like this post because you are not friends with the author.\n";
                }
                return;
            }
        }
        cout << "Post not found.\n";
    }

    void commentOnPost(const string& postID, const string& commentText)
    {
        for (int i = 0; i < postCount; ++i) 
        {
            if (posts[i]->getPostID() == postID) 
            
            {
                if (userID == posts[i]->getUserID() || areFriends(posts[i]->getUserID()))
                
                {
                    Comment* comment = new Comment(commentText, userID);
                    posts[i]->addComment(comment);
                    cout << "Comment added to the post.\n";
                }

                else 
                {
                   cout << "You can't comment on this post because you are not friends with the author.\n";
                }
                return;
            }
        }
        cout << "Post not found.\n";
    }

    void sendMessage(const string& receiverID, const string& content) 
    {
        if (messageCount < MAX_MESSAGES) 
        
        {
            messages[messageCount++] = new Message(userID, receiverID, content);
            cout << "Message sent successfully.\n";
        }

        else
        {
            cout << "Message limit reached. Cannot send more messages.\n";
        }
    }

    void createGroupChat(const string& chatID, const string& participantID) 
    {
        if (groupChatCount < MAX_FRIENDS) 
        {
            groupChats[groupChatCount++] = new GroupChat(chatID);
            groupChats[groupChatCount - 1]->addParticipant(participantID);
            cout << "Group chat created successfully.\n";
        }


        else
        
        {
           cout << "Group chat limit reached. Cannot create more group chats.\n";
        }
    }

    void sendGroupMessage(const string& chatID, const string& message)
    {
        for (int i = 0; i < groupChatCount; ++i) 
        {
            if (groupChats[i]->getChatID() == chatID)
            {
                groupChats[i]->sendMessage(userID, message);
                return;
            }
        }
        cout << "Group chat not found.\n";
    }

    void displayGroupChats() const 
    {
        cout << "Group Chats for " << name << " (" << userID << "):\n";
        for (int i = 0; i < groupChatCount; ++i) 
        {
            groupChats[i]->displayChat();
        }
    }

    void displayPosts() const 
    {
        cout << "Posts by " << name << " (" << userID << "):\n";
        for (int i = 0; i < postCount; ++i) 
        {
            posts[i]->displayPost();
        }
    }

    void displayPostComments(const string& postID) const
    {
        for (int i = 0; i < postCount; ++i) 
        {
            if (posts[i]->getPostID() == postID)
            {
                posts[i]->displayComments();
                return;
            }
        }
        cout << "Post not found.\n";
    }

    void displayPostLikes(const string& postID) const
    {
        for (int i = 0; i < postCount; ++i) {
            if (posts[i]->getPostID() == postID) {
                posts[i]->displayLikes();
                return;
            }
        }
        cout << "Post not found.\n";
    }

    void displayMessages() const
    {
        cout << "Messages for " << name << " (" << userID << "):\n";
        for (int i = 0; i < messageCount; ++i)
        {
            messages[i]->displayMessage();
        }
    }

    void saveToFile() 
    {
        ofstream file(userID + ".bin", ios::binary | ios::out);
        file.write((char*)this, sizeof(User));
        file.close();
    }

    void loadFromFile(const string& id) 
    {
        ifstream file(id + ".bin", ios::binary | ios::in);
        if (file.is_open()) {
            file.read((char*)this, sizeof(User));
            file.close();
        }
    }

    string getUserID() const 
    {
        return userID; 
    }

    void printUserInfo() const 
    {
        cout << "User ID: " << userID << "\n";
        cout << "Name: " << name << "\n";
        cout << "Email: " << email << "\n";
        cout << "Birthdate: " << birthdate << "\n";
    }
};

bool isUserRegistered(const string& userID)
{
    ifstream file(userID + ".bin");
    return file.good();
}

GroupChat::GroupChat(const std::string& id)
    : chatID(id) 
{

}

void GroupChat::addParticipant(const string& participantID) 
{
    if (participantCount < MAX_FRIENDS)
    
    {
        participants[participantCount++] = participantID;
    }

    else
    
    {
        cout << "Participant limit reached for this group chat.\n";
    }
}

void GroupChat::sendMessage(const string& senderID, const string& message)
{
    if (messageCount < MAX_MESSAGES) 
    {
        messages[messageCount++] = senderID + ": " + message;
    }

    else
    {
        cout << "Message limit reached for this group chat.\n";
    }
}

string GroupChat::getChatID() const 
{
    return chatID;
}

void GroupChat::displayChat() const 
{
    cout << "Group Chat ID: " << chatID << "\nParticipants:\n";

    for (int i = 0; i < participantCount; ++i) 
    {
        cout << participants[i] << "\n";
    }


    cout << "Messages:\n";
    for (int i = 0; i < messageCount; ++i)
    {
        cout << messages[i] << "\n";
    }
    cout << "\n";
}

Post::Post(const string& id, const string& content, const string& user)
    : postID(id), content(content), userID(user) 
{
    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    timestamp = buffer;
}

void Post::displayPost() const 
{
    cout << "Post ID: " << postID << "\nContent: " << content
        << "\nPosted by: " << userID << "\nTimestamp: " << timestamp << "\n";
}

void Post::addComment(Comment* comment) 
{
    if (commentCount < MAX_COMMENTS) 
    {
        comments[commentCount++] = comment;
    }

    else
    {
        cout << "Comment limit reached for this post.\n";
    }
}

void Post::addLike(Like* like)
{
    if (likeCount < MAX_LIKES) 
    {
        likes[likeCount++] = like;
    }

    else 
    {
        cout << "Like limit reached for this post.\n";
    }
}

void Post::displayComments() const 
{
    cout << "Comments on Post ID: " << postID << "\n";
    for (int i = 0; i < commentCount; ++i) 
    {
        comments[i]->displayComment();
    }
}

void Post::displayLikes() const 
{
    cout << "Likes on Post ID: " << postID << "\n";
    for (int i = 0; i < likeCount; ++i) {
        likes[i]->displayLike();
    }
}

Comment::Comment(const string& comment, const string& user)
    : commentText(comment), userID(user) 
{

}

void Comment::displayComment() const
{
    cout << "Comment: " << commentText << "\nCommented by: " << userID << "\n";
}

Like::Like(const string& user)
    : userID(user)
{

}

void Like::displayLike() const 
{
    cout << userID << " liked this post.\n";
}

Message::Message(const string& sender, const string& receiver, const string& content)
    : senderID(sender), receiverID(receiver), content(content) 

{
    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    timestamp = buffer;
}

void Message::displayMessage() const
{
    cout << "From: " << senderID << "\nTo: " << receiverID
        << "\nContent: " << content << "\nTimestamp: " << timestamp << "\n\n";
}


// Function to validate email format
bool isValidEmail(const string& email)
{
    // Simple check for email format (contains '@' and '.')
    return email.find('@') != string::npos && email.find('.') != string::npos;
}

// Function to validate password complexity
bool isValidPassword(const string& password) 
{
    // Simple check for password complexity (at least 8 characters)
    return password.length() >= 8;
}

// Function to validate birthdate format
bool isValidBirthdate(const string& birthdate) 
{
    // Simple check for birthdate format (YYYY-MM-DD)
    return birthdate.length() == 10 &&
        isdigit(birthdate[0]) && isdigit(birthdate[1]) &&
        isdigit(birthdate[2]) && isdigit(birthdate[3]) &&
        birthdate[4] == '-' &&
        isdigit(birthdate[5]) && isdigit(birthdate[6]) &&
        birthdate[7] == '-' &&
        isdigit(birthdate[8]) && isdigit(birthdate[9]);
}

// Function to validate name format
bool isValidName(const string& name) 
{
    // Simple check for a non-empty name
    return !name.empty();
}

// Function For Setting Colur
void setConsoleColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


int main()
{
    int choice;
    setConsoleColor(6);
    cout << "                    \t\t-----------------------------" << endl;
    cout << "              \t\t\t  -> Social Interaction App <-" << endl;
    cout << "               \t\t\t-----------------------------" << endl;
    cout << endl;
    cout << endl;
    cout << "                  \t\t ------------------\t" << endl;
    cout << "                   \t\t|  1. Start App" << "  |\t" << endl;
    cout << "                   \t\t------------------\t" << endl;
    cout << "                   \t\t|   2. Exit     " << " |\t" << endl;
    cout << "                   \t\t------------------\t" << endl;
    cout << endl;
    cout << endl;
    cout << "               \t\t\t-----------------------------" << endl;
    cout << "               \t\t\t-----------------------------" << endl;
    cin >> choice;
    system("CLS");

    if (choice == 1)
    {
    string userID, name, email, password, birthdate, friendID, postContent, postID, commentText;
    string groupChatID, groupChatParticipant, groupChatMessage;
    // Create a sample user
    User sampleUser;

    sampleUser.registerUser("Admin", "Admin", "sample@example.com", "password123", "1990-01-01");
    while (true)
    {
            cout << "\n1. Register";
            cout << "\n2.Login";
            cout << "\n3.Send Friend Request";
            cout << "\n4.Accept Friend Request";
            cout << "\n5.Create Post";
            cout << "\n6.Display Posts";
            cout << "\n7.Like Post";
            cout << "\n8.Comment on Post";
            cout << "\n9.Send Message";
            cout << "\n10.Display Messages";
            cout << "\n11.Create Group Chat";
            cout << "\n12.Send Group Message";
            cout << "\n13.Display Group Chats";
            cout << "\n14.Exit ";

        cout << "Enter your choice: ";
        cin >> choice;
        system("CLS");

        switch (choice)
        {

            // Register
        case 1: 
            cout << "Enter User ID: ";
            cin >> userID;

            cout << "Enter Name: ";

            // Clear the newline character from previous input
            cin.ignore(); 
            getline(cin, name);
            while (!isValidName(name)) 
            {
                cout << "Invalid name. Please enter a non-empty name: ";
                getline(cin, name);
            }

            cout << "Enter Email: ";
            cin >> email;
            while (!isValidEmail(email)) 
            {
                cout << "Invalid email. Please enter a valid email: ";
                cin >> email;
            }

            cout << "Enter Password: ";
            cin >> password;
            while (!isValidPassword(password))
            {
                cout << "Invalid password. Please enter a password with at least 8 characters: ";
                cin >> password;
            }

            cout << "Enter Birthdate: ";
            cin >> birthdate;
            while (!isValidBirthdate(birthdate))
            {
                cout << "Invalid birthdate. Please enter a valid birthdate in the format YYYY-MM-DD: ";
                cin >> birthdate;
            }

            sampleUser.registerUser(userID, name, email, password, birthdate);
            break;

            // Login
        case 2: 
            cout << "Enter User ID: ";
            cin >> userID;

            cout << "Enter Password: ";
            cin >> password;

            if (sampleUser.login(userID, password))
            {
                // Display user's posts after login
                sampleUser.displayPosts();
            }
            break;

            // Send Friend Request
        case 3:
            cout << "Enter Friend's User ID to send request: ";
            cin >> friendID;

            if (isUserRegistered(friendID))
            {
                sampleUser.sendFriendRequest(friendID);
            }

            else
            {
                cout << "User not found.\n";
            }
            break;

            // Accept Friend Request
        case 4: 
            cout << "Enter Friend's User ID to accept request: ";
            cin >> friendID;

            if (isUserRegistered(friendID))
            {
                sampleUser.processFriendRequest(friendID, name);
            }

            else
            {
                cout << "User not found.\n";
            }
            break;

            // Create Post
        case 5:

            // Clear the newline character from previous input
            cin.ignore(); 
            cout << "Enter your post content: ";
            getline(cin, postContent);
            sampleUser.createPost(postContent);
            break;

            // Display Posts
        case 6: 
            sampleUser.displayPosts();
            break;


            // Like Post
        case 7: 
            cout << "Enter the Post ID to like: ";
            cin >> postID;
            sampleUser.likePost(postID);
            break;

            // Comment on Post
        case 8: 
            cout << "Enter the Post ID to comment on: ";
            cin >> postID;
            cout << "Enter the Post ID to comment on: ";
            cin >> postID;

            // Clear the newline character from previous input
            cin.ignore(); 
            cout << "Enter your comment: ";
            getline(cin, commentText);

            sampleUser.commentOnPost(postID, commentText);
            break;

            // Send Message
        case 9: 
            cout << "Enter the recipient's User ID: ";
            cin >> friendID;

            // Clear the newline character from previous input
            cin.ignore(); 
            cout << "Enter your message: ";
            getline(cin, postContent);
            sampleUser.sendMessage(friendID, postContent);
            break;

            // Display Messages
        case 10: 
            sampleUser.displayMessages();
            break;

            // Create Group Chat
        case 11: 
            cout << "Enter Group Chat ID: ";
            cin >> groupChatID;
            cout << "Enter Participant's User ID: "; cin >> groupChatParticipant;
            sampleUser.createGroupChat(groupChatID, groupChatParticipant);
            break;

            // Send Group Message
        case 12:
            cout << "Enter Group Chat ID: "; cin >> groupChatID;
            cin.ignore(); // Clear the newline character from previous input
            cout << "Enter your message: ";
            getline(cin, groupChatMessage);
            sampleUser.sendGroupMessage(groupChatID, groupChatMessage);
            break;

            // Display Group Chats
        case 13: 
            sampleUser.displayGroupChats();
            break;

            // Exit
        case 14: 
            cout << "Thank You!!" << endl;
            return 0;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
       
    }
    cout << "\t\t------------------\t\t" << endl;
    cout << "\t\t|" << "                |" << endl;
    cout << "\t\t|  THANK YOU !!  |\t\t" << endl;
    cout << "\t\t|         " << "       |\t\t" << endl;
    cout << "\t\t------------------\t\t" << endl;

}

////////////////////////////  ( Alhamdulilah!! )   /////////////////////////////////////  
