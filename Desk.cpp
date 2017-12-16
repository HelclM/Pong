#include "Classes.h"

int Desk::speed=15;

Desk::Desk(bool ileft){
    left=ileft;
    dest.h=80;
    dest.w=12;
    if(left){
        dest.x=10;
    }
    else{
        dest.x=Graphic::GAME_WINDOW_WIDTH-10-dest.w;
    }
    dest.y=Graphic::GAME_WINDOW_HEIGHT/2-(dest.h/2);
}

void Desk::goUp(){
    if(dest.y-Desk::speed>0){
        dest.y-=Desk::speed;
        up=true;
        std::this_thread::sleep_for(chrono::milliseconds(20));
    }
    up=false;
}

void Desk::goDown(){
    if(dest.y+dest.h+Desk::speed<Graphic::GAME_WINDOW_HEIGHT){
        dest.y+=Desk::speed;
        down=true;
        std::this_thread::sleep_for(chrono::milliseconds(20));
    }
    down=false;
}

SDL_Rect Desk::getRect(){
    std::lock_guard<std::mutex> guard(mu);
    return dest;
}

void Desk::logic(){
    char dir;
    while(!Input::exitgame){
        dir=q->takeFromBack();
        if(dir=='u'){
            goUp();
        }
        else if(dir=='d'){
            goDown();
        }
        while(Input::paused()){
            q->eraseAll();
            SDL_Delay(1000/60);
        }
    }
}

void Desk::setQeueRef(queue* iq){
    q=iq;
}

void Desk::reset(){
    if(left){
        dest.x=10;
    }
    else{
        dest.x=Graphic::GAME_WINDOW_WIDTH-10-dest.w;
    }
    dest.y=Graphic::GAME_WINDOW_HEIGHT/2-(dest.h/2);
    up=false;
    down=false;
    q->eraseAll();
}