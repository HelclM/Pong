#include"Classes.h"

Ball::Ball(Desk* ideskP,Desk* ideskL){
    deskP=ideskP;
    deskL=ideskL;
    dest.w=10;
    dest.h=dest.w;
    dest.x=Graphic::GAME_WINDOW_WIDTH/2;
    dest.y=Graphic::GAME_WINDOW_WIDTH/2;
    xspeed=maxspeed/2;
    yspeed=0;
    scoreP=0;
    scoreL=0;
}

void Ball::reset(){
    dest.x=Graphic::GAME_WINDOW_WIDTH/2;
    dest.y=Graphic::GAME_WINDOW_WIDTH/2;
    xspeed=-xspeed;
    yspeed=0;
    SDL_Delay(1000);
}

int Ball::getScoreL(){
    return scoreL;
}

int Ball::getScoreP(){
    return scoreP;
}

void Ball::logic(){
    bool moved=false;
    while(!Input::exitgame){
        if(dest.x+xspeed<0){
            scoreP+=1;
            reset();
            moved=true;
        }
        if (dest.y+yspeed<0 || (dest.y+dest.h)>Graphic::GAME_WINDOW_HEIGHT){
            yspeed=-yspeed;
            dest.y+=yspeed;
            moved=true;
        }
        if (dest.x+dest.w>Graphic::GAME_WINDOW_WIDTH){
            scoreL+=1;
            reset();
            moved=true;
        }
        if(check_collision(dest,deskP->getRect())){//collision with right desk
            deskCollision(deskP);
            moved=true;
        }
        if(check_collision(dest,deskL->getRect())){//collision with left desk
            deskCollision(deskL);
            moved=true;
        }
        if(!moved){
            dest.x+=xspeed;
            dest.y+=yspeed;
        }
        moved=false;
        SDL_Delay(1000/60);
        if(scoreL==10 || scoreP==10){
            Input::exitgame=true;
        }
        while(Input::paused()){
            SDL_Delay(1000/60);
        }
    }
}
SDL_Rect Ball::getRect(){
    std::lock_guard<std::mutex> guard(mu);
    return dest;
}

bool Ball::check_collision(SDL_Rect A, SDL_Rect B){
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

void Ball::deskCollision(Desk* desk){
    if(desk->down){
        yspeed+= Desk::speed/3;
        if(yspeed>maxyspeed){
            yspeed=maxyspeed;
        }
    }
    else if(desk->up){
        yspeed-=Desk::speed/3;
        if(yspeed<-maxyspeed){
                yspeed=-maxyspeed;
        }
    }
    if(dest.x<Graphic::GAME_WINDOW_WIDTH/2){
        xspeed=abs(xspeed);
    }
    else{
        xspeed=-abs(xspeed);
    }
    dest.x+=xspeed;
    dest.y+=yspeed;
}

void Ball::resetAll(){
    scoreL=0;
    scoreP=0;
    reset();
}