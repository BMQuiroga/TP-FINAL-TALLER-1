#include "intention.h"
#include "moving_intention.h"
#include "shooting_intention.h"
#include "stop_moving_intention.h"
#include "reload_intention.h"
#include "prepare_grenade_intention.h"
#include "throw_grenade_intention.h"
#include "idle_intention.h"

Intention *Intention::make_intention(SDL_Event &event)
{
    SDL_KeyboardEvent& keyEvent = (SDL_KeyboardEvent&) event;
    switch (keyEvent.keysym.sym) {
        case SDLK_LEFT: {
            if (event.type == SDL_KEYDOWN) {
                return new IntentionMove(-1, 0, MOVE_LEFT);    
            } 
            return new IntentionStopMoving(-1, 0, STOP_MOVING_LEFT);
        }
        case SDLK_RIGHT: {
            if (event.type == SDL_KEYDOWN) {
                return new IntentionMove(1, 0, MOVE_RIGHT);
            }
            return new IntentionStopMoving(1, 0, STOP_MOVING_RIGHT);
        }
        case SDLK_DOWN: {
            if (event.type == SDL_KEYDOWN) {
                return new IntentionMove(0, -1, MOVE_DOWN);
            }
            return new IntentionStopMoving(0, -1, STOP_MOVING_DOWN);
        }
        case SDLK_UP: {
            if (event.type == SDL_KEYDOWN) {
                return new IntentionMove(0, 1, MOVE_UP);
            }
            return new IntentionStopMoving(0, 1, STOP_MOVING_UP);
        } 
        case SDLK_s: {
            if (event.type == SDL_KEYDOWN) {
                return new IntentionShooting(true, SHOOT);
            }
            return new IntentionShooting(false, STOP_SHOOTING);
        }
        case SDLK_r: {
            return new IntentionReload(RELOAD);
        }
        case SDLK_g: {
            if (event.type == SDL_KEYDOWN) {
                return new IntentionPrepareGrenade(PREPARE_GRENADE);
            }
            return new IntentionThrowGrenade(THROW_GRENADE);
        }
    }
    if (event.type == SDL_QUIT) {
        return new IntentionIdle(END);
    }
    return nullptr;
}
