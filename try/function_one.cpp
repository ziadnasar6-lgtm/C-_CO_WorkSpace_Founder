// #include <iostream>
// #include <string> // 1. لازم نضمن مكتبة النصوص

// using namespace std; // 2. عشان نستخدم string و cout براحتنا

// string W_spaces(string s)
// {
//     size_t first = s.find_first_not_of(" \t\n\r");
//     if (first == string::npos)
//         return "";
//     size_t last = s.find_last_not_of(" \t\n\r");
//     return s.substr(first, (last - first + 1));
// }

// int main()
// {

//     cout << "Start" << endl;
//     cout << "|" << W_spaces("    hallo iam zooooz  ") << "|" << endl;
//     cout << "End" << endl;

//     return 0;
// }

// #include <iostream>
// #include <string>

// using namespace std;
// string withoutspaces(string s)
// {
//     size_t first = s.find_first_not_of(" \t\n\r");
//     if (first == string::npos)
//     {
//         return "";
//     }

//     size_t last = s.find_last_not_of("\t\n\r");
//     return s.substr(first, (last - first + 1));
// }

// int main()
// {

//     const string RESET = "\033[0m";
//     const string GREEN = "\033[32m";

//     cout << GREEN << "=======================" << RESET << endl;
//     cout << withoutspaces("  iam ZIAD ");
//     cout << endl;
//     cout << GREEN << "=======================" << RESET << endl;

//     return 0;
// }

// #include <iostream>
// #include <string>

// using namespace std;

// string N_spaces(string r)
// {
//     size_t first = r.find_first_not_of("\t\r\n");
//     if (first == string::npos)
//     {
//         return "";
//     }
//     size_t last = r.find_last_not_of("\t\r\n");
//     return r.substr(first, (last - first + 1));
// }

// int main()
// {

//     cout << N_spaces("   zoz iam zoz     ");

//     return 0;
// }

// #include <iostream>
// #include<cstdlib>
// #include <windows.h>

// using namespace std ;

// CRITICAL_SECTION consoleCs ;
// void clear ()
// {     EnterCriticalSection (&consoleCs) ;
//     #ifdef _WIN32 ;
//        system ("cls") ;
//     #else
//        system ("clear") ;
//     #endif
//       LeaveCriticalSection  (&consoleCs)     ;

// }

// int main ()
// {

//     return 0 ;
// }

#include <iostream>
#include <windows.h>
#include <cstdlib>

using namespace std;
CRITICAL_SECTION consoleCs;
void clear()
{
    EnterCriticalSection(&consoleCs);
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main()
{

    return 0;
}
