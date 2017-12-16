#include"Classes.h"

Button::Button(string iname,int n){
    name=iname;
    dest.x=Graphic::MENU_WINDOW_WIDTH/6;
    dest.h=Graphic::MENU_WINDOW_HEIGHT/7;
    dest.w=2*Graphic::MENU_WINDOW_WIDTH/3;
    dest.y=(n-1)*(dest.h+Graphic::MENU_WINDOW_HEIGHT/15)+Graphic::MENU_WINDOW_HEIGHT/15;
}

Button::Button(string iname,int x, int y, int h, int w){
    name=iname;
    dest.x=x;
    dest.y=y;
    dest.h=h;
    dest.w=w;
}

bool Button::pressed(int x, int y){
    if(x>(dest.x+dest.w)){
        return false;
    }
    if(x<dest.x){
        return false;
    }
    if(y>(dest.y+dest.h)){
        return false;
    }
    if(y<dest.y){
        return false;
    }
    
    return true;
}

SDL_Rect Button::getRect(){
    return dest;
}

string Button::getName(){
    return name;
}