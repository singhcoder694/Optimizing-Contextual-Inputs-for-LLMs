#include <assert.h>
#include <cmath>
#include <sstream>
#include "chrono"
#include "qna_tool.h"

using namespace std;
void chotumerge(vector<pair<int, long double>> &dummy, int begin, int midd, int ending)
{
    vector<pair<int, double>> temp;
    int i = begin;
    int j = midd + 1;
    while (i <= midd && j <= ending)
    {

        if (dummy[i].second >= dummy[j].second)
        {
            temp.push_back(dummy[i]);
            i++;
        }
        else
        {
            temp.push_back(dummy[j]);
            j++;
        }
    }
    while (i <= midd)
    {
        temp.push_back(dummy[i]);
        i++;
    }
    while (j <= ending)
    {
        temp.push_back(dummy[j]);
        j++;
    }
    for (int i = begin; i <= ending; i++)
        dummy[i] = temp[i - begin];
}
void mergesme(vector<pair<int, long double>> &dummy, int begin, int ending)
{
    if (begin < ending)
    {
        int midd = begin + (ending - begin) / 2;
        mergesme(dummy, begin, midd);
        mergesme(dummy, midd + 1, ending);
        chotumerge(dummy, begin, midd, ending);
    }
}
QNA_tool::QNA_tool()
{
    // Implement your function here
    d = new Dict();
    gen = new trie();
    se = new SearchEngine();
    fstream file;
    file.open("unigram_freq.csv", ios::in);
    std::string line, word, temp;
    int denoter = 0;
    vector<string> row;
    while (std::getline(file, line))
    {
        row.clear();
        if (denoter == 0)
        {
            denoter = 1;
            continue;
        }
        else
        {
            stringstream s(line);
            while (std::getline(s, word, ','))
            {
                row.push_back(word);
            }
            long long count;
            count = stoll(row[1]);
            gen->geninsert(row[0], count);
        }
    }
}

QNA_tool::~QNA_tool()
{
    // Implement your function here
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    // Implement your function here
    d->insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    se->insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    return;
}

void QNA_tool::topk(string word, vector<vector<vector<Info>>> &vvv)
{
    long long count = d->get_word_count(word);
    long long gencount = gen->gensearch(word);
    long double score = ((long double)(count + 1)) / ((long double)(gencount + 1));
    // cout << "count of word calculated " << word << ' ' << count << endl;
    // cout << "gencount of word calculated " << word << ' ' << gencount << endl;
    // cout << "score of word calculated " << word << ' ' << score << endl;
    int z = 0;
    Node *ll = se->search(word, z);
    // cout<<"check bookcode"<<ll->book_code<<endl;
    // cout<<"check page"<<ll->page<<endl;
    // cout<<"check paragraph"<<ll->paragraph<<endl;
    while (ll != NULL)
    {
        int cb = ll->book_code;
        int cpg = ll->page;
        int cp = ll->paragraph;
        if (cb >= (int)vvv.size())
        {
            while ((int)vvv.size() != cb + 1)
            {
                vector<vector<Info>> t;
                vvv.push_back(t);
            }
        }
        if (cpg >= (int)vvv[cb].size())
        {
            while ((int)vvv[cb].size() != cpg + 1)
            {
                vector<Info> T;
                vvv[cb].push_back(T);
            }
        }
        if (cp >= (int)vvv[cb][cpg].size())
        {
            while ((int)vvv[cb][cpg].size() != cp + 1)
            {
                Info tmp;
                vvv[cb][cpg].push_back(tmp);
            }
        }
        vvv[cb][cpg][cp].score += score;
        vvv[cb][cpg][cp].book = cb;
        vvv[cb][cpg][cp].page = cpg;
        vvv[cb][cpg][cp].para = cp;
        ll = ll->right;
    }
}

