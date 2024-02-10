#include<iostream>
#include<SDL.h>
#include<string>
using namespace std;


const int SDL_WIDTH = 640;
const int SDL_HEIGHT = 480;

//Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

//Current displayed image
SDL_Surface* gCurrentSurface = NULL;

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

    //Deallocate surfaces
    for( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i )
    {
        SDL_FreeSurface( gKeyPressSurfaces[ i ] );
        gKeyPressSurfaces[ i ] = NULL;
    }

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
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
                                break;
                            default:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                                break;


                        }
                    }
                }

                //Apply the image
                SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );

                //Update the surface
                SDL_UpdateWindowSurface( gWindow );
            }

        }
    }
    close();
    return 0;
}