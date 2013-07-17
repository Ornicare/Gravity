#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <vector>
#include "SelectGravityObject.h"
#include "Vector.h"
#include <SDL/SDL_gfxPrimitives.h>
#include <cmath>
#include <sstream>
#include <SDL/SDL_rotozoom.h>

using namespace std;

void printText(const string &prefix, const double &text, SDL_Rect &position, SDL_Color couleur, TTF_Font* police);
void processUniverse();
void pause();
void draw(GravityObject* gravObj);
void drawLine();
int launchSDL();
int screenW = 1920;
int screenH = 1080;
int tempsPrecedent = 0, tempsActuel = 0;
bool trainee;
bool mousePresent;
bool keyboardPresent;
SDL_Rect linePosition;
SelectGravityObject* lineCenter;
double pi;
TTF_Font *police = NULL;

SDL_Surface* screen = NULL;
SDL_Surface *rectangle = NULL;
vector<SelectGravityObject*> universe;
vector<GravityObject*> convertUniverse;
vector<SDL_Surface*> currentDisplay;

int main ( int argc, char** argv )
{
    keyboardPresent = true;
    trainee = false;
    pi = 4 * std::atan(1);
    police = TTF_OpenFont("angelina.TTF", 65);
//    GravityObject test(100);
//    GravityObject* test2 = new GravityObject;
//    GravityObject test3(test);
//    test.setWeight(42);
//    cout << test.getWeight() << endl;
//    cout << test2->getWeight() << endl;
//    cout << test3.getWeight() << endl;
//
//    Vector a(1,0);
//    Vector b(0,1);
//    Vector c = a+b;
//    a+=c;
//    cout << a << endl;
//    cout << b << endl;
//    cout << c << endl;
//
//    Vector* d = new Vector();
//    cout << (*d).getX() << endl;

    srand( time( NULL ) );

    for (int i = 0; i < 100; ++i)
    {
        int x = (rand() % screenW/2) - screenW/4 + screenW/2;
        int y = (rand() % screenH/2) - screenH/4 + screenH/2;
        int w = (rand() % 1000) + 1;
        int vx = (rand() %  101) -50;
        int vy = (rand() % 101) -50;
        int v = (rand() % 100) + 1;

        universe.push_back(new SelectGravityObject(w, new Vector(x,y), new Vector(Vector(vx,vy).normalize().multiply(v/100.0)), new Vector()));
    }
//    universe.push_back(new SelectGravityObject(10000, new Vector(0,0+screenH/2-330), new Vector(0.8,0), new Vector()));
//    universe.push_back(new SelectGravityObject(1000, new Vector(0,0+screenH/2-30), new Vector(1,0), new Vector()));
//    universe.push_back(new SelectGravityObject(100, new Vector(0,0+screenH/2+30), new Vector(1.2,0), new Vector()));

    vector<SelectGravityObject*>::const_iterator gravObjPtr;
    for (gravObjPtr = universe.begin(); gravObjPtr != universe.end(); ++gravObjPtr)
    {
        (*gravObjPtr)->setSelected(true);
        convertUniverse.push_back(dynamic_cast<GravityObject*> (*gravObjPtr));
    }


//    universe.push_back(new GravityObject(10000, new Vector(0+screenW/2,0+screenH/2), new Vector(), new Vector()));
//    universe.push_back(new GravityObject(100, new Vector(200+screenW/2,0+screenH/2), new Vector(0,0.9), new Vector()));
//    universe.push_back(new GravityObject(100, new Vector(-200+screenW/2,0+screenH/2), new Vector(0,-0.9), new Vector()));
//    universe.push_back(new GravityObject(100, new Vector(0+screenW/2,-200+screenH/2), new Vector(0.9,0), new Vector()));
//    universe.push_back(new GravityObject(100, new Vector(0+screenW/2,200+screenH/2), new Vector(-0.9,0), new Vector()));
//    universe.push_back(new GravityObject(1000, new Vector(500+screenW/2,0+screenH/2), new Vector(-1,0), new Vector()));

//    universe.push_back(new GravityObject(10, new Vector(100,1), new Vector(), new Vector()));
//    universe.push_back(new GravityObject(11, new Vector(0,100), new Vector(), new Vector()));
//    universe.push_back(new GravityObject(12, new Vector(100,100), new Vector(), new Vector()));

    launchSDL();


    SDL_FreeSurface(screen);

    return 0;
}

