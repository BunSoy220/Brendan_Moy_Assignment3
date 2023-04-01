//Brendan Moy
#include <fstream>
#include <iostream>
#include <string>

// Uncomment when you implemented linear probing & double hashing
#include "linear_probing.h"
#include "double_hashing.h"

#include "quadratic_probing.h"

using namespace std;


// You can add more functions here
void readSequences(const string &seq_filename, vector<string> &result){
    ifstream fin;
    fin.open(seq_filename, ios::in);
    if (fin.fail()) {
        cerr << "Could not open file " << seq_filename << endl;
        exit(1);
    }
    string line;
    while (!fin.eof()) {//checks if file has content
        getline(fin,line);
        //if(line.length() != 0 || line.length() !=1)
        result.push_back(line);
    }
}

template <typename HashTableType>
void findInputs(vector<string> &query, HashTableType& library){
    for(size_t i = 0; i < query.size(); ++i){
         library.ResetProbes();
        //  string s = query.at(i).substr(0,query.at(i).size()-1);
         if(library.Contains(query.at(i))) 
            cout << query.at(i) << " Found " << library.GetProbes()<< endl;
         else 
            cout << query.at(i) << " Not_Found " << library.GetProbes() <<endl;
    }
}
void readFile(const string &filename, vector<string> &result){
    ifstream fin;
    fin.open(filename, ios::in);
    if (fin.fail()) {
        cerr << "Could not open file " << filename << endl;
        exit(1);
    }
    string line;
    while (!fin.eof()) {//checks if file has content
        getline(fin,line);
        if(line != "")result.push_back(line);
    }
}

template <typename HashTableType>
void insertVector(vector<string> &input, HashTableType& target){
    for(size_t i = 0; i < input.size(); ++i){
        target.Insert(input.at(i));
    }
}
// @hash_table: a hash table (can be linear, quadratic, or double)
// @words_filename: a filename of input words to construct the hash table
// @query_filename: a filename of input words to test the hash table
template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table,
			      const string &words_filename,
			      const string &query_filename) {
    hash_table.MakeEmpty();
    vector<string> words = vector<string>();
    vector<string> query = vector<string>();
    readFile(words_filename, words);
    insertVector(words, hash_table);
    cout << "number_of_elements: " << hash_table.Elements()<< endl;
    cout << "size_of_table: " << hash_table.Size() << endl;
    cout << "load_factor: " << hash_table.LoadFactor() << endl;
    cout << "collisions: " << hash_table.Collisions() << endl;
    cout << "avg_collisions: " << float(hash_table.Collisions())/float(hash_table.Elements()) << endl;
    readFile(query_filename, query);
    cout << endl;
    // for(size_t i = 0; i < query.size(); ++i){
    //     cout << query.at(i) << endl;
    // }
    findInputs(query, hash_table);
}

// @argument_count: argc as provided in main
// @argument_list: argv as provided in imain
// Calls the specific testing function for hash table (linear, quadratic, or double).
int testHashingWrapper(int argument_count, char **argument_list) {
    const string words_filename(argument_list[1]);
    const string query_filename(argument_list[2]);
    const string param_flag(argument_list[3]);
    int R = 89;
    if (argument_count == 5) {
	const string rvalue(argument_list[4]);
	R = stoi(rvalue);
    }

    if (param_flag == "linear") {
      // Uncomment below when you have implemented linear probing.
      HashTableLinear<string> linear_probing_table;
      TestFunctionForHashTable(linear_probing_table, words_filename,
      			 query_filename);
    } else if (param_flag == "quadratic") {
        HashTable<string> quadratic_probing_table;
        TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename);
    } else if (param_flag == "double") {
	 cout << "r_value: " << R << endl;
        // Uncomment below when you have implemented double hashing.
	 HashTableDouble<string> double_probing_table;
     double_probing_table.SetR(R);
	 TestFunctionForHashTable(double_probing_table, words_filename,
	 			 query_filename);
    } else {
	cout << "Unknown tree type " << param_flag
	     << " (User should provide linear, quadratic, or double)" << endl;
    }
    return 0;
}

// Sample main for program create_and_test_hash. DO NOT CHANGE IT.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testHashingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char **argv) {
    if (argc != 4 and argc != 5) {
	cout << "Usage: " << argv[0]
	     << " <wordsfilename> <queryfilename> <flag>" << endl;
	cout << "or Usage: " << argv[0]
	     << " <wordsfilename> <queryfilename> <flag> <rvalue>" << endl;
	return 0;
    }

    testHashingWrapper(argc, argv);
    return 0;
}
