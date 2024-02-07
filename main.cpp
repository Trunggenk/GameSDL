#include<iostream>
#include<SDL.h>

SDL_Window * gWindow = NULL; // tạo cửa soor

SDL_Surface* gScreenSurface = NULL;//tao khu vực trong cửa man hinh
SDL_Surface* gHelloWorld = NULL;// tao noi anh se hien len
int SDL_WIDTH=200;
int SDL_HEIGHT=200;

bool quit = false;
SDL_Event e;


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
    gHelloWorld=SDL_LoadBMP("C:\\Users\\Trung\\CLionProjects\\GameSDL\\hello_world.bmp");
    if(gHelloWorld== NULL)
    {
        printf("khong load dc hinh anh %s" ,SDL_GetError());
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
using namespace std;
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