void pause()
{
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = true;
                break;

            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                case SDLK_RETURN:
                    done = true;
                    break;
                case SDLK_a:
                    if(lineCenter!=0)
                    {
                        lineCenter->setWeight(lineCenter->getWeight()+100);
                    }
                    break;
                case SDLK_z:
                    if(lineCenter!=0)
                    {
                        lineCenter->setWeight(max(lineCenter->getWeight()-100,0));
                    }
                    break;
                case SDLK_UP:
                    if(lineCenter!=0)
                    {
                        lineCenter->addSpeed(Vector(0,-0.1));
                    }
                    break;
                case SDLK_DOWN:
                    if(lineCenter!=0)
                    {
                        lineCenter->addSpeed(Vector(0,0.1));
                    }
                    break;
                case SDLK_LEFT:
                    if(lineCenter!=0)
                    {
                        lineCenter->addSpeed(Vector(-0.1,0));
                    }
                    break;
                case SDLK_RIGHT:
                    if(lineCenter!=0)
                    {
                        lineCenter->addSpeed(Vector(0.1,0));
                    }
                    break;
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
//                    vector<SelectGravityObject*> selectedObjects = getSelectedObjects();
                    bool isAnyObjSelected = false;
                    vector<SelectGravityObject*>::const_iterator gravObjPtr;
                    for (gravObjPtr = universe.begin(); gravObjPtr != universe.end(); ++gravObjPtr)
                    {
                        (*gravObjPtr)->setSelected(false);
                        if ((*gravObjPtr)->isInZone(event.button.x, event.button.y))
                        {
                            if(!isAnyObjSelected) isAnyObjSelected = true;
                            lineCenter = *gravObjPtr;
                        }
                        if(lineCenter!=0)
                        {
                            if (!isAnyObjSelected)
                            {
                                lineCenter->addSpeed((Vector(event.button.x, event.button.y)-*(lineCenter)->getPosition()).normalize().multiply(Vector(event.button.x, event.button.y).distanceTo(*(lineCenter)->getPosition())/sqrt(screenH*screenH+screenW*screenW)));
                            }
                        }


                    }
                }
                else if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    vector<SelectGravityObject*>::const_iterator gravObjPtr;
                    for (gravObjPtr = universe.begin(); gravObjPtr != universe.end(); ++gravObjPtr)
                    {
                        (*gravObjPtr)->setSelected(false);
                    }
                    lineCenter = 0;
                }
                else if (event.button.button == SDL_BUTTON_MIDDLE)
                {
                    trainee = !trainee;
                }
                break;
            }
            case SDL_MOUSEMOTION:
            {
                linePosition.x = event.motion.x;
                linePosition.y = event.motion.y;
                break;
            }
            }


            if ((event.active.state & SDL_APPMOUSEFOCUS) == SDL_APPMOUSEFOCUS)
            {
                if (event.active.gain == 0)
                {
                    mousePresent = false;
                }
                else if (event.active.gain == 1)
                {
                    mousePresent = true;
                }
            }

            if ((event.active.state & SDL_APPINPUTFOCUS) == SDL_APPINPUTFOCUS)
            {
                if (event.active.gain == 0)
                {
                    keyboardPresent = false;
                }
                else if (event.active.gain == 1)
                {
                    keyboardPresent = true;
                }
            }
        }

//        cout << mousePresent << keyboardPresent << (keyboardPresent | mousePresent) << endl;

        if(lineCenter!=0)
        {
            lineCenter->setSelected(true);
        }


        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > 15) /* Si 30 ms se sont écoulées */
        {
            if(!trainee) SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

            processUniverse();

            drawLine();

            SDL_Flip(screen);
            tempsPrecedent = tempsActuel; /* Le temps "actuel" devient le temps "precedent" pour nos futurs calculs */
        }



    }
}

