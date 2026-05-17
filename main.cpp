#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <windows.h>
#include <thread>
#include <chrono>

using namespace std;

// --- Constants & ANSI Colors ---
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string YELLOW = "\033[1;33m";
const string BLUE = "\033[1;34m";
const string CYAN = "\033[1;36m";
const string MAGENTA = "\033[1;35m";
const string WHITE = "\033[1;37m";

const int MAX_USERS = 100;
const int MAX_PLACES = 60;
const int MAX_COMMENTS = 500;
const int MAX_BOOKINGS = 500;

// --- Structs ---
struct User
{
    string fullName;
    string email;
    string password;
    string recoveryCode;
    int points;
    bool isAdmin;
};

struct StudyPlace
{
    int id;
    string name;
    string governorate;
    string district;
    string address;
    string mapsLink;
    string phone;
    string price;
    string rating;
    string description;
    string facilities;
    string tags;
    double matchScore;
};

struct Favorite
{
    string email;
    string placeName;
    string govName;
};

struct Comment
{
    string senderName;
    string placeName;
    string text;
};

struct Booking
{
    string email;
    string placeName;
    string govName;
    int cost;
};

// --- Global Variables ---
User currentUser;
bool isLoggedIn = false;

// Global Critical Section for Console Synchronization
CRITICAL_SECTION consoleCS;

// --- Helpers ---
string trim(string s)
{
    size_t first = s.find_first_not_of(" \t\n\r");
    if (first == string::npos)
        return "";
    size_t last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, (last - first + 1));
}

string split(string &line, char delim)
{
    size_t pos = line.find(delim);
    if (pos == string::npos)
    {
        string res = line;
        line = "";
        return trim(res);
    }
    string res = line.substr(0, pos);
    line = line.substr(pos + 1);
    return trim(res);
}

void clear()
{
    EnterCriticalSection(&consoleCS);
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    LeaveCriticalSection(&consoleCS);
}

void safeGotoxy(short x, short y)
{
    EnterCriticalSection(&consoleCS);
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    LeaveCriticalSection(&consoleCS);
}

void gotoxy(short x, short y)
{
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// --- PERSISTENT 3D HEADER ---
DWORD WINAPI animateSloganLoop(LPVOID lpParam)
{
    int startX = 85;
    int startY = 2;
    string lines[4] = {
        "   Co Work Space Founder       ",
        " - Where ideas meet,           ",
        " - Founders connect,           ",
        " - Great businesses begin.     "};

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    while (true)
    {
        // Draw Decorated Box
        EnterCriticalSection(&consoleCS);
        GetConsoleScreenBufferInfo(hConsole, &csbi); // SAVE

        SetConsoleTextAttribute(hConsole, 11); // CYAN
        gotoxy(startX, startY);
        cout << " +===============================+ ";
        gotoxy(startX, startY + 1);
        cout << " || [ FOUNDER INSIGHT ]        || ";
        gotoxy(startX, startY + 2);
        cout << " +-------------------------------+ ";
        gotoxy(startX, startY + 3);
        cout << " |                               | ";
        gotoxy(startX, startY + 4);
        cout << " |                               | ";
        gotoxy(startX, startY + 5);
        cout << " |                               | ";
        gotoxy(startX, startY + 6);
        cout << " |                               | ";
        gotoxy(startX, startY + 7);
        cout << " +===============================+ ";

        SetConsoleTextAttribute(hConsole, csbi.wAttributes);       // RESTORE COLOR
        SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition); // RESTORE CURSOR
        LeaveCriticalSection(&consoleCS);

        // Typewriter Effect
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < (int)lines[i].length(); j++)
            {
                EnterCriticalSection(&consoleCS);
                GetConsoleScreenBufferInfo(hConsole, &csbi); // SAVE

                SetConsoleTextAttribute(hConsole, 14 | FOREGROUND_INTENSITY); // YELLOW BOLD
                gotoxy(startX + 2 + j, startY + 3 + i);
                cout << lines[i][j];
                cout.flush();

                SetConsoleTextAttribute(hConsole, csbi.wAttributes);       // RESTORE
                SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition); // RESTORE
                LeaveCriticalSection(&consoleCS);
                Sleep(15);
            }
        }

        Sleep(2000); // Wait for user to read

        // Erase effect (backwards)
        for (int i = 3; i >= 0; i--)
        {
            for (int j = (int)lines[i].length() - 1; j >= 0; j--)
            {
                EnterCriticalSection(&consoleCS);
                GetConsoleScreenBufferInfo(hConsole, &csbi); // SAVE

                gotoxy(startX + 2 + j, startY + 3 + i);
                cout << " ";
                cout.flush();

                SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition); // RESTORE
                LeaveCriticalSection(&consoleCS);
                Sleep(5);
            }
        }
        Sleep(500); // Brief pause before repeat
    }
    return 0;
}

