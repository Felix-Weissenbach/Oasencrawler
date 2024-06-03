#ifndef WORLD_H
#define WORLD_H

class character;

class world
{
    public:
        world(int level);
        ~world();
        void printArea(character& player, character& enemy);
        char moveCharacter(character& player, char dir);
        bool relicsRemaining();
        int getSkillCheck() { return this->skillCheck; }
        void setSkillCheck(int x) { this->skillCheck = x; }

    private:
        char area[5][5];
        int level;
        int skillCheck;
};

#endif // WORLD_H