void draw(SelectGravityObject* gravObj)
{
//    Uint32 color = SDL_MapRGB(screen->format, 255*gravObj->getWeight()/1000.0, 255*(1-gravObj->getWeight()/1000.0) , 0);
//    Uint32 color = SDL_MapRGB(screen->format,255,255 , 0);

    double x = gravObj->getPosition()->getX();
    double y = gravObj->getPosition()->getY();
    double w = gravObj->getWeight();

//    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, w, w, 32, 0, 0, 0, 0);
//    SDL_FillRect(rectangle, NULL, color);
//
//    SDL_Rect position;
//
//    position.x = x-w/2+screenW/2;
//    position.y = y-w/2+screenH/2;
//
//    SDL_BlitSurface(rectangle, NULL, screen, &position);

//    lineColor(screen, 0, 0, 1000, 1000, color);

    filledCircleRGBA(screen, x, y, sqrt(w/2),50,50,50,128);

    if (gravObj->isSelected())
    {
//        circleRGBA(screen, x, y, sqrt(w/2),255*gravObj->getWeight()/1000.0,255*(1-gravObj->getWeight()/1000.0),(cos(SDL_GetTicks()/10000.0*2*pi)+1)/2*255,128);
        circleRGBA(screen, x, y, sqrt(w/2),(cos(SDL_GetTicks()/5000.0*2*pi)+1)/2*255*gravObj->getWeight()/1000.0,(sin(SDL_GetTicks()/(7000.0*(1+gravObj->getWeight()/1000.0))*2*pi)+1)/2*255,(cos(SDL_GetTicks()/11000.0*2*pi)+1)/2*255,(cos(SDL_GetTicks()/17000.0*2*pi)+1)/2*255);
    }
    //SDL_BlitSurface(line, NULL, screen, &position);
//    cout << x << endl;

}

void processUniverse()
{
    vector<SelectGravityObject*>::const_iterator gravObjPtr;
    for (gravObjPtr = universe.begin(); gravObjPtr != universe.end(); ++gravObjPtr)
    {
        (*gravObjPtr)->calculateGravityAcceleration(convertUniverse);
    }

    for (gravObjPtr = universe.begin(); gravObjPtr != universe.end(); ++gravObjPtr)
    {
//        Vector* v = (*gravObjPtr)->getPosition();
//        cout << *v << endl;
        (*gravObjPtr)->calculateSpeed();
        (*gravObjPtr)->calculatePosition();
        draw(*gravObjPtr);
    }
}

int launchSDL()
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    TTF_Init();


    // make sure SDL cleans up before exit
    atexit(SDL_Quit);
    //Set an icon
    SDL_WM_SetIcon(SDL_LoadBMP("sdl_icone.bmp"), NULL);


    //Double buffer : éviter les scintillements.

    // create a new window
    screen = SDL_SetVideoMode(screenW, screenH, 16,
                              SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE);


    SDL_WM_SetCaption("Gravity", "Gravity Simulation");

    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return 1;
    }


    //active l'envoi d'un keydown en cas de maintient
    SDL_EnableKeyRepeat(10, 10);

    pause();

    // free loaded objects
    SDL_FreeSurface(rectangle);

    TTF_Quit();
}

void drawLine()
{
    if(lineCenter!=0)
    {
        lineRGBA(screen, lineCenter->getPosition()->getX(), lineCenter->getPosition()->getY(), linePosition.x, linePosition.y, 255,0,0,255);

        SDL_Color couleur = {255, 0, 0};

        police = TTF_OpenFont("SourceCodePro-Regular.ttf", 30);

        Vector* t = new Vector(linePosition.x,linePosition.y);
        std::ostringstream strs;
        strs << lineCenter->getPosition()->distanceTo(*t);
        std::string str = strs.str();
        SDL_Surface* texte = TTF_RenderText_Blended(police, str.c_str(), couleur);
        texte = rotozoomSurface(texte, 180/pi*atan(-((linePosition.y-lineCenter->getPosition()->getY())/(linePosition.x-lineCenter->getPosition()->getX()))), 1.0, 1); //On transforme la surface image.
        SDL_BlitSurface(texte, NULL, screen, &linePosition); /* Blit du texte */
        delete t;

        SDL_Rect pos;
        pos.x = lineCenter->getPosition()->getX()+lineCenter->getSelectRadius();
        pos.y = lineCenter->getPosition()->getY()-lineCenter->getSelectRadius();
        printText("w=",lineCenter->getWeight(), pos, couleur, police);

        pos.y +=30;
        printText("s=",lineCenter->getSpeed()->getNorm(), pos, couleur, police);

    }
}

void printText(const string &prefix, const double &text, SDL_Rect &position, SDL_Color couleur, TTF_Font* police)
{
    std::ostringstream strs;
    strs << prefix << text;
    std::string str = strs.str();
    SDL_Surface* texte = TTF_RenderText_Blended(police, str.c_str(), couleur);
    SDL_BlitSurface(texte, NULL, screen, &position); /* Blit du texte */

}
