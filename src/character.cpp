#include "character.h"
#include <cstdlib>
#include <iostream>
#include <time.h>

character::character()
{
    this->x = 0;
    this->y = 0;
    this->health = 5;
    this->relics = 0;
    this->level = 0;
    this->damaged = false;
    this->status = 'N';

    srand(time(NULL));

    for(int i = 0; i < 3; i++)
    {
        this->items[i] = rand() % 3;
    }
    this->items[3] = 0;

    for(int i = 0; i < 3; i++)
    {
        this->attributes[i] = rand() % 5 + 1;
    }
}

character::character(int x, int y) : items()
{
    this->x = x;
    this->y = y;
    this->health = 3;
    this->relics = -1;
    this->level = 0;
    this->damaged = false;
    this->status = 'X';
}

character::~character()
{
    //dtor
}

void character::incItem(char i)
{
    int temp;
    switch(i)
    {
    case '1':
        temp = 0;
        break;
    case '2':
        temp = 1;
        break;
    case '3':
        temp = 2;
        break;
    case '4':
        temp = 3;
        break;
    }
    this->items[temp]++;
}

void character::decItem(char i)
{
    int temp;
    switch(i)
    {
    case '1':
        temp = 0;
        break;
    case '2':
        temp = 1;
        break;
    case '3':
        temp = 2;
        break;
    case '4':
        temp = 3;
        break;
    }
    if(this->items[temp] > 0)
        this->items[temp]--;
    else
        this->items[temp] = 0;
}

void character::levelUp()
{
    status = '+';
    this->level++;
}

int character::steppedOn(char type)
{
    //30% chance auf jedes Item, 10% auf Wildcard
    char items[10] = {'1', '2', '3', '1', '1', '2', '2', '3', '3', '4'};

    srand(time(NULL));
    int item = rand() % 10;
    int event = rand() % 3;
    int eventAtt = this->getAttribute(event);

    switch(type)
    {
    case 'e':
        this->setStatus('e');
        break;
    case '!':
        if(this->items[event] == 0 && this->items[3] == 0)
        {
            int hurt = rand() % 10 + eventAtt;
            if(hurt < 10)
            {
                this->decHealth();
                this->tookDamage();
                this->setStatus(items[event]);
            }
            else
            {
                this->setStatus('d');
            }
            return hurt;

        }
        else if(this->items[event] != 0)
        {
            this->decItem(items[event]);
            this->setStatus(items[event]);
        }
        else
        {
            this->decItem('4');
            this->setStatus('0');
        }
        break;
    case 'W':
        this->incHealth();
        this->incItem(items[item]);
        this->setStatus(items[item] + 3);
        this->incAttribute(event);
        return (event+100);
        break;
    case 'R':
        this->incRelics();
        this->incItem(items[item]);
        this->setStatus(items[item] + 3);
        this->incAttribute(event);
        return (event+100);
        break;
    }
    return 0;
}

int character::getDistance(int a, int b)
{
    if(a-b < 0)
        return (a-b)*-1;
    else
        return a-b;
}

void character::moveEnemy(character& player)
{

    int xP = player.Getx();
    int yP = player.Gety();
    int xE = this->Getx();
    int yE = this->Gety();

    int xDist = getDistance(xP, xE);
    int yDist = getDistance(yP, yE);

    bool right = xP > xE;
    bool left = xP < xE;
    bool down = yP > yE;
    bool up = yP < yE;

    bool random[2] = {true, false};

    if(right)
    {
        if(down)
        {
            if(yDist < xDist)
            {
                this->moveRight();
            }
            else if(yDist > xDist)
            {
                this->moveDown();
            }
            else
            {
                if(random[rand() % 2])
                    this->moveRight();
                else
                    this->moveDown();
            }
        }
        else if(up)
        {
            if(yDist < xDist)
            {
                this->moveRight();
            }
            else if(yDist > xDist)
            {
                this->moveUp();
            }
            else
            {
                if(random[rand() % 2])
                    this->moveRight();
                else
                    this->moveUp();
            }
        }
        else
        {
            this->moveRight();
        }
    }
    else if(left)
    {
        if(down)
        {
            if(yDist < xDist)
            {
                this->moveLeft();
            }
            else if(yDist > xDist)
            {
                this->moveDown();
            }
            else
            {
                if(random[rand() % 2])
                    this->moveLeft();
                else
                    this->moveDown();
            }
        }
        else if(up)
        {
            if(yDist < xDist)
            {
                this->moveLeft();
            }
            else if(yDist > xDist)
            {
                this->moveUp();
            }
            else
            {
                if(random[rand() % 2])
                    this->moveLeft();
                else
                    this->moveUp();
            }
        }
        else
        {
            this->moveLeft();
        }
    }
    else
    {
        if(down)
            this->moveDown();
        else if(up)
            this->moveUp();
        else
        {
            player.decHealth();
            player.tookDamage();
            if(x != 0)
                this->moveLeft();
            else
                this->moveRight();
        }
    }
}

void character::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}
