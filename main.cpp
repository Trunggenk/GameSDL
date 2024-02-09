#include<iostream>
#include<SDL.h>
#include<string>
using namespace std;


enum {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};
SDL_Surface* loadSurface(string path);// load tung anh
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];//anh anh tuong ung khi nhan
SDL_Window * gWindow = NULL; // tạo cửa soor
SDL_Surface* gScreenSurface = NULL;//tao khu vực trong cửa man hinh
SDL_Surface* gHelloWorld = NULL;// tao noi anh se hien len
SDL_Surface* gCurrentSurface= NULL; // anh hien tai hien thi
int SDL_WIDTH=500;

int SDL_HEIGHT=600;

bool init()
{
    bool success = true;

    //khoi tao SDL
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        printf("SDL chua duoc khoi tao");
        success = 0;

    }
    else
    {
        gWindow = SDL_CreateWindow("SDL tieu de",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SDL_WIDTH, SDL_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow== NULL)
        {
            printf("window chua duoc tao");
            success=0;
        } else
        {
            // lay be mat chua boi cua so
            gScreenSurface= SDL_GetWindowSurface(gWindow);
        }
    }
    return  success;

}

bool loadMedia()
{
    bool success= 1;
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT]= loadSurface("press.bmp");
    if( gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT]==NULL)
    {
        printf("failed to load image");
        success= false;
    }
    //hay viet ham kiem tra cho key press surface up, down, left, right
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP]= loadSurface("up.bmp");
    if( gKeyPressSurfaces[KEY_PRESS_SURFACE_UP]==NULL)
    {
        printf("failed to load image");
        success= false;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN]= loadSurface("down.bmp");
    if( gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN]==NULL)
    {
        printf("failed to load image");
        success= false;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT]= loadSurface("left.bmp");
    if( gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT]==NULL)
    {
        printf("failed to load image");
        success= false;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT]= loadSurface("right.bmp");
    if( gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT]==NULL)
    {
        printf("failed to load image");
        success= false;
    }
    return success;
}

void close()
{

    SDL_FreeSurface(gHelloWorld);//giai phong bo nhớ cho SDL_sufface
    gHelloWorld=NULL;
    SDL_DestroyWindow(gWindow);
    gWindow=NULL;
    SDL_Quit();

}
SDL_Surface* loadSurface(string path)// load tung anh
{
    SDL_Surface * loadedSurface = SDL_LoadBMP( path.c_str());
    if (loadedSurface == NULL)
    {
        printf("load image error %s", SDL_GetError());
    }

    return loadedSurface;

}
int main(int argc, char *args[])
{
    if(!init())
    {
        printf("khogn the toa");

    }
    else {
        //Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            //Apply the image
            SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
            SDL_UpdateWindowSurface(gWindow);
            SDL_Event e;
            bool quit = false;
            gCurrentSurface= gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
            while( !quit )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                         quit = true;
                    }
                    else if(e.type==SDL_KEYDOWN)
                    {
                        switch (e.key.keysym.sym)
                        {
                            case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                                break;

                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                                break;

                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                                break;
                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                            default:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                                break;


                        }
                    }
                }

                //Apply the image
                SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );

                //Update the surface
                SDL_UpdateWindowSurface( gWindow );
            }

        }
    }
    close();
    return 0;
}