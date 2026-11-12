#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include <string>
#include <vector>

using namespace std;

struct Condition
{
    string field;
    string operation;
    string value;
};

struct Query
{
    
    string selectedField;
    string tableName;
    vector<Condition> conditions;
    vector<string> logicalOperators;
};

vector<string> tokenize(string query);
// Parse Query
bool parseQuery(string query, Query& result);

#endif