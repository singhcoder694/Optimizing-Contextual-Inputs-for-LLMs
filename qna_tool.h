#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "search.h"

using namespace std;

class QNA_tool {

private:

    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_KEY, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here
    Dict* d;
    SearchEngine* se;
    class Info {
    public:
        long double score=0.0;
        int book=0;
        int page=0;
        int para=0;
    };
    class trieNode{
        public:
        trieNode** children=new trieNode*[54];
        long long count;
        trieNode() {
            for (int i=0; i<54; i++){
                children[i]=NULL;
            }
            count=0;
        }

        ~trieNode(){
            count=0;
            delete[] children;
        }
    };

    class trie{
        public:
        trieNode* root;
        int cnt;       
        int mp[128];
        trie(){
            cnt=26;
            for (int i=0; i<=34; i++) mp[i]=-1;
            for (int i=35; i<=38; i++) {
                mp[i]=cnt;
                cnt++;
            }
            for (int i=39; i<=41; i++) mp[i]=-1;
            for (int i=42; i<=43; i++) {
                mp[i]=cnt;
                cnt++;
            }
            for (int i=44; i<=46; i++) mp[i]=-1;
            for (int i=47; i<=57; i++) {
                mp[i]=cnt;
                cnt++;
            }
            for (int i=58; i<=59; i++) mp[i]=-1;
            for (int i=60; i<=62; i++) {
                mp[i]=cnt;
                cnt++;
            }
            for (int i=63; i<=64; i++) mp[i]=-1;
            for (int i=65; i<=90; i++) mp[i]=i-65;
            mp[91]=-1;
            mp[92]=cnt;
            cnt++;
            mp[93]=-1;
            for (int i=94; i<=96; i++) {
                mp[i]=cnt;
                cnt++;
            }
            for (int i=97; i<=122; i++) mp[i]=i-97;
            for (int i=123; i<=126; i++) {
                mp[i]=cnt;
                cnt++;
            }
            mp[127]=-1;
            root=new trieNode();
        }

        void geninsert(string word, long long ct){
            trieNode* current=root;
            for (char ch : word){
                int i=mp[(int)(ch)];
                if (current->children[i]==NULL) current->children[i]=new trieNode();
                current=current->children[i];
            }
            current->count=ct;
        }

        long long gensearch(string word){
            trieNode* current=root;
            for (char ch : word){
                int i=mp[(int)(ch)];
                if (current->children[i]==NULL) return 0;
                current=current->children[i];
            }
            return current->count;
        }

        void destroy(trieNode* node){
            if (node==NULL) return;
            else {
                for (int i=0; i<54; i++){
                    destroy(node->children[i]);
                }
                delete node;
            }
        }

        ~trie(){
           destroy(root); 
        }
    };
    trie* gen;
    class minheap{
    public:
        vector<Info> v;
        int cap;
        minheap(int k){
            cap=k;
        }
        void heapifyUp(int i){
            if (i==0) return;
            int pi=(i-1)/2;
            if (v[i].score<v[pi].score){
                Info tmp=v[pi];
                v[pi]=v[i];
                v[i]=tmp;
                heapifyUp(pi);
            }
        }
        void heapifyDown(int i){
            int li=2*i+1;
            int ri=2*i+2;
            int sz=v.size();
            int mini=i;
            if (li>=sz) return;
            if (v[li].score<v[i].score) mini=li;
            if (ri<sz && v[ri].score<v[mini].score) mini=ri;
            if (mini!=i){
                Info tmp=v[mini];
                v[mini]=v[i];
                v[i]=tmp;
                heapifyDown(mini);
            }
        }
        Info remove(){
            Info ans=v[0];
            v[0]=v[v.size()-1];
            v.pop_back();
            heapifyDown(0);
            return ans;
        }
        void insert(Info x){
            v.push_back(x);
            heapifyUp(v.size()-1);
            if (v.size()>cap) remove();
        }
        bool isEmpty(){
            return (v.size()==0);
        }
    };
    void topk(string word, vector<vector<vector<Info>>> &vvv);

public:

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
};