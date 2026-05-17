#include <iostream>
#include <string>
#include <regex>
#include <windows.h>

using namespace std;

bool isValidEmail(const string &email)
{

    const regex pattern("^[a-zA-Z0-9._%+-]+@(gmail|yahoo)\\.com$");

    return regex_match(email, pattern);
}

int main()
{

    SetConsoleOutputCP(CP_UTF8);
    string userEmail;
    cout << "Enter your email: ";
    cin >> userEmail;

    if (isValidEmail(userEmail))
    {
        cout << "Valid Email! ✅" << endl;
    }
    else
    {
        cout << "Invalid Email! ❌ (Must be @gmail.com or @yahoo.com)" << endl;
    }

    return 0;
}
