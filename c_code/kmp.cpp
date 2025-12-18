//
// Created by atman on 10/24/25.
//
#include <iostream>
#include <string>
#include <vector>

/**definitions**/
#define LAST_TRACEABLE_FLAG -1

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cin;

/**
 *
 * @param pattern the substr to find in base str
 * @return a vector<int> contains which index to jump to when
 *         we don't match a word in pattern.
 */
vector<int> get_kmp_traceback(const string &pattern) {
    vector<int> traceback(pattern.size()); // fixed length init

    traceback[0] = LAST_TRACEABLE_FLAG;

    // here we inspect one element, and we can know which index should jump to when
    // the next one is not match.
    const int inspect_count = pattern.size() - 1;
    int inspect_index = 0;
    int current_traceback = LAST_TRACEABLE_FLAG;
    while ( inspect_index < inspect_count) {
        if(current_traceback == LAST_TRACEABLE_FLAG) {
            traceback[++inspect_index] = 0;
            current_traceback = 0;  // start a new turn
        }
        else {
            if (pattern[inspect_index] == pattern[current_traceback])
                traceback[++inspect_index] = ++current_traceback;
            else
                current_traceback = traceback[current_traceback];
        }
    }
    return traceback;
}

int KMP(string str, string pattern, vector<int> traceback) {
    int pattern_idx = 0;
    int str_idx = 0;
    while (str_idx < str.size() && pattern_idx < pattern.size()) {
        if (str[str_idx] == pattern[pattern_idx]) {
            str_idx++;
            pattern_idx++;
        }
        else {
            if (traceback[pattern_idx] == LAST_TRACEABLE_FLAG)
                str_idx ++;
            else
                pattern_idx = traceback[pattern_idx];
        }
    }
    if (pattern_idx == pattern.size()) //find
        return str_idx - pattern.size();
    else
        return -1;
}

int main() {
    string base; // this is the string for searching
    string pattern; // for matching
    cin >> pattern;
    vector<int> traceback = get_kmp_traceback(pattern);
    cout << pattern << endl;
    for (auto x : traceback) {
        cout << x << ' ';
    }
    cin >> base;
    cout << endl << KMP(base, pattern, traceback) << endl;

    return 0;
}