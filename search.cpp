// Do NOT add any other includes
#include "search.h"

SearchEngine::SearchEngine()
{
    // Implement your function here
    int siz=1000;
    vector<int>v;
    for(int i=0;i<siz;i++)
    {
        v.push_back(-1);
    }
    vector<vector<int>>vu;
    for(int i=0;i<siz;i++)
    {
        vu.push_back(v);
    }
    for(int i=0;i<siz;i++)
    {
        kyah.push_back(vu);
    }
    // vector<vector<vector<int>>> zig(siz, vector<vector<int>>(siz, vector<int>(siz)));
    // int x=0;
    // cout<<x<<endl;
    // for (int i = 0; i < siz; i++)
    // {
    //     for (int j = 0; j < siz; j++)
    //     {
    //         for (int k = 0; k < siz; k++)
    //         {
    //             zig[i][j][k] = -1;
    //             x++;
    //         }
    //     }
    // }
}

SearchEngine::~SearchEngine()
{
    // Implement your function here
}
void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    // Implement your function here
    for (int i = 0; i < sentence.size(); i++)
    {
        sentence[i] = tolower(sentence[i]);
    }
    mumm.push_back(sentence);
    dingdong.push_back(new Node(book_code, page, paragraph, sentence_no, 0));
    if (kyah[book_code][page][paragraph] == -1)
    {
        kyah[book_code][page][paragraph] = paras.size();
        paras.push_back(new col106(book_code, page, paragraph, sentence));
        int chu = 1;
        for (int j = 0; j < sentence.size(); j++)
        {
            if (sentence[j] == ' ' || sentence[j] == '.' || sentence[j] == ',' || sentence[j] == '-' || sentence[j] == ':' || sentence[j] == '!' || sentence[j] == '"' || sentence[j] == '(' || sentence[j] == ')' || sentence[j] == '?' || sentence[j] == '_' || sentence[j] == '[' || sentence[j] == ']' || sentence[j] == ';' || sentence[j] == '@' || (int)sentence[j] == 39)
            {
                chu++;
            }
        }
        paras[paras.size() - 1]->words += chu;
    }
    else
    {
        paras[kyah[book_code][page][paragraph]]->tf += sentence;
    }
    return;
}
double pow(double base, int exponent) {
    if (exponent == 0) {
        return 1.0;
    } else if (exponent > 0) {
        return base * pow(base, exponent - 1);
    } else {
        return 1.0 / pow(base, -exponent);
    }
}
int hashvv(string xx)
{
    int ff = 0;
    for (int i = 0; i < xx.length(); i++)
    {
        if(xx[i]-'a'>=0)
        ff += pow(26, i) * (xx[i] - 'a');
        else
        ff+=pow(26,i)*(xx[i]-'0');
    }
    return ff;
}
Node *SearchEngine::search(string pattern, int &n_matches)
{
    // Implement your function here
    sir = NULL;
    abhi = NULL;
    for (int i = 0; i < pattern.size(); i++)
    {
        pattern[i] = tolower(pattern[i]);
    }
    Node *temp;
    int pompom[127];
    for (int i = 0; i < 127; i++)
    {
        pompom[i] = -1;
    }
    int m = pattern.size();
    // cout << "ppattern is " << pattern << " and size  is " << pattern.size() << endl;
    for (int i = 0; i < m; i++)
    {
        pompom[(int)pattern[i]] = i;
    }
    int n;
    int j;
    string toc;
    Node *cu;
    for (int i = 0; i < mumm.size(); i++)
    {
        // // cout<<" i is "<<i<<endl;
        cu = dingdong[i];
        n = mumm[i].size();
        int s = 0;
        toc = mumm[i];
        while (s <= (n - m))
        {
            j = m - 1;
            while (j >= 0 && pattern[j] == toc[s + j])
            {
                j--;
            }
            if (j < 0)
            {
                // // cout<< " s hai "<<s<<" j hai "<<j<<endl;
                if (s + j + 1 == 0 || toc[s + j] == ' ' || toc[s + j] == '.' || toc[s + j] == ',' || toc[s + j] == '-' || toc[s + j] == ':' || toc[s + j] == '!' || toc[s + j] == '"' || toc[s + j] == '(' || toc[s + j] == ')' || toc[s + j] == '?' || toc[s + j] == '_' || toc[s + j] == '[' || toc[s + j] == ']' || toc[s + j] == ';' || toc[s + j] == '@' || (int)toc[s + j] == 39)
                {
                    int abcd = s + j + 1 + (int)pattern.length();
                    if (abcd == (int)mumm[i].length() || toc[abcd] == ' ' || toc[abcd] == '.' || toc[abcd] == ',' || toc[abcd] == '-' || toc[abcd] == ':' || toc[abcd] == '!' || toc[abcd] == '"' || toc[abcd] == '(' || toc[abcd] == ')' || toc[abcd] == '?' || toc[abcd] == '_' || toc[abcd] == '[' || toc[abcd] == ']' || toc[abcd] == ';' || toc[abcd] == '@' || (int)toc[abcd] == 39)
                    {
                        temp = new Node(cu->book_code, cu->page, cu->paragraph, cu->sentence_no, s);
                        temp->left=NULL;
                        temp->right=NULL;
                        // // cout << "andar wala if" << endl;
                        if (sir == NULL)
                        {
                            sir = temp;
                            abhi = temp;
                        }
                        else
                        {
                            abhi->right = temp;
                            temp->left = abhi;
                            abhi = temp;
                        }
                        n_matches++;
                        // // // cout << n_matches << endl;
                        // // cout << "s: " << s << endl;
                        s += (s + m < n) ? m - pompom[toc[s + m]] : 1;
                        // // cout << "s badme " << s << endl;
                        // // cout << "fuck" << endl;
                    }
                    else
                    {
                        s += max(1, j - pompom[toc[s + j]]);
                    }
                }
                else
                {
                    s += max(1, j - pompom[toc[s + j]]);
                }
            }
            else
            {
                s += max(1, j - pompom[toc[s + j]]);
            }
        }
    }
    return sir;
}
int contf(vector<string> &chikchik, int index, string mmm, int id2, vector<bool> &issw)
{
    int inindex = index;
    bool check = false;
    string dhishkyaoun;
    if (index + 1 < chikchik.size())
    {
        dhishkyaoun = chikchik[++index];
    }
    else
        return 0;
    int ans = 0;
    int j = 0;
    for (int i = id2 + 1; i < mmm.size(); i++)
    {
        if (mmm[i] == ' ' || mmm[i] == '.' || mmm[i] == ',' || mmm[i] == '-' || mmm[i] == ':' || mmm[i] == '!' || mmm[i] == '"' || mmm[i] == '(' || mmm[i] == ')' || mmm[i] == '?' || mmm[i] == '_' || mmm[i] == '[' || mmm[i] == ']' || mmm[i] == ';' || mmm[i] == '@' || (int)mmm[i] == 39)
        {
            if (j == 0)
                continue;
            else
                return ans;
        }
        else
        {
            if (mmm[i] != dhishkyaoun[j])
            {
                return ans;
            }
            else
            {
                if (j == dhishkyaoun.size() - 1)
                {
                    if (dhishkyaoun.length() <= 6 && hashvv(dhishkyaoun) == true)
                    {
                    }
                    else
                        ans += pow(index - inindex, index-inindex);
                    if (index + 1 < chikchik.size())
                    {
                        dhishkyaoun = chikchik[++index];
                    }
                    else
                        return ans;
                    j = 0;
                }
                else
                {
                    j++;
                }
            }
        }
    }
    return ans;
}
Node *SearchEngine::search2(vector<string> &chikchik, int index, vector<bool> &issw)
{
    // Implement your function here
    string pattern = chikchik[index];
    sir = NULL;
    abhi = NULL;
    Node *temp;
    int pompom[127];
    for (int i = 0; i < 127; i++)
    {
        pompom[i] = -1;
    }
    int m = pattern.size();
    // cout << "ppattern is " << pattern << " and size  is " << pattern.size() << endl;
    for (int i = 0; i < m; i++)
    {
        pompom[(int)pattern[i]] = i;
    }
    int n;
    int j;
    string toc;
    Node *cu;
    for (int i = 0; i < mumm.size(); i++)
    {
        // // cout<<" i is "<<i<<endl;
        cu = dingdong[i];
        n = mumm[i].size();
        int s = 0;
        toc = mumm[i];
        while (s <= (n - m))
        {
            j = m - 1;
            while (j >= 0 && pattern[j] == toc[s + j])
            {
                j--;
            }
            if (j < 0)
            {
                // // cout<< " s hai "<<s<<" j hai "<<j<<endl;
                if (s + j + 1 == 0 || toc[s + j] == ' ' || toc[s + j] == '.' || toc[s + j] == ',' || toc[s + j] == '-' || toc[s + j] == ':' || toc[s + j] == '!' || toc[s + j] == '"' || toc[s + j] == '(' || toc[s + j] == ')' || toc[s + j] == '?' || toc[s + j] == '_' || toc[s + j] == '[' || toc[s + j] == ']' || toc[s + j] == ';' || toc[s + j] == '@' || (int)toc[s + j] == 39)
                {
                    int abcd = s + j + 1 + (int)pattern.length();
                    if (abcd == (int)mumm[i].length() || toc[abcd] == ' ' || toc[abcd] == '.' || toc[abcd] == ',' || toc[abcd] == '-' || toc[abcd] == ':' || toc[abcd] == '!' || toc[abcd] == '"' || toc[abcd] == '(' || toc[abcd] == ')' || toc[abcd] == '?' || toc[abcd] == '_' || toc[abcd] == '[' || toc[abcd] == ']' || toc[abcd] == ';' || toc[abcd] == '@' || (int)toc[abcd] == 39)
                    {
                        temp = new Node(cu->book_code, cu->page, cu->paragraph, cu->sentence_no, 0);
                        temp->left = NULL;
                        temp->right = NULL;
                        // // cout << "andar wala if" << endl;
                        if (sir == NULL)
                        {
                            sir = temp;
                            abhi = temp;
                        }
                        else
                        {
                            abhi->right = temp;
                            temp->left = abhi;
                            abhi = temp;
                        }
                        temp->offset = contf(chikchik, index, mumm[i], s + pattern.length(), issw);
                        // if(index==2)
                        // {
                        //     cout<<temp->offset<<" is offset"<<endl;
                        // }
                        // // // cout << n_matches << endl;
                        // // cout << "s: " << s << endl;
                        s += (s + m < n) ? m - pompom[toc[s + m]] : 1;
                        // // cout << "s badme " << s << endl;
                        // // cout << "fuck" << endl;
                    }
                    else
                    {
                        s += max(1, j - pompom[toc[s + j]]);
                    }
                }
                else
                {
                    s += max(1, j - pompom[toc[s + j]]);
                }
            }
            else
            {
                s += max(1, j - pompom[toc[s + j]]);
            }
        }
    }
    return sir;
}
// int main()
// {
//     int x=0;
//     SearchEngine *puke=new SearchEngine();
//     puke->insert_sentence(1,1,1,1,"This is my name");
//     puke->search("nam",x)
// }
