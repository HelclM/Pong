#include<cstdlib>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<string>
#include<thread>
#include<mutex>
#include<deque>
#include<condition_variable>
#include<sstream>
#include<iostream>
#include<cmath>
#include<ctime>
#include<vector>

using namespace std;

struct coord{
    int x;
    int y;
};

struct menu{
    bool pvp=false;
    bool pve=false;
    bool eve=false;
};

class queue{
private:
    std::mutex mu;
    std::deque<char> q;
    std::condition_variable cond;
public:
    void eraseAll();
    bool empty();
    void pushFront(char a);
    char takeFromBack();
    void notifyAll();
};

class Desk{
private:
    std::mutex mu;
    SDL_Rect dest;
    queue* q;
    void goUp();
    void goDown();
    bool left;
public:
    static int speed;
    SDL_Rect getRect();
    bool up=false;
    bool down=false;
    Desk(bool left);
    void setQeueRef(queue* iq);
    void reset();
    void logic();
};

class Ball{
private:
    std::mutex mu;
    SDL_Rect dest;
    int maxspeed=15;
    int maxyspeed=10;
    int xspeed;
    int yspeed;
    int scoreL;
    int scoreP;
    Desk* deskL;
    Desk* deskP;
    void reset();
    void deskCollision(Desk * desk);
    bool check_collision( SDL_Rect A, SDL_Rect B );
public:
    void resetAll();
    SDL_Rect getRect();
    Ball(Desk* ideskP,Desk* ideskL);
    int getScoreP();
    int getScoreL();
    void logic();
};

class Button{
private:
    string name;
    SDL_Rect dest;
public:
    Button(string iname,int n);
    Button(string iname,int x,int y, int h, int w);
    SDL_Rect getRect();
    string getName();
    bool pressed(int x, int y);
};

class Graphic{
private:
    SDL_Window* window=NULL;
    TTF_Font* font=NULL;
    SDL_Texture* backtex=NULL;
    SDL_Texture* desktex=NULL;
    SDL_Texture* balltex=NULL;
    SDL_Texture* scoreLtex=NULL;
    SDL_Texture* scorePtex=NULL;
    SDL_Renderer* rend=NULL;
    int scoreL=-1;
    int scoreP=-1;
    SDL_Color textColor;
    SDL_Rect scoreLdest;
    SDL_Rect scorePdest;
    SDL_Texture* loadImage(string path);
    void drawPic(SDL_Rect destpic, SDL_Texture* tex);
    SDL_Texture* RenderText(string text, SDL_Color textcolor);
    void drawButton(Button* button);
    SDL_Texture* fillRect(SDL_Rect dest,SDL_Color color);
    void resizeTextdest(SDL_Rect* dest,string text,int letterW,int letterH);
    void drawAnnouncement(string text,int letterW,int letterH);
public:
    static int GAME_WINDOW_WIDTH;
    static int GAME_WINDOW_HEIGHT;
    static int MENU_WINDOW_WIDTH;
    static int MENU_WINDOW_HEIGHT;
    void init();
    void createWinRen(string name,int h,int w);
    void resizeWindow(int w,int h);
    void loadResources();
    void drawGame(Desk* deskP,Desk* deskL,Ball* ball);
    void drawMenu(vector<Button>& buttons);
    void drawWinner(string winner);
    void cleanUp();
};

class Input{
private:
    queue qL;
    queue qP;
    menu mode;
    void notifyWaitingThreads();
    static bool pause;
public:
    static bool exitgame;
    static bool exitprogram;
    static bool exitmenu;
    queue* getqueueRefL();
    queue* getqueueRefP();
    void reset();
    menu getMode();
    static bool paused();
    void handleGameEvents();
    void handleMenuEvents(Button* pvp, Button* pve,Button* eve,Button* exit);
};

class Player{
protected:
    Ball* ball;
    queue q;
    Desk* desk;
public:
    queue* getQueueRef();
    void setDeskRef(Desk* idesk);
    void setBallRef(Ball* iball);
    void logic();
    void notifyWaitingThreads();
};

class Threads{
private:
    vector<thread> runningThreads;
public:
    void createThreadsPvP(Desk* deskL,Desk* deskP, Ball* ball);
    void createThreadsPvE(Desk* deskL,Desk* deskP, Ball* ball,Player* player1);
    void createThreadsEvE(Desk* deskL,Desk* deskP, Ball* ball,Player* player1,Player* player2);
    void joinAll();
};
