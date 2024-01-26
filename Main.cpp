#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <cmath>
using namespace std;
using namespace std::chrono;

void Menu(int &Players, bool &isItemMode, int &Digits);
int power_10(int);
void MoveCursor(int, int);
void DeleteOutput(int);

int main()
{
    int Players = 1;
    bool isTimeMode = 0;
    int Digits = 1;
    int PlayerInput = 0;
    

    Menu(Players, isTimeMode, Digits);

    vector<vector<int>> CorrectAnswer(Players, vector<int>(Digits));            //玩家的正確答案 
    vector<vector<bool>> DigitsUsed(Players, vector<bool>(10));                 //玩家猜的數字用過了沒
    srand(time(0));

    for(int i = 0 ; i < Players ; i++)
    { 
        for(int j = 0 ; j < Digits ; j++)
        {
            do
            {
                CorrectAnswer[i][j] = rand() % 9 + 1;                           //產生數字 1-9
            }
            while(DigitsUsed[i][CorrectAnswer[i][j]]);                          //數字被用過的話就重新產生
            DigitsUsed[i][CorrectAnswer[i][j]] = true;                          //紀錄數字被用過了
        }
    }
    
    for(int i = 1 ; i <= Players ; i++)
    {
        cout << "Player " << i << "          ";
    }
    cout << "\n\n";
    
    vector<int> NowPlayerGuess(Digits);                                         //玩家猜的數字
                                                                                //以下正式開始遊戲
    int PlayerEndGame = 0;                                                      //紀錄過關的人
    int PlayRounds = 1;                                                         //紀錄回合數
    double TimeOfThisGame = 0;                                                  //記錄玩家這場遊戲用的時間
    int InputDigits;
    vector<bool> isAnswerCorrect(Players);                                      //記錄各個玩家過關了沒
    vector<int> PlayerSpendRounds(Players);                                     //記錄各個玩家過關花了幾回合
    vector<int> PlayerPlace(Players);                                           //紀錄玩家名次(P2,P6,P1...)
    vector<double> PlayerSpendTime(Players);                                    //紀錄玩家所花的時間

    int a, b;
    steady_clock::time_point GameStart, GameEnd;

    GameStart = steady_clock::now();

    while(PlayerEndGame != Players)
    {
        for(int NowPlayer = 0 ; NowPlayer < Players ; NowPlayer++)
        {
            a = 0;
            b = 0;
            if(!isAnswerCorrect[NowPlayer])
            {
                
                cout << "輸入數字:";
                cin >> PlayerInput;

                for(int i = 0 ; PlayerInput >= power_10(i) ; i++)
                {
                    InputDigits = i + 1;
                }

                for(int i = 0 ; i < Digits ; i++)                                       //將玩家的輸入轉換成陣列
                {
                    NowPlayerGuess[i] = PlayerInput % ((int)power_10(Digits - i)) / power_10(Digits - i - 1);
                }

                for(int i = 0 ; i < Digits ; i++)
                {
                    if (NowPlayerGuess[i] == CorrectAnswer[NowPlayer][i])                   //判斷A
                    {
                        a++;
                    }
                    else if (DigitsUsed[NowPlayer][NowPlayerGuess[i]])                  //判斷B
                    {
                        b++;
                    }
                }
                if(a == Digits)                                                             //正確答案
                {
                    GameEnd = steady_clock::now();
                    TimeOfThisGame = duration_cast<milliseconds>(GameEnd - GameStart).count() / 1000.0;
                    PlayerSpendTime[NowPlayer] = TimeOfThisGame;
                    PlayerSpendRounds[NowPlayer] = PlayRounds;
                    PlayerPlace[PlayerEndGame] = NowPlayer;
                    PlayerEndGame++;
                    isAnswerCorrect[NowPlayer] = true;
                    MoveCursor(18 * NowPlayer, 1);
                    cout << "                  ";
                    MoveCursor(-18, -1);
                    cout << "Correct !!        ";                               
                    MoveCursor(-18, -1);
                    cout << "The answer is:    ";
                    MoveCursor(-18, -1);
                    cout << PlayerInput;
                    MoveCursor(18 - Digits, 3);
                }
                else
                {
                    MoveCursor(18 * NowPlayer, 1);
                    cout << PlayerInput << " > " << a << "A" << b << "B";      //輸出幾A幾B
                    for(int i = 0 ; i < 11 - InputDigits ; i++)
                    {
                        cout << " ";
                    }
                    cout << "\r";
                    MoveCursor(18 * (NowPlayer + 1), 0);
                }
            }
            else
            {
                MoveCursor(18, 0);
            }
        }
        cout << "\n";
        PlayRounds++;
    }
    
    cout << "\n\n\n\n\n";                                                               //記分板 名次 次數 時間
    cout << " Place\t\tRounds\t\tTime\n";
    for(int i = 0 ; i < Players ; i++)
    {
        cout << " Player " << (PlayerPlace[i] + 1) << "\t";
        cout << PlayerSpendRounds[PlayerPlace[i]] << "\t\t";
        cout << PlayerSpendTime[PlayerPlace[i]] << " seconds\n";
    }

    return 0;
}




