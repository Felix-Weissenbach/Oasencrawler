#ifndef CHARACTER_H
#define CHARACTER_H


class character
{
    public:
        character();
        character(int x, int y);
        ~character();

        int Getx() { return x; }
        int Gety() { return y; }
        int getDistance(int a, int b);
        void setPosition(int x, int y);
        int getHealth() { return health; }
        int getRelics() { return relics; }
        void moveUp() { this->y--; }
        void moveDown() { this->y++; }
        void moveLeft() { this->x--; }
        void moveRight() { this->x++; }
        int steppedOn(char type);
        char getStatus() { return status; }
        void resetStatus() { status = 'e'; }
        void levelUp();
        void finalStage() { status = 'F'; }
        bool healthRemaining() { return this->health != 0; }
        void moveEnemy(character& player);
        int getItem(int i) { return this->items[i]; }
        int getAttribute(int i) { return this->attributes[i]; }
        bool isDamaged() { return this->damaged;}
        void resetDamage() { this->damaged = false; }
        void decItem(char i);

    protected:

    private:
        int x;
        int y;
        int health;
        int relics;
        int level;
        char status;
        bool damaged;
        int attributes[3];
        int items[4];

        void decHealth() { this->health--; }
        void incHealth() { this->health++; }
        void incRelics() { this->relics++; }
        void incItem(char i);
        void tookDamage() { this->damaged = true; }
        void setStatus(char newStatus) { this->status = newStatus; }
        void incAttribute(int i) { this->attributes[i]++; }
};

#endif // CHARACTER_H