void showHeader()
{
    EnterCriticalSection(&consoleCS);
    cout << MAGENTA << BOLD;
    cout << "     ____   ___     __        _____  ____  _  __    ____  ____   _    ____ _____ " << endl;
    cout << "    / ___| / _ \\    \\ \\      / / _ \\|  _ \\| |/ /   / ___||  _ \\ / \\  / ___| ____|" << endl;
    cout << "   | |    | | | |  __ \\ \\ /\\ / / | | | |_) | ' /    \\___ \\| |_) / _ \\| |   |  _|  " << endl;
    cout << "   | |___ | |_| | |__| \\ V  V /| |_| |  _ <| . \\     ___) |  __/ ___ \\ |___| |___ " << endl;
    cout << "    \\____| \\___/        \\_/\\_/  \\___/|_| \\_\\_|\\_\\   |____/|_| /_/   \\_\\____|_____|" << endl;
    cout << YELLOW << BOLD;
    cout << "                 _____ ___  _   _ _   _ ____  _____ ____  " << endl;
    cout << "                |  ___/ _ \\| | | | \\ | |  _ \\| ____|  _ \\ " << endl;
    cout << "                | |_ | | | | | | |  \\| | | | |  _| | |_) |" << endl;
    cout << "                |  _|| |_| | |_| | |\\  | |_| | |___|  _ < " << endl;
    cout << "                |_|   \\___/ \\___/|_| \\_|____/|_____|_| \\_\\" << endl
         << endl
         << endl;
    cout << RESET;
    cout << CYAN << "       +=====================================================================+" << RESET << endl;
    cout << CYAN << "       |                                                                     |" << RESET << endl;
    cout << CYAN << "       +=====================================================================+" << RESET << endl
         << endl;
    LeaveCriticalSection(&consoleCS);
}

void drawButton(string text, string color = CYAN)
{
    EnterCriticalSection(&consoleCS);
    int padding = 40 - (text.length() / 2);
    cout << string(padding - 5, ' ') << color << "+===========================+" << RESET << endl;
    cout << string(padding - 5, ' ') << color << "|| " << RESET << left << setw(23) << text << color << " ||" << RESET << endl;
    cout << string(padding - 5, ' ') << color << "+===========================+" << RESET << endl;
    LeaveCriticalSection(&consoleCS);
}

void drawDoubleBoxMenuItem(int id, string name, string color = CYAN)
{
    EnterCriticalSection(&consoleCS);
    string text = "[" + to_string(id) + "] " + name;
    int outerWidth = 40;
    int innerWidth = outerWidth - 6;
    int padding = 40 - (outerWidth / 2);

    cout << string(padding, ' ') << color << "+";
    for (int i = 0; i < outerWidth - 2; i++)
        cout << "=";
    cout << "+" << RESET << endl;

    cout << string(padding, ' ') << color << "|  +";
    for (int i = 0; i < innerWidth - 2; i++)
        cout << "-";
    cout << "+  |" << RESET << endl;

    cout << string(padding, ' ') << color << "|  | " << RESET << left << setw(innerWidth - 4) << text << color << " |  |" << RESET << endl;

    cout << string(padding, ' ') << color << "|  +";
    for (int i = 0; i < innerWidth - 2; i++)
        cout << "-";
    cout << "+  |" << RESET << endl;

    cout << string(padding, ' ') << color << "+";
    for (int i = 0; i < outerWidth - 2; i++)
        cout << "=";
    cout << "+" << RESET << endl;
    LeaveCriticalSection(&consoleCS);
}

void drawBottomPadding()
{
    EnterCriticalSection(&consoleCS);
    cout << endl
         << endl
         << endl;
    LeaveCriticalSection(&consoleCS);
}

void drawSimpleLine(int width = 50)
{
    EnterCriticalSection(&consoleCS);
    cout << "  +" << string(width, '-') << "+" << endl;
    LeaveCriticalSection(&consoleCS);
}

