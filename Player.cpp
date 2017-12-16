#include"Classes.h"

void Player::logic(){
    int waittime=0;
    SDL_Rect ballrect;
    SDL_Rect deskrect;
    srand (time(NULL));
    int noMoveCounter=0;
    while(!Input::exitgame){
        ballrect=ball->getRect();
        deskrect=desk->getRect();
        if(abs(ballrect.y-(deskrect.y+deskrect.h/2))<deskrect.h/3){
            q.eraseAll();
            ++noMoveCounter;
        }
        else{
            noMoveCounter=0;
            if(ballrect.y<(deskrect.y+deskrect.h/2)){
                q.pushFront('u');
            }
            else{
                q.pushFront('d');
            }
        }
        if(noMoveCounter>100){
            if((rand() % 100)>50){
                desk->down=true;
            }
            else{
                desk->up=true;
            }
            noMoveCounter=0;
        }
        waittime= rand() % 45;
        std::this_thread::sleep_for(chrono::milliseconds(waittime));
        while(Input::paused()){
            SDL_Delay(1000/60);
        }
    }
}

queue* Player::getQueueRef(){
    return &q;
}

void Player::setBallRef(Ball* iball){
    ball=iball;
}

void Player::setDeskRef(Desk* idesk){
    desk=idesk;
}

void Player::notifyWaitingThreads(){
    q.notifyAll();
}