void DeleteOutput(int x)
{
    for(int i = 0 ; i < x ; i++)
    {
        cout << "\033[1C \033[1C";
    }
}
void MoveCursor(int x, int y)
{
    if(x > 0)
    {
        for(int i = 0 ; i < x ; i++)
        {   cout << "\033[1C";  }
    }
    else
    {
        for(int i = 0 ; i < abs(x) ; i++)
        {   cout << "\033[1D";  }
    }
    if(y > 0)
    {
        for(int i = 0 ; i < y ; i++)
        {   cout << "\033[1A";  }
    }
    else
    {
        for(int i = 0 ; i < abs(y) ; i++)
        {   cout << "\033[1B";  }
    }
}

int power_10(int x)
{
   int a = 1;
   for(int i = 0 ; i < x ; i++)
   {
      a *= 10;
   }
   return a;
}   

void Menu(int &Players, bool &isItemMode, int &Digits)
{
    system("chcp 65001");
    cout << "\033[1A   >>>  Welcome to 1A2B !  <<<\n\n\n";
    int temp = 0;
    do
    {
        cout << "請選擇輸入遊玩人數:\n";
        cout << "最低 1 人\n";
        cout << "最多 8 人\033[2A\033[10C";
        cin >> Players;
        if(Players < 1)
        {   cout << "蛤?太少人了! \n          \n";    }
        else if(Players > 8)
        {   cout << "太多人了!    \n          \n";    }
    }
    while (Players < 1 || Players > 8);
    
    do
    {   
        cout << "是否開啟道具功能:\n";
        cout << "0 : 否      \n";
        cout << "1 : 是      \033[2A\033[5C";
        cin >> temp;
        if(!(temp == 0 || temp == 1))
        {   cout << "輸入錯誤! 請重新輸入!\n            \n";    }
    }
    while (!(temp == 0 || temp == 1));
    isItemMode = temp;

    do
    {
        cout << "設定答案位數:\n";
        cout << "最少 1 位數\n";
        cout << "最多 9 位數\033[2A\033[2C";
        cin >> Digits;
        if(Digits < 1 || Digits > 9)
        {   cout << "輸入錯誤! 請重新輸入!\n            \n";    }
    }
    while (Digits < 1 || Digits > 9);
    

    cout << "           \n";
    cout << "           \n";
    cout << "           \n";
    cout << "玩家人數 : " << Players << "人\n";
    cout << "道具功能 : ";
    if (isItemMode)
    {   cout << "開啟\n";   }
    else
    {   cout << "關閉\n";   }
    cout << "答案位數 : " << Digits << "位\n\n";
    cout << "輸入任意數字開始遊戲:";
    cin >> temp;
    cout << "\033[2J\033[1;1H";
}
