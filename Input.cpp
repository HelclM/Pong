#include"Classes.h"

bool Input::exitgame=false;
bool Input::exitprogram=false;
bool Input ::exitmenu=false;
bool Input::pause=false;

queue* Input::getqueueRefL(){
    return &qL;
}

queue* Input::getqueueRefP(){
    return &qP;
}

void Input::handleGameEvents(){
    const Uint8* state=SDL_GetKeyboardState(NULL);
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if (event.type==SDL_QUIT){
            Input::exitgame=true;
            Input::pause=false;
        }
    }
    if(state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN]){
        qP.pushFront('u');
    }
    if(state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_UP]){
        qP.pushFront('d');
    }
    if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S]){
        qL.pushFront('u');
    }
    if (state[SDL_SCANCODE_S] && !state[SDL_SCANCODE_W]){
        qL.pushFront('d');
    }
    if(state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]){
        pause=true;
    }
    if(state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]){
        pause=false;
    }
}

void Input::notifyWaitingThreads(){
    qL.notifyAll();
    qP.notifyAll();
}

void Input::handleMenuEvents(Button* pvp, Button* pve,Button* eve,Button* exit){
    coord mouse;
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if (event.type==SDL_QUIT){
            Input::exitmenu=true;
            Input::exitprogram=true;
        }
    }
    if(SDL_GetMouseState(&mouse.x,&mouse.y) & SDL_BUTTON(SDL_BUTTON_LEFT)){
        if(pvp->pressed(mouse.x,mouse.y)){
            mode.pvp=true;
            Input::exitmenu=true;
        }
        if(pve->pressed(mouse.x,mouse.y)){
            mode.pve=true;
            Input::exitmenu=true;
        }
        if(eve->pressed(mouse.x,mouse.y)){
            mode.eve=true;
            Input::exitmenu=true;
        }
        if(exit->pressed(mouse.x,mouse.y)){
            Input::exitmenu=true;
            Input::exitprogram=true;
        }
    }   
}

void Input::reset(){
    mode.pve=false;
    mode.pvp=false;
    mode.eve=false;
    notifyWaitingThreads();
}

menu Input::getMode(){
    return mode;
}

bool Input::paused(){
    return pause;
}