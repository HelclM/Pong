#include"Classes.h"

int main(){
    Input events;
    Player player1,player2;
    Desk deskL(true);
    Desk deskP(false);
    Ball ball(&deskP,&deskL);
    Threads threads;
    vector<Button> buttons;
    buttons.push_back(Button ("Player vs Player",1));
    buttons.push_back(Button ("PC vs Player",2));
    buttons.push_back(Button ("PC vs PC",3));
    buttons.push_back(Button ("Exit",4));
    player1.setBallRef(&ball);
    player1.setDeskRef(&deskL);
    player2.setBallRef(&ball);
    player2.setDeskRef(&deskP);
    Graphic video;
    video.init();
    video.createWinRen("Pong",Graphic::MENU_WINDOW_HEIGHT,Graphic::MENU_WINDOW_WIDTH);
    video.loadResources();
    //program loop
    while(!Input::exitprogram){
        video.drawMenu(buttons);
        while(!Input::exitmenu){
            events.handleMenuEvents(&buttons[0],&buttons[1],&buttons[2],&buttons[3]);
            SDL_Delay(1000/60);
        }
        if(Input::exitprogram){
            video.cleanUp();
            return 0;
        }
        if(events.getMode().pve){
            deskL.setQeueRef(player1.getQueueRef());
            deskP.setQeueRef(events.getqueueRefP());
            threads.createThreadsPvE(&deskL,&deskP,&ball,&player1);
        }
        else if(events.getMode().pvp){
            deskL.setQeueRef(events.getqueueRefL());
            deskP.setQeueRef(events.getqueueRefP());
            threads.createThreadsPvP(&deskL,&deskP,&ball);
        }
        else if(events.getMode().eve){
            deskL.setQeueRef(player1.getQueueRef());
            deskP.setQeueRef(player2.getQueueRef());
            threads.createThreadsEvE(&deskL,&deskP,&ball,&player1,&player2);
        }
        video.resizeWindow(Graphic::GAME_WINDOW_HEIGHT,Graphic::GAME_WINDOW_WIDTH);
        //game loop
        while(!Input::exitgame){
            events.handleGameEvents();
            video.drawGame(&deskP,&deskL,&ball);
            SDL_Delay(1000/60);
        }
        //result of the game
        if(ball.getScoreL()==10 || ball.getScoreP()==10){
            if(ball.getScoreL()>ball.getScoreP()){
                video.drawWinner("left player wins");
            }
            else if(ball.getScoreL()<ball.getScoreP()){
                video.drawWinner("right player wins");
            }
        }
        deskP.reset();
        deskL.reset();
        player1.notifyWaitingThreads();
        player2.notifyWaitingThreads();
        events.reset();
        ball.resetAll();
        threads.joinAll();
        video.resizeWindow(Graphic::MENU_WINDOW_WIDTH,Graphic::MENU_WINDOW_HEIGHT);
        Input::exitgame=false;
        Input::exitmenu=false;
    }
    video.cleanUp();
    return 0;
}