#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>  // Include the utility header to use std::pair
#include <vector>

using namespace std;
#define endl '\n'
#define vint vector<int>
#define vstring vector<string>
#define uset unordered_set
#define umap unordered_map
#define vbool vector<bool>
#define vvint vector<vector<int>>
#define ll long long
#define MOD 1000000007

vstring split(string &str, char del) {
    int n = str.length();

    vstring ans;
    string temp = "";

    for (int i = 0; i < n; i++) {
        if (str[i] != del)
            temp += str[i];
        else {
            ans.push_back(temp);
            temp = "";
        }
    }

    if (temp != "")
        ans.push_back(temp);

    return ans;
}

void minimizeCashFlow(vector<pair<double, int>> balance, umap<int, string> &personmap) {
    int n = balance.size();

    vector<pair<int, pair<int, double>>> finalFlow;

    priority_queue<pair<double, int>> nve;
    priority_queue<pair<double, int>> pve;

    for (int i = 0; i < n; i++) {
        if (balance[i].first < 0)
            nve.push({-balance[i].first, -balance[i].second});
        else if (balance[i].first > 0)
            pve.push({balance[i].first, balance[i].second});
    }

    while (!nve.empty() && !pve.empty()) {
        auto n = nve.top();
        auto p = pve.top();
        nve.pop();
        pve.pop();

        double toExchange = min(p.first, n.first);
        n.first -= toExchange;
        p.first -= toExchange;

        finalFlow.push_back({p.second, {-n.second, toExchange}});

        if (n.first != 0)
            nve.push({n.first, n.second});

        if (p.first != 0)
            pve.push({p.first, p.second});
    }

    for (auto &itr : finalFlow)
        cout << personmap[itr.first] << " will give to " << personmap[itr.second.first] << " amount " << itr.second.second << endl;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    ifstream inputFile("input.txt");

    if (!inputFile.is_open()) {
        cerr << "Error opening the file!" << endl;
        return 1;
    }

    string line;
    getline(inputFile, line);

    vstring personslist = split(line, ' ');
    umap<int, string> personmap;
    for (int i = 1; i < int(personslist.size()); i++)
        personmap[i - 1] = personslist[i];

    int persons = stoi(personslist[0]);
    vector<pair<double, int>> balance(persons);
    for (int i = 0; i < persons; i++) {
        balance[i].first = 0;
        balance[i].second = i;
    }

    while (getline(inputFile, line)) {
        vstring temp = split(line, ' ');

        int pos = 0;

        double totalSpent = 0;
        int payers = stoi(temp[pos]);
        pos++;
        for (int i = 0; i < payers; i++) {
            int person = stoi(temp[pos]);
            double amount = stod(temp[pos + 1]);
            balance[person].first -= amount;
            totalSpent += amount;
            pos += 2;
        }

        if (temp[pos] == "e") {
            pos++;
            int splitters = stoi(temp[pos]);
            pos++;
            double share = totalSpent / splitters;

            for (int i = 0; i < splitters; i++) {
                balance[stoi(temp[pos])].first += share;
                pos++;
            }
        } else {
            pos++;
            int people = stoi(temp[pos]);
            pos++;

            for (int i = 0; i < people; i++) {
                int person = stoi(temp[pos]);
                double amount = stod(temp[pos + 1]);
                balance[person].first += amount;
                pos += 2;
            }
        }
    }

    inputFile.close();

    minimizeCashFlow(balance, personmap);

    return 0;
}