Node *QNA_tool::get_top_k_para(string question, int k)
{
    // Implement your function here
    string word = "";
    vector<vector<vector<Info>>> vvv;
    int sz = question.size();
    for (int i = 0; i < sz; i++)
    {
        char c = question[i];
        if (c == ' ' || c == '.' || c == ',' || c == '-' || c == ':' || c == '!' || c == '"' || c == '(' || c == ')' || c == '?' || c == '_' || c == '[' || c == ']' || c == ';' || c == '@' || (int)c == 39)
        {
            if (word.size() != 0)
            {
                topk(word, vvv);
                word.clear();
            }
        }
        else if (i == sz - 1)
        {
            word.push_back(c);
            topk(word, vvv);
            word.clear();
        }
        else
            word.push_back(c);
    }
    // cout << "query ke word chale gaye" << endl;
    minheap top(k);
    for (int i = 0; i < (int)vvv.size(); i++)
    {
        for (int j = 0; j < (int)vvv[i].size(); j++)
        {
            for (int l = 0; l < (int)vvv[i][j].size(); l++)
            {
                if (vvv[i][j][l].score != 0.0)
                    top.insert(vvv[i][j][l]);
            }
        }
    }
    // cout << "heap me daal diye" << endl;
    Info current = top.remove();
    // int bo=current.book;
    // int pag=current.page;
    // int par=current.para;
    // Node *curr = new Node(current.book, current.page, current.para, 0, 0);
    // // curr->book_code = current.book;
    // // curr->page = current.page;
    // // curr->paragraph = current.para;
    Node *curr = new Node();
    curr->book_code = current.book;
    curr->page = current.page;
    curr->paragraph = current.para;
    curr->right = NULL;
    curr->left = NULL;
    // cout << "pehla" << endl;
    while (!top.isEmpty())
    {
        current = top.remove();
        // cout << "heap se nikla" << endl;
        // cout << "current ka bookcode" << curr->book_code << endl;
        // prev->book_code = current.book;
        // prev->page = current.page;
        // prev->paragraph = current.para;
        // cout << "yaha" << endl;
        Node *prev = new Node();
        prev->book_code = current.book;
        prev->page = current.page;
        prev->paragraph = current.para;
        curr->left = prev;
        // cout << "hooda city centre" << endl;
        prev->right = curr;
        prev->left = NULL;
        curr = curr->left;
        // cout << "iteration done" << endl;
    }
    // cout << "while khatm" << endl;
    return curr;
}
// class mummy
// {
// public:
//     string sentence = "";
//     long long score = 0;
//     int increaser=0;
//     mummy(string ss)
//     {
//         sentence=ss;
//     }
// };
class daddy
{
public:
    string word = "";
    int val = 0;
    daddy(string ss)
    {
        word = ss;
    }
};
double power(double base, int exponent)
{
    if (exponent == 0)
    {
        return 1.0;
    }
    else if (exponent > 0)
    {
        return base * power(base, exponent - 1);
    }
    else
    {
        return 1.0 / power(base, -exponent);
    }
}
int hashv(string xx)
{
    int ff = 0;
    for (int i = 0; i < xx.length(); i++)
    {
        if (xx[i] - 'a' >= 0)
            ff += power(26, i) * (xx[i] - 'a');
        else
            ff += power(26, i) * (xx[i] - '0');
    }
    return ff;
}
void QNA_tool::query(string question, string filename)
{
    // // Implement your function here
    // auto starti = std::chrono::high_resolution_clock::now();

    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();
    for (int i = 0; i < question.size(); i++)
    {
        question[i] = tolower(question[i]);
    }
    // std::cout << "Q: " << question << std::endl;
    // std::cout << "A: "
    //           << "Studying COL106 :)" << std::endl;
    vector<daddy *> words;
    vector<string> chikchik;
    string boom = "";
    for (int i = 0; i < question.size(); i++)
    {
        char ch = question[i];
        int ind = (int)(ch);
        if (ch == '.' || ch == ',' || ch == '-' || ch == ':' || ch == '!' || ch == '"' || ch == '(' || ch == ')' || ch == '?' || ch == '[' || ch == ']' || ch == ';' || ch == '@' || ch == ' ' || ind == 39)
        {
            words.push_back(new daddy(boom));
            chikchik.push_back(boom);
            boom = "";
        }
        else
        {
            boom += ch;
        }
    }
    words.erase(words.begin());
    chikchik.erase(chikchik.begin());
    daddy *neww = new daddy("");
    neww->word = words[0]->word;
    words.erase(words.begin());
    chikchik.erase(chikchik.begin());
    words.push_back(neww);
    chikchik.push_back(neww->word);
    vector<bool> issw(308915776);
    issw[hashv("it")] = true;
    issw[hashv("is")] = true;
    issw[hashv("are")] = true;
    issw[hashv("was")] = true;
    issw[hashv("were")] = true;
    issw[hashv("be")] = true;
    issw[hashv("been")] = true;
    issw[hashv("being")] = true;
    issw[hashv("have")] = true;
    issw[hashv("has")] = true;
    issw[hashv("had")] = true;
    issw[hashv("having")] = true;
    issw[hashv("do")] = true;
    issw[hashv("does")] = true;
    issw[hashv("did")] = true;
    issw[hashv("doing")] = true;
    issw[hashv("a")] = true;
    issw[hashv("an")] = true;
    issw[hashv("the")] = true;
    issw[hashv("and")] = true;
    issw[hashv("but")] = true;
    issw[hashv("if")] = true;
    issw[hashv("or")] = true;
    issw[hashv("as")] = true;
    issw[hashv("of")] = true;
    issw[hashv("at")] = true;
    issw[hashv("by")] = true;
    issw[hashv("for")] = true;
    issw[hashv("with")] = true;
    issw[hashv("into")] = true;
    issw[hashv("to")] = true;
    issw[hashv("from")] = true;
    issw[hashv("in")] = true;
    issw[hashv("out")] = true;
    issw[hashv("on")] = true;
    issw[hashv("off")] = true;
    issw[hashv("here")] = true;
    issw[hashv("there")] = true;
    issw[hashv("so")] = true;
    issw[hashv("can")] = true;
    issw[hashv("will")] = true;
    // issw[hashv("would")] = true;
    // issw[hashv("should")] = true;
    // issw[hashv("could")] = true;
    for (int i = 0; i < words.size(); i++)
    {
        if (words[i]->word.length() < 6 && issw[hashv(words[i]->word)] == true)
        {
            words[i]->val = 1;
        }
        else
        {
            words[i]->val = 50;
        }
    }
    words[words.size() - 2]->val = 500;
    vector<vector<long double>> tfidf(words.size(), vector<long double>(se->paras.size()));
    for (int i = 0; i < words.size(); i++)
    {
        for (int j = 0; j < se->paras.size(); j++)
        {
            tfidf[i][j] = 1.0;
        }
    }
    Node *calc = NULL;
    for (int i = 0; i < words.size(); i++)
    {
        calc = se->search2(chikchik, i, issw);
        int xx = 0;
        while (calc != NULL)
        {
            int bb = calc->book_code;
            int pg = calc->page;
            int pra = calc->paragraph;
            int brrr = calc->offset;
            int india = se->kyah[bb][pg][pra];
            se->paras[india]->offsetter += brrr;
            tfidf[i][india] += 1.0;
            calc = calc->right;
            xx++;
        }
    }
    vector<long double> totalcnt(words.size());
    for (int i = 0; i < words.size(); i++)
    {
        int x = 1;
        for (int j = 0; j < tfidf[i].size(); j++)
        {
            if (tfidf[i][j] > 1.0)
                x++;
        }
        totalcnt[i] = log2l(se->paras.size() / (long double)x);
    }
    vector<vector<long double>> tf(words.size(), vector<long double>(se->paras.size()));
    for (int i = 0; i < words.size(); i++)
    {
        for (int j = 0; j < se->paras.size(); j++)
        {
            tf[i][j] = tfidf[i][j] / se->paras[j]->words;
        }
    }
    for (int i = 0; i < words.size(); i++)
    {
        for (int j = 0; j < se->paras.size(); j++)
        {
            tfidf[i][j] = tf[i][j] * totalcnt[i];
        }
    }
    long double maxi = -1;
    for (int i = 0; i < se->paras.size(); i++)
    {
        long double x = 0;
        for (int j = 0; j < words.size(); j++)
        {
            x += tfidf[j][i] * words[j]->val;
        }
        se->paras[i]->score = x * se->paras[i]->offsetter;
        maxi = max(maxi, se->paras[i]->score);
    }
    vector<pair<int, long double>> tudup;
    for (int i = 0; i < se->paras.size(); i++)
    {
        tudup.push_back(make_pair(i, se->paras[i]->score));
    }
    mergesme(tudup, 0, tudup.size() - 1);
    int x = 0;
    int xyz = tudup.size();
    if (7 < tudup.size())
    {
        xyz = 7;
    }
    long double y = 0.0;
    Node *sir = NULL;
    Node *abhi = NULL;
    int i;
    int wordl = 0;
    for (i = 0; i < xyz; i++)
    {
        x = tudup[i].first;
        y = tudup[i].second;
        wordl += se->paras[x]->words;
        if (wordl > 3500)
            break;
        if (y < 0.2 * maxi)
        {
            break;
        }
        if (sir == NULL)
        {
            sir = new Node(se->paras[x]->book_code, se->paras[x]->page, se->paras[x]->paragraph, 0, 0);
            abhi = sir;
            abhi->left = NULL;
            abhi->right = NULL;
        }
        else
        {
            abhi->right = new Node(se->paras[x]->book_code, se->paras[x]->page, se->paras[x]->paragraph, 0, 0);
            abhi = abhi->right;
            abhi->left = NULL;
            abhi->right = NULL;
        }
    }
    // auto endi = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> durationi = endi - starti;
    // std::cout << "Time taken: " << durationi.count() << " seconds" << std::endl;
    // string question = "What did Gandhi write in his last letter to Mira?";
    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "Context gen time: " << elapsed_seconds.count() << "s\n";
    query_llm(filename, sir, i, "API_KEY", question);
    return;
}
std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph)
{

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;

    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence))
    {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ','))
        {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos)
            {
                token = token.substr(start, end - start + 1);
            }

            // Check if the element is a number or a string
            if (token[0] == '\'')
            {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            }
            else
            {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if (
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph))

        {
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node *root, int k, string API_KEY, string question)
{

    // first write the k paragraphs into different files

    Node *traverse = root;
    int num_paragraph = 0;

    while (num_paragraph < k)
    {
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();

    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}