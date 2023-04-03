// YOUR NAME.
// spell_check.cc: A simple spell checker.

#include <fstream>
#include <iostream>
#include <string>
#include "double_hashing.h"
using namespace std;
// splits string where target strings are located
//@split: the target string to split
//@input: the string to be split
//returns a new vector with split strings
vector<string> split_string(string split, string input)
{
    size_t index = input.find(split);
    vector<string> result = vector<string>();
    while (index != string::npos)
    {                                             // the string still contains split
        result.push_back(input.substr(0, index)); // push back substring up to index
        input = input.substr(index + 1, input.length());
        index = input.find(split);
    }
    result.push_back(input);
    return result;
}

//removes all instances of target characters from input string
//@characters: contains target characters, will remove all instances of all individual characters in this string
//@input: string to remove
void charRemover(string characters, string &input)
{
    for (char c : characters)
    {
        size_t index = input.find(c);
        while (index != string::npos)
        {
            input.erase(index, 1);
            index = input.find(c);
        }
    }
}

// removes all instances of target string from input string
// @target: string to remove
// @input: string that will be edited
void stringRemover(string target, string &input)
{
    size_t index = input.find(target);
    while (index != string::npos)
    {
        input.erase(index, 1);
        index = input.find(target);
    }
}

//removes all caps from inputed reference string
void removeCaps(string &input)
{
    for (size_t i = 0; i < input.length(); ++i)
    {
        if (input.at(i) >= 'A' && input.at(i) <= 'Z')
        {
            input.replace(i, 1, 1, tolower(input.at(i)));
        }
    }
}

//reads file and adds lines to a vector, removing all /n and /r characters
//@filename: name of the file in string
//@result: vector to add lines into
void readFile(const string &filename, vector<string> &result)
{
    ifstream fin;
    fin.open(filename, ios::in);
    if (fin.fail())
    {
        cerr << "Could not open file " << filename << endl;
        exit(1);
    }
    string line;
    while (!fin.eof())
    { // checks if file has content
        getline(fin, line);
        charRemover("\n\r", line);//removes /n and /r
        if (line != "")
            result.push_back(line);
    }
}

// Case A correctness function
// adds characters 'a' - 'z' in all postions i.e. "hello" "ahello" -> "helloz"
// checks dictionary if contains edited word, and prints statement
// @string: incorrect string
// @dictionary: hash table containing valid words
void CaseA(string s, HashTableDouble<string> &dictionary)
{
    for (size_t i = 0; i < s.length() + 1; ++i)
    {
        for (int c = 'a'; c < 'z'; ++c)
        {
            string copy = s;
            if (dictionary.Contains(copy.insert(i, 1, c))) // adds 1 letter to copy at postion i, for all characters c
                cout << "** " << s << " -> " << copy << " ** Case A" << endl;
        }
    }
}

// Case B correctness function
// removes characters from all postions, one at a time i.e.: "help" -> "elp" -> "hlp" ...
// checks dictionary if contains edited word, and prints statement
// @string: incorrect string
// @dictionary: hash table containing valid words
void CaseB(string s, HashTableDouble<string> &dictionary)
{
    for (size_t i = 0; i < s.length(); ++i)
    {
        string copy = s;
        if (dictionary.Contains(copy.erase(i, 1)))
            cout << "** " << s << " -> " << copy << " ** Case B" << endl;
    }
}
// Case A correctness function
// adds characters 'a' - 'z' in all postions i.e. "hello" "ahello" -> "helloz"
// checks dictionary if contains edited word, and prints statement
// @string: incorrect string
// @dictionary: hash table containing valid words
void CaseC(string s, HashTableDouble<string> &dictionary)
{
    for (size_t i = 1; i < s.length(); ++i)
    {
        string copy = s;
        std::swap(copy.at(i), copy.at(i-1));
        // char c = copy.at(i);
        // copy.erase(i, 1);
        // copy.insert(i - 1, 1, c);
        if (dictionary.Contains(copy))
            cout << "** " << s << " -> " << copy << " ** Case C" << endl;
    }
}
void Correction(string s, HashTableDouble<string> &dictionary)
{
    CaseA(s, dictionary);
    CaseB(s, dictionary);
    CaseC(s, dictionary);
}
// Creates and fills double hashing hash table with all words from
// dictionary_file
HashTableDouble<string> MakeDictionary(const string &dictionary_file)
{
    HashTableDouble<string> dictionary_hash;
    dictionary_hash.SetR(89);
    vector<string> words = vector<string>();
    readFile(dictionary_file, words);
    for (size_t i = 0; i < words.size(); ++i)
    {
        dictionary_hash.Insert(words.at(i));
    }
    return dictionary_hash;
}

// For each word in the document_file, it checks the 3 cases for a word being
// misspelled and prints out possible corrections
void SpellChecker(HashTableDouble<string> &dictionary, const string &document_file)
{
    vector<string> lines = vector<string>();
    vector<string> words = vector<string>();
    readFile(document_file, lines);
    for (size_t i = 0; i < lines.size(); ++i)
    {
        charRemover(",.:\n\r", lines[i]);
        removeCaps(lines[i]);
        vector<string> &&temp = split_string(" ", lines[i]);
        for (size_t i = 0; i < temp.size(); ++i)
        {
            if (temp.at(i) != "" && temp.at(i) != " ")
                words.push_back(temp.at(i));
        }
    }
    for (size_t i = 0; i < words.size(); ++i)
    {
        if (dictionary.Contains(words.at(i)))
            cout << words.at(i) << " is CORRECT" << endl;
        else
        {
            cout << words.at(i) << " is INCORRECT" << endl;
            Correction(words.at(i), dictionary);
        }
    }
}

// @argument_count: same as argc in main
// @argument_list: save as argv in main.
// Implements
int testSpellingWrapper(int argument_count, char **argument_list)
{
    const string document_filename(argument_list[1]);
    const string dictionary_filename(argument_list[2]);

    // Call functions implementing the assignment requirements.

    HashTableDouble<string> dictionary;
    dictionary = MakeDictionary(dictionary_filename);

    SpellChecker(dictionary, document_filename);

    return 0;
}

// Sample main for program spell_check.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testSpellingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>"
             << endl;
        return 0;
    }

    testSpellingWrapper(argc, argv);

    return 0;
}
