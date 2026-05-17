#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib> // مكتبة ضرورية لدالة system("cls")
#include <conio.h> // مكتبة ضرورية لدالة _kbhit() للكشف عن الضغط على المفاتيح

using namespace std;

// دالة تحريك المؤشر
void gotoxy(short x, short y)
{
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void runAnimation()
{
    int startX = 10;
    int startY = 5;

    string lines[4] = {
        "   Co Work Space Founder       ",
        " - Where ideas meet,           ",
        " - Founders connect,           ",
        " - Great businesses begin.     "};

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // سأجعل الأنيميشن يتوقف إذا ضغطت على أي مفتاح في الكيبورد
    while (!_kbhit())
    {
        // 1. رسم الصندوق (باللون السماوي)
        SetConsoleTextAttribute(hConsole, 11);
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

        // 2. كتابة النص (بالأصفر)
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < (int)lines[i].length(); j++)
            {
                if (_kbhit())
                    return; // توقف إذا ضغط المستخدم مفتاحاً
                SetConsoleTextAttribute(hConsole, 14);
                gotoxy(startX + 2 + j, startY + 3 + i);
                cout << lines[i][j];
                cout.flush();
                Sleep(20);
            }
        }

        Sleep(2000); // انتظر قليلاً للقراءة

        // 3. مسح النص
        for (int i = 3; i >= 0; i--)
        {
            for (int j = (int)lines[i].length() - 1; j >= 0; j--)
            {
                if (_kbhit())
                    return;
                gotoxy(startX + 2 + j, startY + 3 + i);
                cout << " ";
                cout.flush();
                Sleep(10);
            }
        }

        Sleep(500);
    }
}

int main()
{

    SetConsoleTitleA("Co Work Space Founder - Animation Study");

    cout << "Welcome to the study file." << endl;
    cout << "The animation will start now. Press any key to stop it." << endl;
    system("pause");
    system("cls");

    runAnimation();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // إعادة اللون للأبيض العادي
    cout << "\n\nAnimation stopped. Study complete!" << endl;
    return 0;
}
