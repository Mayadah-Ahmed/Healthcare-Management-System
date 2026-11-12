
#include "QueryParser.h"
#include <cctype>

using namespace std;

// Check operators
bool isOperator(char c)
{
    return c == '=' || c == '!' ||
        c == '<' || c == '>';
}

// Convert string to lowercase
string toLower(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }

    return str;
}

// Tokenization
vector<string> tokenize(string query)
{
    vector<string> tokens;
    string token;
    bool insideQuotes = false;

    for (int i = 0; i < query.length(); i++)
    {
        char c = query[i];
		// Handle quoted strings 
        if (c == '\'')
        {
            insideQuotes = !insideQuotes;
            continue;
        }
        
        if (!insideQuotes && isOperator(c))
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }

            string op(1, c);
            // Check for >= and <= operators
            if (i + 1 < query.length() &&
                query[i + 1] == '=')
            {
                op += query[i + 1];
                i++;
            }
			
            tokens.push_back(op);
		}
        // if we are outside quotes and encounter a space, we push the current token to the tokens vector and clear it for the next token
        else if (isspace((unsigned char)c) && !insideQuotes)
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else if (c == ';' && !insideQuotes)
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        
        else
        {
            
            token += c;
        }
    }

    // Check if quotes are closed
    if (insideQuotes)
    {
        return {};
    }
    if (!token.empty())
    {
        tokens.push_back(token);
    }

    return tokens;
}

// Parse Query

bool parseQuery(string query, Query& result)
{
    vector<string> tokens = tokenize(query);
    int i = 0;

    result.selectedField.clear();
    result.tableName.clear();
    result.conditions.clear();
    result.logicalOperators.clear();

    if (tokens.empty())
        return false;

    // SELECT
    if (toLower(tokens[i]) != "select")
        return false;

    i++;

    if (i >= tokens.size())
        return false;

    // Selected Field: ALL, *, or Doctor Name
  // Selected Field
    if (toLower(tokens[i]) == "all" || tokens[i] == "*")
    {
        result.selectedField = tokens[i];
        i++;
    }
    else if (i + 1 < tokens.size())
    {
        if (toLower(tokens[i]) == "doctor" &&
            toLower(tokens[i + 1]) == "name")
            result.selectedField = "Doctor Name";

        else if (toLower(tokens[i]) == "doctor" &&
            toLower(tokens[i + 1]) == "id")
            result.selectedField = "Doctor ID";

        else if (toLower(tokens[i]) == "doctor" &&
            toLower(tokens[i + 1]) == "specialty")
            result.selectedField = "Doctor Specialty";

        else if (toLower(tokens[i]) == "appointment" &&
            toLower(tokens[i + 1]) == "id")
            result.selectedField = "Appointment ID";

        else if (toLower(tokens[i]) == "appointment" &&
            toLower(tokens[i + 1]) == "date")
            result.selectedField = "Appointment Date";

        else
            return false;

        i += 2;
    }
    else
    {
        return false;
    }
  
    // FROM
    if (i >= tokens.size() ||
        toLower(tokens[i]) != "from")
        return false;

    i++;

    // Table Name
    if (i >= tokens.size())
        return false;

    if (toLower(tokens[i]) == "doctors")
        result.tableName = "Doctors";
    else if (toLower(tokens[i]) == "appointments")
        result.tableName = "Appointments";
    else
        return false;

    i++;

   // Check selected field
    if (result.tableName == "Appointments" &&
        (result.selectedField == "Doctor Name" ||
            result.selectedField == "Doctor Specialty"))
        return false;

    if (result.tableName == "Doctors" &&
        (result.selectedField == "Appointment ID" ||
            result.selectedField == "Appointment Date"))
        return false;

    // No WHERE
    if (i == tokens.size())
        return true;

    // WHERE
    if (toLower(tokens[i]) != "where")
        return false;

    i++;

    // Conditions
    while (i < tokens.size())
    {
        Condition condition;

        // Field Name
        if (i + 1 >= tokens.size())
            return false;

        if (toLower(tokens[i]) == "doctor" &&
            toLower(tokens[i + 1]) == "id")
        {
            condition.field = "Doctor ID";
        }
        else if (toLower(tokens[i]) == "doctor" &&
            toLower(tokens[i + 1]) == "name")
        {
            condition.field = "Doctor Name";
        }
        else if (toLower(tokens[i]) == "doctor" &&
            toLower(tokens[i + 1]) == "specialty")
        {
            condition.field = "Doctor Specialty";
        }
        else if (toLower(tokens[i]) == "appointment" &&
            toLower(tokens[i + 1]) == "id")
        {
            condition.field = "Appointment ID";
        }
        else if (toLower(tokens[i]) == "appointment" &&
            toLower(tokens[i + 1]) == "date")
        {
            condition.field = "Appointment Date";
        }
        else
        {
            return false;
        }

        i += 2;

        // Check if field belongs to selected table
        if (result.tableName == "Doctors")
        {
            if (condition.field == "Appointment ID" ||
                condition.field == "Appointment Date")
                return false;
        }
        else if (result.tableName == "Appointments")
        {
            if (condition.field == "Doctor Name" ||
                condition.field == "Doctor Specialty")
                return false;
        }

        // Operator
        if (i >= tokens.size())
            return false;

        condition.operation = tokens[i];

        if (condition.operation != "=" &&
            condition.operation != "!=" &&
            condition.operation != ">" &&
            condition.operation != "<" &&
            condition.operation != ">=" &&
            condition.operation != "<=")
            return false;

        i++;

        // Value
        if (i >= tokens.size())
            return false;

        condition.value = tokens[i];
        i++;

        result.conditions.push_back(condition);

       // AND / OR
        if (i < tokens.size())
        {
            string logicalOp = toLower(tokens[i]);

            if (logicalOp == "and" || logicalOp == "or")
            {
                result.logicalOperators.push_back(
                    logicalOp == "and" ? "AND" : "OR"
                );
                i++;
            }
            else
            {
                return false;
            }
        }
    }

    if (result.conditions.empty())
        return false;

    if (result.logicalOperators.size() !=
        result.conditions.size() - 1)
        return false;

    return true;
}