void drawThickLine(int width = 50)
{
    // Using simple = and || as double lines can be messy in some consoles
    EnterCriticalSection(&consoleCS);
    cout << "  " << string(width + 2, '=') << endl;
    LeaveCriticalSection(&consoleCS);
}

// --- Data Logic ---
bool loadUser(string name, string email)
{
    ifstream file("users.txt");
    string line;
    while (getline(file, line))
    {
        User u;
        u.fullName = split(line, '|');
        u.email = split(line, '|');
        u.password = split(line, '|');
        u.recoveryCode = split(line, '|');
        string pts = split(line, '|');
        string adm = split(line, '|');
        u.points = atoi(pts.c_str());
        u.isAdmin = (adm == "1");
        if (u.fullName == name && u.email == email)
        {
            currentUser = u;
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void registerUser(User u)
{
    ofstream file("users.txt", ios::app);
    file << u.fullName << "|" << u.email << "|" << u.password << "|" << u.recoveryCode << "|" << u.points << "|" << (u.isAdmin ? 1 : 0) << endl;
    file.close();
}

void updatePoints(string email, int newPoints)
{
    User users[MAX_USERS];
    int count = 0;
    ifstream infile("users.txt");
    string line;
    while (count < MAX_USERS && getline(infile, line))
    {
        users[count].fullName = split(line, '|');
        users[count].email = split(line, '|');
        users[count].password = split(line, '|');
        users[count].recoveryCode = split(line, '|');
        string pts = split(line, '|');
        string adm = split(line, '|');
        users[count].points = atoi(pts.c_str());
        users[count].isAdmin = (adm == "1");
        if (users[count].email == email)
            users[count].points = newPoints;
        count++;
    }
    infile.close();
    ofstream outfile("users.txt");
    for (int i = 0; i < count; i++)
    {
        outfile << users[i].fullName << "|" << users[i].email << "|" << users[i].password << "|" << users[i].recoveryCode << "|" << users[i].points << "|" << (users[i].isAdmin ? 1 : 0) << endl;
    }
    outfile.close();
}

void addComment(string sender, string place, string text)
{
    ofstream file("comments.txt", ios::app);
    file << sender << "|" << place << "|" << text << endl;
    file.close();
}

void addBooking(string email, string place, string gov, int points)
{
    ofstream file("bookings.txt", ios::app);
    file << email << "|" << place << "|" << gov << "|" << points << endl;
    file.close();
}

void loadGovernorate(string govName, StudyPlace places[], int &count)
{
    count = 0;
    string filename = "data/" + govName + ".txt";
    ifstream file(filename.c_str());
    if (!file.is_open())
        return;
    while (count < MAX_PLACES && !file.eof())
    {
        if (!(file >> places[count].id))
            break;
        file.ignore();
        getline(file, places[count].name);
        getline(file, places[count].governorate);
        getline(file, places[count].district);
        getline(file, places[count].address);
        getline(file, places[count].mapsLink);
        getline(file, places[count].phone);
        getline(file, places[count].price);
        getline(file, places[count].rating);
        getline(file, places[count].description);
        getline(file, places[count].facilities);
        getline(file, places[count].tags);
        places[count].matchScore = 0;
        count++;
    }
    file.close();
}

// --- UI Components ---
void displayCardShort(StudyPlace p, bool thick = false)
{
    EnterCriticalSection(&consoleCS);
    cout << endl;
    if (thick)
        drawThickLine(58);
    else
        drawSimpleLine(58);
    cout << "  || ID: " << left << setw(4) << p.id << " | Name: " << CYAN << left << setw(35) << (p.name.length() > 30 ? p.name.substr(0, 30) : p.name) << RESET << " ||" << endl;
    cout << "  || Gov: " << left << setw(10) << p.governorate << " | Price: " << GREEN << left << setw(27) << (p.price + " EGP") << RESET << " ||" << endl;
    if (thick)
        drawThickLine(58);
    else
        drawSimpleLine(58);
    LeaveCriticalSection(&consoleCS);
}

void displayFullDetail(StudyPlace p)
{
    clear();
    showHeader();
    EnterCriticalSection(&consoleCS);
    cout << BOLD << GREEN << "\n                --- VIEWING WORKSPACE DETAILS ---\n"
         << RESET;
    drawThickLine(63);
    cout << "  || NAME:      " << WHITE << BOLD << left << setw(49) << (p.name.length() > 48 ? p.name.substr(0, 48) : p.name) << RESET << " ||" << endl;
    drawThickLine(63);
    cout << BOLD << CYAN << "  | Location:  " << RESET << left << setw(50) << (p.district + ", " + p.governorate) << " |" << endl;
    cout << BOLD << CYAN << "  | Address:   " << RESET << left << setw(50) << (p.address.length() > 49 ? p.address.substr(0, 49) : p.address) << " |" << endl;
    cout << BOLD << CYAN << "  | Phone:     " << RESET << GREEN << left << setw(50) << p.phone << RESET << " |" << endl;
    cout << BOLD << CYAN << "  | Pricing:   " << RESET << left << setw(50) << (p.price + " EGP") << RESET << " |" << endl;
    cout << BOLD << CYAN << "  | Rating:    " << RESET << MAGENTA << left << setw(50) << p.rating << RESET << " |" << endl;
    drawSimpleLine(63);
    cout << "  | Description: " << left << setw(48) << (p.description.length() > 48 ? p.description.substr(0, 48) : p.description) << " |" << endl;
    if (p.description.length() > 48)
        cout << "  |              " << left << setw(48) << p.description.substr(48, 48) << " |" << endl;
    cout << BOLD << GREEN << "  | Facilities:  " << RESET << left << setw(49) << (p.facilities.length() > 48 ? p.facilities.substr(0, 48) : p.facilities) << " |" << endl;
    cout << BOLD << GREEN << "  | Tags: " << RESET << BLUE << left << setw(54) << p.tags << RESET << " |" << endl;
    drawThickLine(63);

    cout << BOLD << MAGENTA << "\n  OPTIONS:\n"
         << RESET;
    cout << BOLD << GREEN << "  [1] Book Now (50 pts)   [2] Read Reviews   [3] Write Review\n";
    cout << BOLD << GREEN << "  [4] Add to Favorites    [5] Back\n  >> " << RESET;
    LeaveCriticalSection(&consoleCS);
    int bc;
    cin >> bc;
    if (bc == 1)
    {
        if (currentUser.points >= 50)
        {
            currentUser.points -= 50;
            updatePoints(currentUser.email, currentUser.points);
            addBooking(currentUser.email, p.name, p.governorate, 50);
            cout << GREEN << "  SUCCESS! REMAING: " << currentUser.points << RESET << endl;
        }
        else
            cout << RED << "  INSUFFICIENT CREDIT!" << RESET << endl;
        system("pause");
    }
    else if (bc == 2)
    {
        clear();
        showHeader();
        EnterCriticalSection(&consoleCS);
        cout << YELLOW << "\n  PEOPLE ARE SAYING ABOUT " << p.name << ":\n"
             << RESET;
        LeaveCriticalSection(&consoleCS);
        ifstream f("comments.txt");
        string l;
        bool matched = false;
        while (getline(f, l))
        {
            string s = split(l, '|');
            string pl = split(l, '|');
            string t = split(l, '|');
            if (pl == p.name)
            {
                EnterCriticalSection(&consoleCS);
                cout << "  - " << BOLD << s << RESET << ": " << t << endl;
                LeaveCriticalSection(&consoleCS);
                matched = true;
            }
        }
        if (!matched)
        {
            EnterCriticalSection(&consoleCS);
            cout << BLUE << "======================================" << endl;
            cout << "  No reviews yet. Be the first!\n";
            cout << BLUE << "======================================" << endl;
            LeaveCriticalSection(&consoleCS);
        }

        f.close();
        system("pause");
    }
    else if (bc == 3)
    {
        string t;
        EnterCriticalSection(&consoleCS);
        cout << BOLD << GREEN << "  Write your feedback (1 line): ";
        LeaveCriticalSection(&consoleCS);
        cin.ignore();
        getline(cin, t);
        addComment(currentUser.fullName, p.name, t);
        cout << GREEN << "  Review saved!\n"
             << RESET;
        system("pause");
    }
    else if (bc == 4)
    {
        ofstream f("favorites.txt", ios::app);
        f << currentUser.email << "|" << p.name << "|" << p.governorate << endl;
        f.close();
        cout << GREEN << "  Added to favorites!\n"
             << RESET;
        system("pause");
    }
}

// --- Menu Functions ---
void searchPlaces()
{
    clear();
    showHeader();
    cout << endl
         << BOLD << YELLOW << "  --- SELECT YOUR LOCATION ---" << RESET << endl
         << endl;

    string govs[] = {"Cairo", "Alexandria", "Mansoura", "Qalyubia", "Monufia", "Hurghada", "Upper_Egypt"};
    for (int i = 0; i < 7; i++)
    {
        drawDoubleBoxMenuItem(i + 1, govs[i], (i % 2 == 0 ? CYAN : GREEN));
    }

    EnterCriticalSection(&consoleCS);
    cout << BOLD << GREEN << "\n  >> " << RESET;
    cout << "\n\n\n\033[3A"; // 3-line visual buffer below prompt
    LeaveCriticalSection(&consoleCS);
    int loc;
    cin >> loc;
    drawBottomPadding();
    if (loc < 1 || loc > 7)
        return;

    StudyPlace places[MAX_PLACES];
    int count;
    loadGovernorate(govs[loc - 1], places, count);

    EnterCriticalSection(&consoleCS);
    cout << GREEN << "\n  --- MATCHING ENGINE (PREMIUM SELECTION) ---\n"
         << RESET;
    cout << endl
         << endl;
    LeaveCriticalSection(&consoleCS);
    string q[6];
    EnterCriticalSection(&consoleCS);
    cout << BOLD << CYAN << "  1. FINANCIAL STRATEGY: Do you prefer an economical approach or a premium experience?\n     (budget_friendly / premium): " << RESET;
    cout << "\n\n\n\033[3A";
    LeaveCriticalSection(&consoleCS);
    cin >> q[0];
    drawBottomPadding();
    EnterCriticalSection(&consoleCS);
    cout << BOLD << CYAN << "  2. SONIC ENVIRONMENT: Are you seeking absolute silence or the hum of a vibrant cafe?\n     (quiet / cafe): " << RESET;
    cout << "\n\n\n\033[3A";
    LeaveCriticalSection(&consoleCS);
    cin >> q[1];
    drawBottomPadding();
    EnterCriticalSection(&consoleCS);
    cout << BOLD << CYAN << "  3. TECHNICAL REQUIREMENT: Is high-speed internet or professional meeting room your priority?\n     (internet / meetings): " << RESET;
    cout << "\n\n\n\033[3A";
    LeaveCriticalSection(&consoleCS);
    cin >> q[2];
    drawBottomPadding();
    EnterCriticalSection(&consoleCS);
    cout << BOLD << CYAN << "  4. TACTICAL SPACE: Do you require private office solitude or shared workspace energy?\n     (private_office / shared_space): " << RESET;
    cout << "\n\n\n\033[3A";
    LeaveCriticalSection(&consoleCS);
    cin >> q[3];
    drawBottomPadding();
    EnterCriticalSection(&consoleCS);
    cout << BOLD << CYAN << "  5. MISSION NATURE: Is this session for academic study or scaling your innovation startup?\n     (study / startups): " << RESET;
    cout << "\n\n\n\033[3A";
    LeaveCriticalSection(&consoleCS);
    cin >> q[4];
    drawBottomPadding();
    EnterCriticalSection(&consoleCS);
    cout << BOLD << CYAN << "  6. COLLABORATION STYLE: Do you prioritize networking potential or solitary focus?\n     (startups / quiet): " << RESET;
    cout << "\n\n\n\033[3A";
    LeaveCriticalSection(&consoleCS);
    cin >> q[5];
    drawBottomPadding();
    EnterCriticalSection(&consoleCS);
    cout << endl;
    LeaveCriticalSection(&consoleCS);
    for (int i = 0; i < count; i++)
    {
        double score = 0;
        if (places[i].tags.find(q[0]) != string::npos)
            score += 17;
        if (places[i].tags.find(q[1]) != string::npos)
            score += 17;
        if (places[i].tags.find(q[2]) != string::npos)
            score += 17;
        if (places[i].tags.find(q[3]) != string::npos)
            score += 17;
        if (places[i].tags.find(q[4]) != string::npos)
            score += 16;
        if (places[i].tags.find(q[5]) != string::npos)
            score += 16;
        places[i].matchScore = score;
    }
    for (int i = 0; i < count - 1; i++)
        for (int j = 0; j < count - i - 1; j++)
            if (places[j].matchScore < places[j + 1].matchScore)
            {
                StudyPlace t = places[j];
                places[j] = places[j + 1];
                places[j + 1] = t;
            }

    clear();
    showHeader();
    cout << "  Results:\n";
    for (int i = 0; i < 5 && i < count; i++)
        displayCardShort(places[i]);
    EnterCriticalSection(&consoleCS);
    cout << BOLD << GREEN << "\n  ID for Details (0 back): ";
    LeaveCriticalSection(&consoleCS);
    int id;
    cin >> id;
    if (id != 0)
        for (int i = 0; i < count; i++)
            if (places[i].id == id)
                displayFullDetail(places[i]);
}

void viewAllPlaces()
{
    clear();
    showHeader();
    cout << BOLD << YELLOW << "  SELECT GOVERNORATE TO VIEW ALL IN THICK MODE:\n"
         << RESET << endl;

    string govs[] = {"Cairo", "Alexandria", "Mansoura", "Qalyubia", "Monufia", "Hurghada", "Upper_Egypt"};
    for (int i = 0; i < 7; i++)
    {
        drawDoubleBoxMenuItem(i + 1, govs[i], (i % 2 == 0 ? MAGENTA : BLUE));
    }

    cout << BOLD << GREEN << "\n  >> " << RESET;
    cout << "\n\n\n\033[3A"; // Visual buffer
    int l;
    cin >> l;
    drawBottomPadding();
    if (l < 1 || l > 7)
        return;
    StudyPlace p[MAX_PLACES];
    int c;
    loadGovernorate(govs[l - 1], p, c);
    clear();
    showHeader();
    for (int i = 0; i < c; i++)
        displayCardShort(p[i], true);
    cout << BLUE << "===================================" << endl;
    cout << BLUE << "===================================" << endl;
    EnterCriticalSection(&consoleCS);
    cout << BOLD << GREEN << "\n  Enter ID for Details: ";
    LeaveCriticalSection(&consoleCS);

    int id;
    cin >> id;
    for (int i = 0; i < c; i++)
        if (p[i].id == id)
            displayFullDetail(p[i]);
}

void showFavorites()
{
    clear();
    showHeader();
    cout << BOLD << YELLOW << "\n  YOUR FAVORITES LIST:\n"
         << RESET;
    ifstream f("favorites.txt");
    string l;
    int idx = 1;
    Favorite favs[100];
    int count = 0;
    while (getline(f, l))
    {
        string e = split(l, '|');
        string pl = split(l, '|');
        string g = split(l, '|');
        if (e == currentUser.email)
        {
            favs[count] = {e, pl, g};
            cout << "  [" << idx++ << "] " << pl << " (" << g << ")\n";
            count++;
        }
    }
    f.close();
    if (count == 0)
    {
        cout << "  Empty.\n";
        system("pause");
        return;
    }
    cout << "\n  Select # to view details (0 back): ";
    int sel;
    cin >> sel;
    if (sel > 0 && sel <= count)
    {
        StudyPlace p[MAX_PLACES];
        int c;
        loadGovernorate(favs[sel - 1].govName, p, c);
        for (int i = 0; i < c; i++)
            if (p[i].name == favs[sel - 1].placeName)
                displayFullDetail(p[i]);
    }
}

void showUserActivity(int type)
{
    clear();
    showHeader();
    if (type == 6)
    {
        cout << BLUE << "\n  MY BOOKINGS / CREDIT:\n"
             << RESET;
        cout << "  Current Points: " << YELLOW << currentUser.points << RESET << endl;
        ifstream f("bookings.txt");
        string l;
        while (getline(f, l))
        {
            string e = split(l, '|');
            string pl = split(l, '|');
            string g = split(l, '|');
            string c = split(l, '|');
            if (e == currentUser.email)
                cout << "  - Booked " << pl << " (" << g << ") for " << c << " pts\n";
        }
        f.close();
    }
    else if (type == 9)
    {
        cout << MAGENTA << "\n  MY COMMENTS HISTORY:\n"
             << RESET;
        ifstream f("comments.txt");
        string l;
        while (getline(f, l))
        {
            string s = split(l, '|');
            string pl = split(l, '|');
            string t = split(l, '|');
            if (s == currentUser.fullName)
                cout << "  - On " << pl << ": " << t << endl;
        }
        f.close();
    }
    system("pause");
}

void showTopPlaces()
{
    clear();
    showHeader();
    cout << YELLOW << "\n  TOP RECOMMENDED PLACES (Rating > 9.0):\n"
         << RESET;
    string govs[] = {"Cairo", "Alexandria", "Mansoura", "Qalyubia", "Monufia", "Hurghada", "Upper_Egypt"};
    for (int i = 0; i < 7; i++)
    {
        StudyPlace p[MAX_PLACES];
        int c;
        loadGovernorate(govs[i], p, c);
        for (int j = 0; j < c; j++)
        {
            if (atof(p[j].rating.c_str()) >= 9.0)
                displayCardShort(p[j]);
        }
    }
    system("pause");
}

// --- Controller ---
void authController()
{
    clear();
    showHeader();
    cout << endl;
    EnterCriticalSection(&consoleCS);
    cout << BOLD << GREEN << "             --- SYSTEM ACCESS CENTER ---" << endl
         << endl;
    cout << BOLD;
    LeaveCriticalSection(&consoleCS);
    drawButton("[1] LOGIN (Founder)");
    drawButton("[2] REGISTER NEW ACCOUNT");
    drawButton("[3] TERMINATE SYSTEM", RED);
    EnterCriticalSection(&consoleCS);
    cout << RESET;
    cout << GREEN << "===============" << endl;
    cout << "\n=  Choice: =\n";
    cout << "===============" << RESET << endl;
    LeaveCriticalSection(&consoleCS);

    int c;
    cin >> c;
    cin.ignore();
    if (c == 1)
    {
        string n, e;
        cout << BOLD << CYAN << " Enter your  Name : ";

        getline(cin, n);
        cout << BOLD << CYAN << " Enter your  Email : ";

        cin >> e;
        if (loadUser(trim(n), trim(e)))
            isLoggedIn = true;
        else
        {
            cout << RED << "  Denied.\n";
            system("pause");
        }
    }
    else if (c == 2)
    {
        User u;
        cout << BOLD << CYAN << " Enter your  Name : ";
        getline(cin, u.fullName);
        cout << BOLD << CYAN << " Enter your  Email : ";

        cin >> u.email;
        cout << BOLD << CYAN << " Enter your  Password : ";

        cin >> u.password;
        u.recoveryCode = "RECOV-" + u.email.substr(0, 3);
        u.points = 100;
        u.isAdmin = false;
        registerUser(u);
        cout << GREEN << "  Registered! Code: " << u.recoveryCode << RESET << endl;
        system("pause");
    }
    else
        exit(0);
}
// =======================================================================
// =======================================================================
// =======================================================================
// =======================================================================
// =======================================================================
// =======================================================================
// =======================================================================
// =======================================================================
// =======================================================================
// =======================================================================

int main()
{
    InitializeCriticalSection(&consoleCS);
    // Start background thread for slogan animation
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)animateSloganLoop, NULL, 0, NULL);

    while (true)
    {
        if (!isLoggedIn)
            authController();
        else
        {
            clear();
            showHeader();
            EnterCriticalSection(&consoleCS);
            cout << "  FOUNDER: " << CYAN << currentUser.fullName << RESET << " | " << YELLOW << currentUser.points << " PTS" << RESET << endl;
            LeaveCriticalSection(&consoleCS);
            drawButton("[1] Filtered Search", GREEN);
            drawButton("[2] My Favorites", YELLOW);
            drawButton("[3] View All Places");
            drawButton("[4] Top Recommended", MAGENTA);
            drawButton("[6] My Bookings", BLUE);
            drawButton("[7] Quick Tags Search", WHITE);
            drawButton("[8] Settings", CYAN);
            drawButton("[9] My Comments", MAGENTA);
            drawButton("[0] Logout", RED);
            EnterCriticalSection(&consoleCS);
            cout << GREEN << "======================" << endl;
            cout << "======================" << RESET << endl;
            cout << BOLD << CYAN << "\n  Select Option: \n";
            cout << GREEN << "======================" << endl;
            cout << "======================" << RESET << endl;
            LeaveCriticalSection(&consoleCS);
            drawBottomPadding();

            int m;
            cin >> m;
            if (m == 1)
                searchPlaces();
            else if (m == 2)
                showFavorites();
            else if (m == 3)
                viewAllPlaces();
            else if (m == 4)
                showTopPlaces();
            else if (m == 6)
                showUserActivity(6);
            else if (m == 9)
                showUserActivity(9);
            else if (m == 0)
                isLoggedIn = false;
        }
    }
    return 0;
}
