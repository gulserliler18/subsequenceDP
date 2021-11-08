#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <fstream>
#include <set>
using namespace std;

class cell
{
public:
    int data;
    pair<int, int> previous;
    cell()
    {
        previous = make_pair(0, 0);
        data = 0;
    };
};

int fill_matrix(cell **dpv, string zuerst, string zweite, int l1, int l2, int match, int mismatch, int empty, vector<pair<int, int>> &res)
{
    int top = 0;
    for (int i = 0; i < l1; i++)
    {
        for (int j = 0; j < l2; j++)
        {
            int above = dpv[i][j + 1].data + empty;
            int left = dpv[i + 1][j].data + empty;
            int el_chapo;
            if (zuerst[i] == zweite[j])
                el_chapo = dpv[i][j].data + match;
            else
                el_chapo = dpv[i][j].data + mismatch;
            int thus = max(0, max(el_chapo, max(above, left)));
            dpv[i + 1][j + 1].data = thus;
            if (thus == el_chapo)
            {
                dpv[i + 1][j + 1].previous = make_pair(i, j);
            }
            else if (thus == above)
            {
                dpv[i + 1][j + 1].previous = make_pair(i + 1, j);
            }
            else if (thus == left)
            {
                dpv[i + 1][j + 1].previous = make_pair(i, j + 1);
            }

            if (thus > top)
            {
                top = thus;
                res.clear();
                res.emplace_back(make_pair(i, j));
            }
            else if (thus == top)
            {
                res.emplace_back(make_pair(i, j));
            }
        }
    }
    return top;
}

int main(int argc, char *argv[])
{
    string file = argv[1], zuerst, zweite;
    ifstream inn;
    ofstream off;
    off.open("output.txt");
    vector<string> holder;
    inn.open(file);
    while (!inn.eof())
    {
        inn >> file;
        if (!holder.size())
            holder.emplace_back(file);
        else
        {
            int i;
            for (i = 0; i < holder.size(); i++)
            {
                if (file < holder[i])
                {
                    break;
                }
            }
            holder.emplace(holder.begin() + i, file);
        }
    }
    while (holder.size())
    {
        int len;
        zuerst = holder[0];
        vector<pair<int, int>> location;
        for (int i = 1; i < holder.size(); i++)
        {
            zweite = holder[i];
            cell **dpv = new cell *[zuerst.size() + 1];
            for (size_t i = 0; i < zuerst.size() + 1; i++)
            {
                dpv[i] = new cell[zweite.size() + 1];
            }
            len = fill_matrix(dpv, zuerst, zweite, zuerst.size(), zweite.size(), 1, -2, -4, location);
            // Matrix Printer
            /*cout << '\t' << '\t';
            for (int i = 0; i < zweite.size(); i++)
                cout << zweite[i] << '\t';
            cout << endl;
            for (int i = 0; i < zuerst.size() + 1; i++)
            {
                if (i != 0)
                    cout << zuerst[i - 1] << '\t';
                if (i == 0)
                    cout << '\t';
                for (int j = 0; j < zweite.size() + 1; j++)
                    cout << dpv[i][j] << "\t";
                cout << endl;
            }*/
            off << "strings: " << zuerst << " ," << zweite << endl;
            off << "score: " << len << " ,sequence(s): ";
            set<string> seqs;
            for (size_t i = 0; i < location.size(); i++)
            {
                string lcs = "";
                cell traverse = dpv[location[i].first + 1][location[i].second + 1];
                int x = location[i].first + 1, y = location[i].second + 1;
                while (true)
                {
                    lcs = zuerst[x - 1] + lcs;
                    x = traverse.previous.first;
                    y = traverse.previous.second;
                    traverse = dpv[x][y];
                    if (!traverse.data)
                    {
                        break;
                    }
                }
                if (seqs.find(lcs) == seqs.end() || seqs.empty())
                {
                    seqs.insert(lcs);
                    off << lcs << " ";
                }
            }
            off << endl;

            for (size_t i = 0; i < zuerst.size() + 1; i++)
            {
                free(dpv[i]);
            }
            free(dpv);
        }
        holder.erase(holder.begin());
    }
    return 0;
}
