#include"Classes.h"

int Graphic::GAME_WINDOW_HEIGHT=600;
int Graphic::GAME_WINDOW_WIDTH=600;

int Graphic::MENU_WINDOW_HEIGHT=600;
int Graphic::MENU_WINDOW_WIDTH=400;

void Graphic::init(){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
}
 
void Graphic::createWinRen(string name, int h, int w){
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,w,h, SDL_WINDOW_OPENGL);
    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!rend){
        cout<<"error creating renderer: "<<SDL_GetError()<<endl;
    }
}

SDL_Texture* Graphic::loadImage(string path){
    SDL_Texture* tex=NULL;
    SDL_Surface* sur=IMG_Load(path.c_str() );
    if(sur==NULL){
        cout<<"error surface("<<path<<")="<<IMG_GetError()<<endl;
    }
    else{
        tex=SDL_CreateTextureFromSurface(rend,sur);
        if (tex==NULL){
            cout<<"error tex("<<path<<"):"<<SDL_GetError()<<endl;
        }       
    }
    SDL_FreeSurface(sur);
    return tex;
}

void Graphic::loadResources(){
    backtex=loadImage("./Images/back.png");
    balltex=fillRect({10,10,10,10},{255,255,255,255});
    desktex=fillRect({10,10,10,10},{255,255,255,255});
    font=TTF_OpenFont("./Fonts/OpenSans-BoldItalic.ttf",500);
    if(font==NULL){
        cout<<"font error"<<TTF_GetError()<<endl;
    }
    textColor={255,255,255,255};
    scoreLdest.h=70;
    scoreLdest.w=30;
    scoreLdest.x=Graphic::GAME_WINDOW_WIDTH/4-scoreLdest.w/2;
    scoreLdest.y=0;
    scorePdest.h=70;
    scorePdest.w=30;
    scorePdest.x=3*Graphic::GAME_WINDOW_WIDTH/4-scorePdest.w/2;
    scorePdest.y=0;
}

void Graphic::drawPic(SDL_Rect destpic, SDL_Texture* tex){
    SDL_Rect dest;
    if(SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h)!=0){
        cout<<"query error: "<<SDL_GetError()<<endl;
    }   
    dest.x=destpic.x;
    dest.y=destpic.y;
    dest.w=destpic.w;
    dest.h=destpic.h;
    if(SDL_RenderCopy(rend, tex, NULL,&dest)!=0){
        cout<<"render copy error: "<<SDL_GetError()<<endl;
    }
}

SDL_Texture* Graphic::RenderText(string text, SDL_Color textcolor={0,0,0,0}){
    SDL_Texture* tex;
    SDL_Surface* textSurface=TTF_RenderText_Solid(font,text.c_str(),textcolor);
    if(textSurface==NULL){
        cout<<"text Surface error "<<TTF_GetError()<<endl;
    }
    tex=SDL_CreateTextureFromSurface(rend,textSurface);
    if(tex==NULL){
        cout<<"text Texture error "<<TTF_GetError()<<endl;
    }
    SDL_FreeSurface(textSurface);
    return tex;
}

void Graphic::cleanUp(){
    SDL_DestroyTexture(backtex);
    SDL_DestroyTexture(balltex);
    SDL_DestroyTexture(desktex);
    SDL_DestroyTexture(scoreLtex);
    SDL_DestroyTexture(scorePtex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
void Graphic::drawGame(Desk* deskP,Desk* deskL,Ball* ball){
    SDL_RenderCopy(rend,backtex,NULL,NULL);
    drawPic(deskP->getRect(),desktex);
    drawPic(deskL->getRect(),desktex);
    drawPic(ball->getRect(),balltex);
    if(scoreL!=ball->getScoreL()){
        scoreL=ball->getScoreL();
        stringstream ss;
        ss<<scoreL;
        if(scoreLtex!=NULL){
            SDL_DestroyTexture(scoreLtex);
        }
        scoreLtex=RenderText(ss.str(),textColor);
        resizeTextdest(&scoreLdest,ss.str(),35,70);
    }
    if(scoreP!=ball->getScoreP()){
        scoreP=ball->getScoreP();
        stringstream ss;
        ss<<scoreP;
        if(scorePtex!=NULL){
            SDL_DestroyTexture(scorePtex);
        }
        scorePtex=RenderText(ss.str(),textColor);
        resizeTextdest(&scorePdest,ss.str(),35,70);
    }
    drawPic(scorePdest,scorePtex);
    drawPic(scoreLdest,scoreLtex);
    if(Input::paused()){
        drawAnnouncement("II",100,300);
    }
    SDL_RenderPresent(rend);
}

void Graphic::drawButton(Button* button){
    SDL_Rect buttdest=button->getRect();
    SDL_Rect textdest;
    resizeTextdest(&textdest,button->getName(),15,40);
    textdest.x=buttdest.x+buttdest.w/2-textdest.w/2;
    textdest.y=buttdest.y+buttdest.h/2-textdest.h/2;
    SDL_Texture* texButt;
    SDL_Texture* texText;
    texButt=fillRect(buttdest,{0,0,0,0});
    texText=RenderText(button->getName(),{255,255,255,255});
    drawPic(buttdest,texButt);
    drawPic(textdest,texText);
    SDL_DestroyTexture(texText);
    SDL_DestroyTexture(texButt);
}

void Graphic::drawMenu(vector<Button>& buttons){
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderClear(rend);
    for (int i = 0; i < (int)buttons.size(); i++) {
        drawButton(&buttons[i]);
    }
    SDL_RenderPresent(rend);
}

void Graphic::drawAnnouncement(string text,int letterW ,int letterH){
    SDL_Texture* winnertex;
    SDL_Rect dest;
    winnertex=RenderText(text,{255,0,0,255});
    resizeTextdest(&dest,text,letterW,letterH);
    dest.y=Graphic::GAME_WINDOW_HEIGHT/2-dest.h/2;
    dest.x=Graphic::GAME_WINDOW_WIDTH/2-dest.w/2;
    drawPic(dest,winnertex);
    SDL_DestroyTexture(winnertex);
}

SDL_Texture* Graphic::fillRect(SDL_Rect dest, SDL_Color color){
    SDL_Surface* s;
    SDL_Texture* tex;
    s = SDL_CreateRGBSurface(0,dest.w,dest.h, 32, 0, 0, 0,0);
    if(s==NULL){
        cout<<"error creating RGB surface: "<<SDL_GetError()<<endl;
    }
    else{
        SDL_FillRect(s, NULL, SDL_MapRGB(s->format, color.r,color.g,color.b));
        tex=SDL_CreateTextureFromSurface(rend,s);
        if (tex==NULL){
            cout<<"error creating texture from filled rectangle: "<<SDL_GetError()<<endl;
        }
    }
    return tex;
}

void Graphic::resizeWindow(int w, int h){
    SDL_SetWindowSize(window,w,h);
}

void Graphic::resizeTextdest(SDL_Rect* dest, string text,int letterW,int letterH){
    dest->h=letterH;
    dest->w=letterW*text.length();
}

void Graphic::drawWinner(string winner){
    drawAnnouncement(winner,30,70);
    SDL_RenderPresent(rend);
    SDL_Delay(1000);
}