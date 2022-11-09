// YOUR NAME.
#include <fstream>
#include <iostream>
#include <string>
#include <fstream>

// Uncomment when you implemented linear probing & double hashing

#include "linear_probing.h"
#include "double_hashing.h"

#include "quadratic_probing.h"

using namespace std;


// You can add more functions here

// @hash_table: a hash table (can be linear, quadratic, or double)
// @words_filename: a filename of input words to construct the hash table
// @query_filename: a filename of input words to test the hash table


vector<string>& getFileStrings(const string &filename){
    ifstream fin;
    fin.open(filename, ios::in);
    if (fin.fail()) {
        cerr << "Could not open file " << filename << endl;
        exit(1);
    }
    vector<string> result = vector<string>();
    string line;
    while (!fin.eof()) {//checks if file has content
        getline(fin, line);//gets line
        result.push_back(line);
    }
    return result;
}
template <typename HashTableType>
void TestFunctionForHashTable(HashTableType& hash_table, const string &words_filename,const string &query_filename){
    hash_table.MakeEmpty();
    vector<string>query = getFileStrings(query_filename);
    vector<string>list = getFileStrings(words_filename);
    for(int i = 0; i < list.size()-1; ++i) {
        hash_table.Insert(list.at(i));
    }

    cout << "number_of_elements: " << hash_table.getElements() <<endl;
    cout << "size_of_table: " << hash_table.size()<<endl;
    cout << "load_factor: " << float(hash_table.getElements())/hash_table.size()<<endl;
    cout << "collisions: " << hash_table.getCollision()<<endl;
    cout << "avg_collisions: " <<  (float(hash_table.getCollision())/float(hash_table.getElements()))<<endl;

    cout <<endl;
    for(int i = 0; i < query.size()-1; ++i) {
        string res  = (hash_table.Contains(query.at(i)))?" Found ":" Not_Found ";
        cout << query.at(i) << res  << hash_table.getProbes(query.at(i)) <<endl;
    }
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
        LinearHashTable<string> linear_probing_table = LinearHashTable<string>();
        TestFunctionForHashTable<LinearHashTable<string>>(linear_probing_table, words_filename, query_filename);
    } else if (param_flag == "quadratic") {
        HashTable<string> quadratic_probing_table = HashTable<string>();
        TestFunctionForHashTable<HashTable<string>>(quadratic_probing_table, words_filename, query_filename);
    } else if (param_flag == "double") {
	cout << "r_value: " << R << endl;
        // Uncomment below when you have implemented double hashing.
	    // HashTableDouble<string> double_probing_table;
	    // TestFunctionForHashTable(double_probing_table, words_filename,
	// 			 query_filename);
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
