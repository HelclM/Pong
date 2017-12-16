#include <algorithm>

#include "Classes.h"

void Threads::createThreadsEvE(Desk* deskL, Desk* deskP, Ball* ball, Player* player1, Player* player2){
    runningThreads.push_back(thread (&Player::logic,player1));
    runningThreads.push_back(thread (&Player::logic,player2));
    runningThreads.push_back(thread (&Desk::logic,deskP));
    runningThreads.push_back(thread (&Desk::logic,deskL));
    runningThreads.push_back(thread (&Ball::logic,ball));
}

void Threads::createThreadsPvE(Desk* deskL, Desk* deskP, Ball* ball, Player* player1){
    runningThreads.push_back(thread (&Player::logic,player1));
    runningThreads.push_back(thread (&Desk::logic,deskP));
    runningThreads.push_back(thread (&Desk::logic,deskL));
    runningThreads.push_back(thread (&Ball::logic,ball));
}

void Threads::createThreadsPvP(Desk* deskL, Desk* deskP, Ball* ball){
    runningThreads.push_back(thread (&Desk::logic,deskP));
    runningThreads.push_back(thread (&Desk::logic,deskL));
    runningThreads.push_back(thread (&Ball::logic,ball));
}

void Threads::joinAll(){
    for (int i = 0; i < (int)runningThreads.size(); i++) {
        runningThreads[i].join();
    }
    while(!runningThreads.empty()){
        runningThreads.pop_back();
    }
}