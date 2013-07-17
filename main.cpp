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


void initUniverse();
void line(float x1, float y1, float x2, float y2);
void drawSpecialLine();
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
SDL_Color couleur;
double zoomFactor = 1.0;
double screenX = 0;
double screenY = 0;
SDL_Surface* screen = NULL;
SDL_Surface *rectangle = NULL;
vector<SelectGravityObject*> universe;
vector<GravityObject*> convertUniverse;
vector<SDL_Surface*> currentDisplay;

void initUniverse(int nb, double xp, double yp, double vxp, double vyp)
{
    for (int i = 0; i < nb; ++i)
    {
        int x = (rand() % screenW/2) - screenW/4 + screenW/2;
        int y = (rand() % screenH/2) - screenH/4 + screenH/2;
        int w = (rand() % 1000) + 1;
        int vx = (rand() %  101) -50;
        int vy = (rand() % 101) -50;
        int v = (rand() % 100) + 1;

        universe.push_back(new SelectGravityObject(w, new Vector(x+xp,y+yp), new Vector(Vector(vx,vy).normalize().multiply(v/100.0)+Vector(vxp,vyp)), new Vector()));
    }
}


int main ( int argc, char** argv )
{
    keyboardPresent = true;
    trainee = false;
    pi = 4 * std::atan(1);
    couleur = {255, 0, 0};
    srand(time(NULL));

    //temp
//    zoomFactor = 10;
//    screenX = 8000;
//    screenY = 4000;



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


//    for (int i = 0; i < 10; ++i)
//    {
//        int x = (rand() % (int)screenX*2) - screenX;
//        int y= (rand() % (int)screenY*2) - screenY;
//        int nb = (rand() % 100) + 1;
//        int vx = (rand() %  101) -50;
//        int vy = (rand() % 101) -50;
//        initUniverse(nb,x,y,vx/50.0, vy/50.0);
//    }


//    universe.push_back(new SelectGravityObject(1000000, new Vector(0+screenW/2,0+screenH/2), new Vector(0,0), new Vector()));

    for (int i=-49;i<50;i++)
        universe.push_back(new SelectGravityObject(100, new Vector(0,0+screenH/2-10*i), new Vector(1,0), new Vector()));

//    for (int i=-49;i<50;i++)
//        universe.push_back(new SelectGravityObject(rand() % 100, new Vector(0,0+screenH/2-10*i), new Vector((rand() % 100)/100.0,0), new Vector()));
//    universe.push_back(new SelectGravityObject(10000, new Vector(screenW/2,0+screenH/2-100), new Vector(1,0), new Vector()));
//    universe.push_back(new SelectGravityObject(10000, new Vector(screenW/2,0+screenH/2+100), new Vector(-1,0), new Vector()));
//    universe.push_back(new SelectGravityObject(100, new Vector(screenW/2+100,0+screenH/2), new Vector(0,1), new Vector()));
//    universe.push_back(new SelectGravityObject(100, new Vector(screenW/2-100,0+screenH/2), new Vector(0,-1), new Vector()));
//    universe.push_back(new SelectGravityObject(1000, new Vector(screenW,0+screenH/2+30), new Vector(-1,0), new Vector()));

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
                    screenY+=zoomFactor*10;
                    break;
                case SDLK_DOWN:
                    screenY-=zoomFactor*10;
                    break;
                case SDLK_LEFT:
                    screenX-=zoomFactor*10;
                    break;
                case SDLK_RIGHT:
                    screenX+=zoomFactor*10;
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
                        if ((*gravObjPtr)->isInZone(event.button.x*zoomFactor-screenX, event.button.y*zoomFactor-screenY))
                        {
                            if(!isAnyObjSelected) isAnyObjSelected = true;
                            lineCenter = *gravObjPtr;
                        }
                        if(lineCenter!=0)
                        {
                            if (!isAnyObjSelected)
                            {
                                lineCenter->addSpeed((Vector(event.button.x*zoomFactor-screenX, event.button.y*zoomFactor-screenY)-*(lineCenter)->getPosition()).normalize().multiply(Vector(event.button.x*zoomFactor-screenX, event.button.y*zoomFactor-screenY).distanceTo(*(lineCenter)->getPosition())/sqrt(screenH*screenH+screenW*screenW)));
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
                else if (event.button.button == SDL_BUTTON_WHEELUP)
                {
                    screenX = (event.button.x*zoomFactor-screenX)/2;
                    screenY = (event.button.y*zoomFactor-screenY)/2;
                    zoomFactor = max(1.0,zoomFactor-1);
                }
                else if (event.button.button == SDL_BUTTON_WHEELDOWN)
                {
                    screenX = (event.button.x*zoomFactor-screenX)/2;
                    screenY = (event.button.y*zoomFactor-screenY)/2;
                    zoomFactor++;
                }
                break;
            }
            case SDL_MOUSEMOTION:
            {
                linePosition.x = event.motion.x*zoomFactor-screenX;
                linePosition.y = event.motion.y*zoomFactor-screenY;
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

    filledCircleRGBA(screen, (x+screenX)/zoomFactor, (y+screenY)/zoomFactor, sqrt(w/2)/zoomFactor,50,50,50,128);

    if (gravObj->isSelected())
    {
//        circleRGBA(screen, x, y, sqrt(w/2),255*gravObj->getWeight()/1000.0,255*(1-gravObj->getWeight()/1000.0),(cos(SDL_GetTicks()/10000.0*2*pi)+1)/2*255,128);
        circleRGBA(screen, (x+screenX)/zoomFactor, (y+screenY)/zoomFactor, sqrt(w/2)/zoomFactor,(cos(SDL_GetTicks()/5000.0*2*pi)+1)/2*255*gravObj->getWeight()/1000.0,(sin(SDL_GetTicks()/(7000.0*(1+gravObj->getWeight()/1000.0))*2*pi)+1)/2*255,(cos(SDL_GetTicks()/11000.0*2*pi)+1)/2*255,(cos(SDL_GetTicks()/17000.0*2*pi)+1)/2*255);
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

    SDL_Rect pos;
    pos.x = 0;
    pos.y = 0;
    printText("Zoom=", zoomFactor, pos, couleur, police);

    pos.y+=30;
    printText("ScreenX=", screenX, pos, couleur, police);

    pos.y+=30;
    printText("ScreenY=", screenY, pos, couleur, police);

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
    police = TTF_OpenFont("SourceCodePro-Regular.ttf", 30);

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


        drawSpecialLine();

        Vector* t = new Vector(linePosition.x,linePosition.y);
        SDL_Rect linePosition2;
        linePosition2.x = (linePosition.x+screenX)/zoomFactor;
        linePosition2.y = (linePosition.y+screenY)/zoomFactor;
        std::ostringstream strs;
        strs << lineCenter->getPosition()->distanceTo(*t);
        std::string str = strs.str();
        SDL_Surface* texte = TTF_RenderText_Blended(police, str.c_str(), couleur);
        SDL_Surface* texte2 = rotozoomSurface(texte, 180/pi*atan(-((linePosition.y-lineCenter->getPosition()->getY())/(linePosition.x-lineCenter->getPosition()->getX()))), 1.0, 1); //On transforme la surface image.
        SDL_BlitSurface(texte2, NULL, screen, &linePosition2); /* Blit du texte */
        SDL_FreeSurface(texte);
        SDL_FreeSurface(texte2);
        delete t;

        SDL_Rect pos;
        pos.x = lineCenter->getPosition()->getX()+lineCenter->getSelectRadius();
        pos.y = lineCenter->getPosition()->getY()-lineCenter->getSelectRadius();
        pos.x = (pos.x+screenX)/zoomFactor;
        pos.y = (pos.y+screenY)/zoomFactor;
        printText("w=",lineCenter->getWeight(), pos, couleur, police);

        pos.y +=30;
        printText("s=",lineCenter->getSpeed()->getNorm(), pos, couleur, police);

    }
}

void drawSpecialLine()
{
//    lineRGBA(screen, lineCenter->getPosition()->getX(), lineCenter->getPosition()->getY(), linePosition.x, linePosition.y, 255,0,0,255);

//    int i1 = max(lineCenter->getPosition()->getX(), (double)linePosition.x);
//    int i2 = min(lineCenter->getPosition()->getX(), (double)linePosition.x);
//
//    int j1 = max(lineCenter->getPosition()->getY(), (double)linePosition.y);
//    int j2 = min(lineCenter->getPosition()->getY(), (double)linePosition.y);
//
//    int k1 = max(i1-i2,j1-j2);
//
//    for(int k = 0; k<j1; ++k)
//    {
//        int i = i2+(i1-i2)*k/k1;
//        int j = j2+(j1-j2)*k/k1;
//        pixelRGBA(screen, i, j, 0, 255, 255, 128);
//    }



    Vector* v = new Vector(-(linePosition.y-lineCenter->getPosition()->getY()), linePosition.x-lineCenter->getPosition()->getX());
    double distance = v->getNorm()/20.0;
    v->normalize();
    Vector* vp = new Vector(*v);
    Vector* v2 = new Vector(linePosition.x-lineCenter->getPosition()->getX(), linePosition.y-lineCenter->getPosition()->getY());
    v2->normalize();
    *v-=*v2;
    vp->multiply(-1);
    *vp-=*v2;
    v->multiply(distance/20);
    vp->multiply(distance/20);

    for(int k = 0; k <60;++k)
    {
        line(lineCenter->getPosition()->getX(), lineCenter->getPosition()->getY(), linePosition.x+v->getX()*k/6.0, linePosition.y+v->getY()*k/6.0);
        line(lineCenter->getPosition()->getX(), lineCenter->getPosition()->getY(), linePosition.x+vp->getX()*k/6.0, linePosition.y+vp->getY()*k/6.0);
    }

    delete v;
    delete v2;
    delete vp;
}

void line(float x1, float y1, float x2, float y2)
{
    //Utilisation du zoom
    x1 = (x1+screenX)/zoomFactor;
    x2 = (x2+screenX)/zoomFactor;
    y1 = (y1+screenY)/zoomFactor;
    y2 = (y2+screenY)/zoomFactor;
    // Bresenham's line algorithm
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    bool reversePercent = (x2-x1)+(y2-y1)>0;
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;

    const int maxX = (int)x2;


    for(int x=(int)x1; x<maxX; x++)
    {

        double percent;
        if (maxX != x1)
        {
            percent = (x-x1)/(maxX - x1);
        }
        else if (y2 != y1)
        {
            percent = (y-y1)/(y2-y1);
        }
        else
        {
            percent = 1;
        }

        if (reversePercent) percent = 1-percent;

//        SDL_Color couleur = {255, 0, 0};
//        SDL_Rect pos;
//        pos.x = lineCenter->getPosition()->getX()+lineCenter->getSelectRadius();
//        pos.y = lineCenter->getPosition()->getY()-lineCenter->getSelectRadius()+90;
//        printText("",(maxX-x1), pos, couleur, police);
//        pos.y+=30;
//        printText("",(y2-y1), pos, couleur, police);
//        pos.y+=30;
//        printText("",(maxX-x1)-(y2-y1), pos, couleur, police);


        if(steep)
        {
             pixelRGBA(screen, y, x, 255, percent*255, 0, 128);
        }
        else
        {
             pixelRGBA(screen, x, y, 255, percent*255, 0, 128);
        }

        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}

void printText(const string &prefix, const double &text, SDL_Rect &position, SDL_Color couleur, TTF_Font* police)
{
    std::ostringstream strs;
    strs << prefix << text;
    std::string str = strs.str();
    SDL_Surface* texte = TTF_RenderText_Blended(police, str.c_str(), couleur);
    SDL_BlitSurface(texte, NULL, screen, &position); /* Blit du texte */
    SDL_FreeSurface(texte);
}
