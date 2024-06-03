#include <iostream>
#include <limits>
#include "character.h"
#include "world.h"
#include <conio.h>


using namespace std;

int main()
{
    int stage = 0;
    int result;
    bool alive = true;
    character* player = new character();
    character* enemy = new character(3, 3);
    do
    {
        world* game = new world(stage);
        player->setPosition(0, 0);
        enemy->setPosition(3, 3);
        char currentField;
        while(true)
        {
            game->printArea(*player, *enemy);

            if(!game->relicsRemaining())
            {
                break;
            }

            if(!player->healthRemaining())
            {
                alive = false;
                break;
            }

            char input;
            bool isValid = false;
            do
            {
                input = getch();  //liest genau einen character ohne die Enter Taste zu drücken; flüssigeres Spiel aber keine so gute Eingabeüberprüfung
                //cin >> input;
                if(cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else
                {
                    input = tolower(input);
                    switch(input)
                    {
                    case 'w':
                        if(player->Gety() != 0)
                            isValid = true;
                        break;
                    case 'a':
                        if(player->Getx() != 0)
                            isValid = true;
                        break;
                    case 's':
                        if(player->Gety() != 4)
                            isValid = true;
                        break;
                    case 'd':
                        if(player->Getx() != 4)
                            isValid = true;
                        break;
                    default:
                        break;
                    }
                }
            }
            while(!isValid);
            currentField = game->moveCharacter(*player, input);
            result = player->steppedOn(currentField);
            game->setSkillCheck(result);
        }
        if(alive)
        {
            delete game;
            if(stage < 5)
            {
                player->levelUp();
                if(stage == 4)
                    player->finalStage();
                stage++;
            }
            else
            {
                #if defined _WIN32
                system("cls");
                #endif
                cout << " _ _ _ _ ___   _____  _   _  __        _____ _   _ _ _ _ _ _ " << endl;
                cout << "| | | | | \\ \\ / / _ \\| | | | \\ \\      / /_ _| \\ | | | | | | |" << endl;
                cout << "| | | | | |\\ V / | | | | | |  \\ \\ /\\ / / | ||  \\| | | | | | |" << endl;
                cout << "|_|_|_|_|_| | || |_| | |_| |   \\ V  V /  | || |\\  |_|_|_|_|_|" << endl;
                cout << "(_|_|_|_|_) |_| \\___/ \\___/     \\_/\\_/  |___|_| \\_(_|_|_|_|_)" << endl;
                delete player;
                break;
            }
        }
        else
        {
            #if defined _WIN32
            system("cls");
            #endif
            cout << "__   _____  _   _   _     ___  ____  _____ " << endl;
            cout << "\\ \\ / / _ \\| | | | | |   / _ \\/ ___|| ____|" << endl;
            cout << " \\ V / | | | | | | | |  | | | \\___ \\|  _|  " << endl;
            cout << "  | || |_| | |_| | | |__| |_| |___) | |___ " << endl;
            cout << "  |_| \\___/ \\___/  |_____\\___/|____/|_____|" << endl;
            delete player;
            break;
        }
    } while(alive);
    return 0;
}
