#include "character.h"
#include "world.h"
#include <cstdlib>
#include <time.h>
#include <io.h>
#include <fcntl.h>
#include <cstdio>
#include <iostream>
#include <windows.h>    //für farben; funktionert nur auf windows

world::world(int level)
{
    this->level = level;
    this->skillCheck = 0;
    //e == Leeres Feld 4/10 && ! == Gefahr 4/10 && W == Brunnen 1/10 && R == relic 1/10
    //anfangs werden die ersten zehn indizes random gewählt
    //mit jedem level wird die chance ein leeres Feld zu bekommen geringer und die chance für ein Gefahrenfeld höher
    char types[] = {'e', 'e', 'e', 'e', 'W', '!', '!', '!', '!', 'R', '!', '!', '!', '!', '!'};
    int relicCheck = 0;
    int randIndex;
    srand(time(NULL));

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(i == 0 && j == 0)
            {
                this->area[i][j] = 'e';
                j++;
            }
            randIndex = rand() % 10 + level;
            //stellt sicher dass mind 1 relic existiert
            if(randIndex == 9)
                relicCheck++;
            if(!relicCheck && (i == 4 && j == 4))
                randIndex = 9;

            this->area[i][j] = types[randIndex];
        }
    }
}

world::~world()
{
    //dtor
}

bool world::relicsRemaining()
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(this->area[i][j] == 'R') return true;
        }
    }
    return false;
}

