// Do NOT add any other includes
#include <string>
#include <vector>
#include <iostream>
#include "Node.h"
using namespace std;

class SearchEngine
{
private:
    Node *sir = NULL;
    Node *abhi = NULL;
    vector<string> mumm = {};
    vector<Node *> dingdong = {};
    
    // You can add attributes/helper functions here

public:
    class col106
    {
    public:
        int book_code;
        int page;
        int paragraph;
        string tf = "";
        long long offsetter;
        long double score;
        int words;
        col106(int b_code, int pg, int para, string sen)
        {
            book_code = b_code;
            page = pg;
            paragraph = para;
            tf+=sen;
            offsetter=1;
            score=0;
            words=0;
        }
    };
    size_t siz=1000;
    vector<vector<vector<int>>> kyah;
    vector<col106*>paras;
    Node *search2(vector<string> &chikchik, int index,vector<bool>&issw);

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node *search(string pattern, int &n_matches);

    /* -----------------------------------------*/
};