#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<cctype>
#include<sstream>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<utility>
using namespace std;
typedef long long ll;

struct avlNode{
    ll timestamp;
    string post;
    avlNode* left;
    avlNode* right;
    int height;

    avlNode(string s, ll t){
        timestamp=t;
        post=s;
        left=nullptr;
        right=nullptr;
        height=0;
    }
};

class avlPost{
    avlNode* root;

    ll number_of_posts=0;

    int height(avlNode* node){
        return node ? node->height:-1;
    }

    void updateHeight(avlNode* node){
        if(node) node->height=1+max(height(node->left),height(node->right));
    }

    avlNode* rightRotate(avlNode* y){
        avlNode* x=y->left;
        avlNode* T2=x->right;
        
        x->right=y;
        y->left=T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    avlNode* leftRotate(avlNode* x){
        avlNode* y=x->right;
        avlNode* T2=y->left;
        
        y->left=x;
        x->right=T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    int getBalance(avlNode* node){
        return node ? height(node->left)-height(node->right):0;
    }


    avlNode* insertHelp(avlNode* node, ll t, string content){
        if (node==nullptr){
            return new avlNode(content, t);
        }
        if (t<node->timestamp){
            node->left=insertHelp(node->left,t,content);
        }
        else if (t>node->timestamp){
            node->right=insertHelp(node->right,t,content);
        }
        else if (content!=node->post){
            node->left=insertHelp(node->left,t,content);
        }
        else return node;

        updateHeight(node);

        int balance=getBalance(node);

        if (balance>1 && t<node->left->timestamp){
            return rightRotate(node);
        }
        else if (balance>1 && t>node->left->timestamp){
            node->left=leftRotate(node->left);
            return rightRotate(node);
        }
        else if (balance<-1 && t>node->right->timestamp){
            return leftRotate(node);
        }
        else if (balance<-1 && t<node->right->timestamp){
            node->right=rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void deleteTree(avlNode* node){
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    void reverseInorder(avlNode* node, vector<string> &v, int &count,int N){
        if (!node || count>=N) return;
        reverseInorder(node->right,v,count,N);
        if (count<N){
            v.emplace_back(node->post);
            count++;
        }
        reverseInorder(node->left,v,count,N);
    }


public:
    avlPost(){
        root=nullptr;
    }
    ~avlPost(){
        deleteTree(root);
    }

    void addPost(string content, ll t){
        root=insertHelp(root,t,content);
        number_of_posts++;
    }

    vector<string> getPosts(int N){
        vector<string> ans={};
        int count=0;
        if (N==-1 || N>number_of_posts){
            reverseInorder(root,ans,count,number_of_posts);
        }
        else reverseInorder(root,ans,count,N);
        return ans;
    }
};

// Graph body starts here

struct userVertex{
    string username;
    unordered_set<userVertex*> friends;
    avlPost posts;

    userVertex(string name){
        username=name;
    }
};

class SocialNet{
    unordered_map<string, userVertex*> users;
    ll global_time=0;

    ll bfs(userVertex* u, userVertex* v){
        if (u==v) return 0;
        ll dist=1;
        queue<userVertex*> q;
        q.push(u);
        unordered_set<userVertex*> visited;
        visited.insert(u);
        while (q.size()){
            ll q_size=q.size();
            while (q_size--){
                userVertex* curr=q.front();
                q.pop();
                for (auto it:curr->friends){
                    if (it==v) return dist;
                    if (visited.find(it)==visited.end()){
                        q.push(it);
                        visited.insert(it);
                    }
                }
            }
            dist++;
        }
        return -1;
    }

public:
    ~SocialNet(){
        for (auto &[name,user]:users){
            delete user;
        }
    }

    void addUser(const string &name){
        string lower_name="";
        for (char c:name) lower_name+=tolower(c);
        if (users.find(lower_name)==users.end()){
            users[lower_name]=new userVertex(lower_name);
        }
        else{
            cout<<"Username already exists, try something else."<<endl;
        }
    }

    void addFriend(const string &name1, const string &name2){
        string u1="";
        string u2="";
        for (char c:name1) u1+=tolower(c);
        for (char c:name2) u2+=tolower(c);

        if (u1==u2) return;

        if (users.count(u1)!=0 && users.count(u2)!=0){
            userVertex* a=users[u1];
            userVertex* b=users[u2];

            if (a->friends.find(b)==a->friends.end()) a->friends.insert(b);
            if (b->friends.find(a)==b->friends.end()) b->friends.insert(a);
        }
    }

    void listFriends(const string &name){
        string lower_name="";
        for (char c:name) lower_name+=tolower(c);
        if (users.count(lower_name)!=0){
            vector<string> ans;
            for (userVertex* user: users[lower_name]->friends){
                ans.emplace_back(user->username);
            }
            sort(ans.begin(),ans.end());
            for (string s:ans){
                cout<<s<<" ";
            }
            cout<<'\n';
        }
    }

    void suggestFriends(const string &name, int N){
        vector<string> ans={};
        if (N<=0) return;

        string u="";
        for (char c:name) u+=tolower(c);

        if (users.count(u)!=0){
            userVertex* user=users[u];
            unordered_map<string,int> mutuals;
            for (auto it:user->friends){
                for (auto it2:it->friends){
                    if (it2==user) continue;
                    if (user->friends.find(it2)!=user->friends.end()) continue;
                    mutuals[it2->username]++;
                }
            }

            vector<pair<string,int>> cand;
            cand.reserve(mutuals.size());
            for (auto &kv:mutuals) cand.emplace_back(kv.first,kv.second);

            sort(cand.begin(), cand.end(), [](const auto &a, const auto &b){
                if (a.second!=b.second) return a.second>b.second;
                return a.first<b.first;
            });

            for (int i=0; i<cand.size() && (int)ans.size()<N; ++i){
                ans.emplace_back(cand[i].first);
            }

            for (string s:ans){
                cout<<s<<" ";
            }
            cout<<'\n';
        }
        return;
    }

    ll degreeOfSeparation(const string &name1, const string &name2){
        string u1="";
        string u2="";
        for (char c:name1) u1+=tolower(c);
        for (char c:name2) u2+=tolower(c);

        if (users.count(u1)!=0 && users.count(u2)!=0){
            userVertex* a=users[u1];
            userVertex* b=users[u2];
            return bfs(a,b);
        }
        return -1;
    }

    void addPost(const string &name, const string &content){
        string lower_name="";
        for (char c:name) lower_name+=tolower(c);
        if (users.count(lower_name)!=0){
            global_time++;
            users[lower_name]->posts.addPost(content,global_time);
        }
    }

    void outputPosts(const string &name, int N){
        string lower_name="";
        for (char c:name) lower_name+=tolower(c);
        if (users.count(lower_name)!=0){
            vector<string> posts=users[lower_name]->posts.getPosts(N);
            for (string p: posts) cout<<p<<'\n';
        }
        cout<<endl;
    }
};

int main(){
    SocialNet social_media;
    string command;
    while (getline(cin,command)){
        if (command.empty()) continue;

        stringstream ss(command);
        string word;
        vector<string> keywords;

        while (ss>>word){
            keywords.push_back(word);
            if (keywords.size()==2){
                string rest;
                getline(ss,rest);
                if (!rest.empty()){
                    if (rest[0]==' ') rest.erase(0,rest.find_first_not_of(' '));
                    keywords.push_back(rest);
                }
                break;
            }
        }

        if (keywords.empty()) continue;
        string function=keywords[0];

        if (function=="ADD_USER"){
            if (keywords.size()>=2) social_media.addUser(keywords[1]);
        }
        else if (function=="ADD_FRIEND"){
            if (keywords.size()>=3) social_media.addFriend(keywords[1],keywords[2]);
        }
        else if (function=="LIST_FRIENDS"){
            if (keywords.size()>=2) social_media.listFriends(keywords[1]);
        }
        else if (function=="SUGGEST_FRIENDS"){
            if (keywords.size()>=3){
                int N=stoi(keywords[2]);
                social_media.suggestFriends(keywords[1],N);
            }
        }
        else if (function=="DEGREES_OF_SEPARATION"){
            if (keywords.size()>=3) cout<<social_media.degreeOfSeparation(keywords[1],keywords[2])<<'\n';
        }
        else if (function=="ADD_POST"){
            if (keywords.size()>=3) social_media.addPost(keywords[1],keywords[2]);
        }
        else if (function=="OUTPUT_POSTS"){
            if (keywords.size()>=3){
                int N=stoi(keywords[2]);
                social_media.outputPosts(keywords[1],N);
            }
        }
        else if (function=="EXIT" || function=="QUIT") break;
        else continue;
    }
}