void world::printArea(character& player, character& enemy)
{
    using namespace std;

    int xP = player.Getx();
    int yP = player.Gety();
    int xE = enemy.Getx();
    int yE = enemy.Gety();

    enemy.moveEnemy(player);

    #if defined _WIN32 //temp lösung, funktioniert nur auf windows, system ist unsafe; link: https://stackoverflow.com/questions/6486289/how-can-i-clear-console
    system("cls");
    #endif

    cout << " LIVES ";
    if(player.getHealth() < 10)
        cout << " ";
    cout << player.getHealth();
    cout << "     LEVEL " << this->level << endl;
    cout << "RELICS ";
    if(player.getRelics() < 10)
        cout << " ";
    cout << player.getRelics() << endl;

    //ändert translation mode zu UTF16 für box-drawing characters und speichert den vorherigen mode in 'previousMode'
    int previousMode = _setmode(_fileno(stdout), _O_U16TEXT);

    wcout << L"┌───┬───┬───┬───┬───┐" << endl;
    for(int i = 0; i < 5; i++)
    {
        wcout << L"│";
        for(int j = 0; j < 5; j++)
        {
            if(i == yP && j == xP)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                wcout << L" ⌂";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                wcout << L" │";
            }
            else if(i == yE && j == xE)
            {
                if(area[j][i] == 'R')
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 236);
                else if(area[j][i] == 'W')
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 188);
                else
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                wcout << L" Ħ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                wcout << L" │";
            }
            else
            {
                switch(area[j][i])
                {
                case 'e':
                    wcout << L"   │";
                    break;
                case '!':
                    wcout << L" ! │";
                    break;
                case 'W':
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                    wcout << L" Ʊ";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                    wcout << L" │";
                    break;
                case 'R':
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                    wcout << L" §";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                    wcout << L" │";
                    break;
                }
            }
        }
        wcout << endl;
        if(i != 4)
        {
            wcout << L"├───┼───┼───┼───┼───┤";
            switch(i)
            {
            case 0:
                wcout << L"        SHIELD";
                break;
            case 1:
                wcout << L"        AMULET";
                break;
            case 2:
                wcout << L"      MEDICINE";
                break;
            case 3:
                wcout << L"    MAGIC LAMP";
                break;
            }
            wcout << L"    " ;

            if(player.getItem(i) < 10)
                wcout << L" " ;

            wcout << player.getItem(i) << L"    │    ";
            if(i != 3)
            {

                switch(i)
                {
                case 0:
                    wcout << L"DEXTERITY";
                    break;
                case 1:
                    wcout << L"    MAGIC";
                    break;
                case 2:
                    wcout << L"    VIGOR";
                    break;
                }
                wcout << L"    " ;
                if(player.getAttribute(i) < 10)
                    wcout << L" " ;

                wcout << player.getAttribute(i) << L"    ";
            }

            wcout << endl;
        }
    }
    wcout << L"└───┴───┴───┴───┴───┘" << endl;
    //ändert translation mode zurück zum vorherigen, sodass cout mit non-wide strings verwendet werden kann
    _setmode(_fileno(stdout), previousMode);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    switch(player.getStatus())
    {
    case 'N':
        _setmode(_fileno(stdout), _O_U16TEXT);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        wcout << L"⌂ ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        wcout << L" ==  PLAYER   -->   WSAD == ↑↓←→" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        wcout << L"Ħ ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        wcout << L" ==   ENEMY   -->   ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 236);
        wcout << L"Ħ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        wcout << L" == STANDS ON RELIC; ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 188);
        wcout << L"Ħ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        wcout << L" == STANDS ON WELL" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        wcout << L"! ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        wcout << L" ==  DANGER   -->   1/6 CHANCE FOR --LIVES" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        wcout << L"Ʊ ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        wcout << L" ==    WELL   -->   ++LIVES" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        wcout << L"§ ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        wcout << L" ==   RELIC   -->   ALL COLLECTED == LEVEL UP" << endl;
        _setmode(_fileno(stdout), previousMode);
        break;
    case '+':
        cout << "!!!LEVEL UP!!!" << endl;
        player.resetStatus();
        break;
    case 'F':
        cout << "-----FINAL STAGE-----" << endl;
        player.resetStatus();
        break;
    case '0':
        srand(time(NULL));
        switch(rand() % 3)
        {
        case 0:
            cout << "SPIKES SHOOT OUT OF THE WALL! ";
            break;
        case 1:
            cout << "A FIREBALL EXPLODES IN YOUR FACE! ";
            break;
        case 2:
            cout << "YOU COUGH UP BLOOD! ";
            break;
        }
        break;
    case '1':
        cout << "SPIKES SHOOT OUT OF THE WALL! ";
        break;
    case '2':
        cout << "A FIREBALL EXPLODES IN YOUR FACE! ";
        break;
    case '3':
        cout << "YOU COUGH UP BLOOD! ";
        break;
    case '4':
        cout << "YOU FIND A SHIELD!";
        switch(this->getSkillCheck())
        {
        case 100:
            cout << " AND FEEL A BIT QUICKER!" << endl;
            break;
        case 101:
            cout << " AND FEEL MAGIC RUNNING THROUGH YOUR BODY!" << endl;
            break;
        case 102:
            cout << " AND FEEL A BIT HEALTHIER!" << endl;
            break;
        }
        player.resetStatus();
        break;
    case '5':
        cout << "YOU FIND A MAGIC AMULET!";
        switch(this->getSkillCheck())
        {
        case 100:
            cout << " AND FEEL A BIT QUICKER!" << endl;
            break;
        case 101:
            cout << " AND FEEL MAGIC RUNNING THROUGH YOUR BODY!" << endl;
            break;
        case 102:
            cout << " AND FEEL A BIT HEALTHIER!" << endl;
            break;
        }
        player.resetStatus();
        break;
    case '6':
        cout << "YOU FIND SOME MEDICINE!";
        switch(this->getSkillCheck())
        {
        case 100:
            cout << " AND FEEL A BIT QUICKER!" << endl;
            break;
        case 101:
            cout << " AND FEEL MAGIC RUNNING THROUGH YOUR BODY!" << endl;
            break;
        case 102:
            cout << " AND FEEL A BIT HEALTHIER!" << endl;
            break;
        }
        player.resetStatus();
        break;
    case '7':
        cout << "YOU FIND A MAGIC LAMP!";
        switch(this->getSkillCheck())
        {
        case 100:
            cout << " AND FEEL A BIT QUICKER!" << endl;
            break;
        case 101:
            cout << " AND FEEL MAGIC RUNNING THROUGH YOUR BODY!" << endl;
            break;
        case 102:
            cout << " AND FEEL A BIT HEALTHIER!" << endl;
            break;
        }
        player.resetStatus();
        break;
    case 'd':
        cout << "YOU ROLLED: " << this->getSkillCheck() << "/10 DODGED - LUCKY!" << endl;
        player.resetStatus();
        break;
    }
    if(player.isDamaged())
    {
        if(xP == xE && yP == yE)
            cout << "THE ENEMY HURT YOU! YOU TOOK 1 DAMAGE!" << endl;
        if(this->getSkillCheck() > 0 && this->getSkillCheck() < 10)
            cout << "YOU ROLLED: " << this->getSkillCheck() << "/10 YOU TOOK 1 DAMAGE!" << endl;
        player.resetDamage();
        player.resetStatus();
    }
    else
    {
        bool usedItem = true;
        switch(player.getStatus())
        {
        case '0':
            cout << "BUT YOU RUB THE MAGIC LAMP AND THE DANGER VANISHES" << endl;
            player.resetStatus();
            break;
        case '1':
            cout << "BUT YOUR SHIELD BLOCKS THEM AND BREAKS!" << endl;
            player.resetStatus();
            break;
        case '2':
            cout << "BUT YOUR AMULET DISPELS THE FIREBALL AND GETS USED UP!" << endl;
            player.resetStatus();
            break;
        case '3':
            cout << "BUT THE MEDICINE HELPS!" << endl;
            player.resetStatus();
            break;
        default:
            usedItem = false;
            break;
        }

        if(usedItem)
        {
            player.decItem(player.getStatus());
            player.resetStatus();
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

char world::moveCharacter(character& player, char dir)
{
    switch(dir)
    {
    case 'w':
        player.moveUp();
        break;
    case 'a':
        player.moveLeft();
        break;
    case 's':
        player.moveDown();
        break;
    case 'd':
        player.moveRight();
        break;
    }
    int x = player.Getx();
    int y = player.Gety();

    char newField = this->area[x][y];
    this->area[x][y] = 'e';
    return newField;
}
