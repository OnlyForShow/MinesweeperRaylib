#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include <raylib.h>



#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 1100
#define WINDOW_NAME "Minesweeper"

#define WIDTH_FIELD 50
#define HEIGHT_FIELD 50


typedef char BOOL;

#define TRUE 1
#define FALSE 0

typedef struct zoom_level
{
    double zoom_x1; 
    double zoom_x2; 
    double zoom_y1; 
    double zoom_y2; 
} zoom_level;

zoom_level global_zoom;
zoom_level global_zoom_ref;

typedef struct Point2D
{
    double x,y;
} Point2D;


Point2D worldCoord(int x, int y, zoom_level * zoom)
{
    return (Point2D){zoom->zoom_x1 + (zoom->zoom_x2 - zoom->zoom_x1) * ((double)(x)/(double)(SCREEN_WIDTH)),
         zoom->zoom_y1 + (zoom->zoom_y2 - zoom->zoom_y1) * ((double)(y)/(double)(SCREEN_HEIGHT))};
}

Point2D inv_worldCoord(double x, double y, zoom_level * zoom)
{
    return (Point2D){(x - zoom->zoom_x1)/(zoom->zoom_x2 - zoom->zoom_x1) * (double)(SCREEN_WIDTH),
        (y - zoom->zoom_y1)/(zoom->zoom_y2 - zoom->zoom_y1) * (double)(SCREEN_HEIGHT)};
}


void init_zoom(zoom_level * zoom, double width, double height)
{
    zoom->zoom_x1 = 0.0;
    zoom->zoom_x2 = width;
    zoom->zoom_y1 = 0;
    zoom->zoom_y2 = height;

    
}

enum MINE_STRATEGY {UNIFORM, CIRCULAR};

Font NumberFont;
Texture2D texMine;
Texture2D texFlag;
Texture2D texHidden;
Texture2D texExposed;



void loadSound()
{
    
}

void loadTexture()
{
    NumberFont = LoadFontEx("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 100, NULL, 0);


    texMine = LoadTexture("texture/texMine.png");
    texFlag = LoadTexture("texture/texFlag.png");
    texHidden = LoadTexture("texture/texHidden.png");
    texExposed = LoadTexture("texture/texExposed.png");
    
}



typedef struct field
{
    BOOL is_Mine;
    BOOL is_Hidden;
    BOOL is_Flagged;
    int number_of_mines_around_you; 
}field;

field error_field = {.is_Mine = FALSE};

typedef struct mine_field
{
    long width,height;
    size_t number_of_mines;
    field * fields;
} mine_field;

void generateMines(mine_field *f, size_t number_of_mines);
void setNumberOfMinesAround(mine_field *f, long xpos, long ypos);
field getField(mine_field *f, long xpos, long ypos);

void initMineField(mine_field *f, long width, long height, size_t number_of_mines)
{
    f->fields = (field*) malloc(sizeof(field) * width * height);

    for(long y = 0; y < height; y++)
    {
        for(long x = 0; x < width; x++)
        {

            f->fields[x + y *width].is_Mine = FALSE;
            f->fields[x + y *width].is_Hidden = TRUE;
            f->fields[x + y *width].is_Flagged = FALSE;
            f->fields[x + y *width].number_of_mines_around_you = 0;            
        }
    }
    
    f->number_of_mines = number_of_mines;
    f->width = width;
    f->height = height;
    
    generateMines(f, number_of_mines);
    
    //generateNumbers
    for(long y = 0; y < height; y++)
    {
        for(long x = 0; x < width; x++)
        {
            setNumberOfMinesAround( f, x, y);
            
        }
    }

    
}

void resetMineField(mine_field * f)
{
    long width = f->width;
    long height = f->height;
    long number_of_mines = f->number_of_mines;
    for(long y = 0; y < height; y++)
    {
        for(long x = 0; x < width; x++)
        {

            f->fields[x + y *width].is_Mine = FALSE;
            f->fields[x + y *width].is_Hidden = TRUE;
            f->fields[x + y *width].is_Flagged = FALSE;
            f->fields[x + y *width].number_of_mines_around_you = 0;            
        }
    }
    
    generateMines(f, number_of_mines);
    
    //generateNumbers
    for(long y = 0; y < height; y++)
    {
        for(long x = 0; x < width; x++)
        {
            setNumberOfMinesAround( f, x, y);          
        }
    }


}

void deleteMineField(mine_field * f)
{
    free(f->fields);
}

void UnloadImages()
{
    
}

void generateMines(mine_field *f, size_t number_of_mines)
{
    
    srand(time(NULL));
    for(size_t k = 0; k < number_of_mines; k++)
    {
        long xpos = 0;
        long ypos = 0;
        do
        {
            
            xpos = (rand()%(f->width));
            ypos = (rand()%(f->height));
            
        }
        while(f->fields[xpos + ypos * f->width].is_Mine);

        f->fields[xpos + ypos * f->width].is_Mine = TRUE;
    }
}

field getField(mine_field * f, long xpos, long ypos)
{
    BOOL result = (xpos < 0 || xpos >= f->width || ypos < 0 || ypos >= f->height);

    if(result)
    {
        return error_field;
    }
  
    return f->fields[xpos + ypos * f->width];
}

void setNumberOfMinesAround(mine_field *f, long xpos, long ypos)
{
    assert(f->width > xpos);
    assert(f->height > ypos);
    assert(0 <= xpos);
    assert(0 <= ypos);

    long result_number = 0;
    
    if(getField(f,xpos,ypos).is_Mine) return;

    int pos[] = {-1,-1, 0, -1, 1, -1,
                 -1, 0, 0, 0, 1, 0,
                -1, 1, 0, 1, 1, 1};

    for(int k = 0; k < 9; k++)
    {
        long x_new = xpos + pos[2*k];
        long y_new = ypos + pos[2*k+1];

        //If nonexistant field is selected, getField() returns an error_field which contains no mine
        if(getField(f,x_new,y_new).is_Mine) result_number++;
    }

    f->fields[xpos + ypos * f->width].number_of_mines_around_you = result_number;
    

}

void revealField(mine_field *f, long xpos, long ypos)
{
    
}

void updateField(mine_field *f)
{
    float wheel = GetMouseWheelMove();

    static BOOL dragged = FALSE;
    static Point2D ref;
    
    Vector2 pos = GetMousePosition();
    Point2D transformed = worldCoord(pos.x, pos.y, &global_zoom);

    
    if(IsKeyDown(KEY_R))
    {
        resetMineField(f);
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        if(!dragged)
        {
            dragged = TRUE;
            ref.x = pos.x;
            ref.y = pos.y;
        }else
        {
            Point2D t_ref = worldCoord(ref.x,ref.y,&global_zoom);
            global_zoom.zoom_x1 = global_zoom_ref.zoom_x1 - (transformed.x - t_ref.x);
            global_zoom.zoom_x2 = global_zoom_ref.zoom_x2 - (transformed.x - t_ref.x);
            global_zoom.zoom_y1 = global_zoom_ref.zoom_y1 - (transformed.y - t_ref.y);
            global_zoom.zoom_y2 = global_zoom_ref.zoom_y2 - (transformed.y - t_ref.y);
        }
        
    }else if(IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
    {
        global_zoom_ref.zoom_x1 = global_zoom.zoom_x1;
        global_zoom_ref.zoom_x2 = global_zoom.zoom_x2;
        global_zoom_ref.zoom_y1 = global_zoom.zoom_y1;
        global_zoom_ref.zoom_y2 = global_zoom.zoom_y2;

        dragged = FALSE;
    }

    double scale = 0.1;
        
    if(wheel > 0)
    {
        global_zoom.zoom_x1 = global_zoom.zoom_x1 + (transformed.x - global_zoom.zoom_x1)*(scale);
        global_zoom.zoom_x2 = global_zoom.zoom_x2 - (global_zoom.zoom_x2 - transformed.x)*(scale);
        global_zoom.zoom_y1 = global_zoom.zoom_y1 + (transformed.y - global_zoom.zoom_y1)*(scale);
        global_zoom.zoom_y2 = global_zoom.zoom_y2 - (global_zoom.zoom_y2 - transformed.y)*(scale);
        
        global_zoom_ref = global_zoom;

    }
    else if(wheel < 0)
    {
        global_zoom.zoom_x1 = global_zoom.zoom_x1 - (transformed.x - global_zoom.zoom_x1)*(scale) ;
        global_zoom.zoom_x2 = global_zoom.zoom_x2 + (global_zoom.zoom_x2 - transformed.x)*(scale);
        global_zoom.zoom_y1 = global_zoom.zoom_y1 - (transformed.y - global_zoom.zoom_y1)*(scale) ;
        global_zoom.zoom_y2 = global_zoom.zoom_y2 + (global_zoom.zoom_y2 - transformed.y)*(scale);
        
        global_zoom_ref = global_zoom;

    }

    
}

void renderField(mine_field *f)
{
    const int width_field = WIDTH_FIELD;
    const int height_field = HEIGHT_FIELD;

    const int xstart = 0;
    const int ystart = 0;

    Vector2 pos = GetMousePosition();
    Point2D transformed = worldCoord(pos.x, pos.y, &global_zoom);
    
    
    

    
    for(long y = 0; y < f->height; y++)
    {
        for(long x = 0; x < f->width; x++)
        {
            float xpos = xstart + x * (width_field);
            float ypos = ystart + y * (height_field);

            Point2D tmp_1 = inv_worldCoord(xpos, ypos, &global_zoom);
            Point2D tmp_2 = inv_worldCoord(xpos + width_field, ypos + height_field, &global_zoom);

            //Does the field needs to be rendered?
            if( (global_zoom.zoom_x1 > xpos || global_zoom.zoom_x2 < xpos) &&
                (global_zoom.zoom_y1 > ypos || global_zoom.zoom_y2 < ypos) &&
                (global_zoom.zoom_x1 > xpos + width_field || global_zoom.zoom_x2 < xpos + width_field) &&
                (global_zoom.zoom_y1 > ypos + height_field ||
                 global_zoom.zoom_y2 < ypos + height_field))
                continue;
            
            
            float scale = (tmp_2.x - tmp_1.x)/(width_field);
            float number_font_offset_x = (tmp_2.x - tmp_1.x)/(3.5);
            float number_font_offset_y = (tmp_2.y - tmp_1.y)/(8.0);
            
            field tmp_field = getField(f, x, y);

            if(!tmp_field.is_Hidden)
            {
                if(tmp_field.is_Flagged)
                {
                    DrawTextureEx(texFlag,
                                  (Vector2){tmp_1.x,tmp_1.y},
                                  0.0f,
                                  scale,
                                  WHITE
                    );
                }else
                {
                    DrawTextureEx(texHidden,
                                  (Vector2){tmp_1.x,tmp_1.y},
                                  0.0f,
                                  scale,
                                  WHITE
                    );
                }
                continue;
            }
 
            if(tmp_field.is_Mine)
            {
                DrawTextureEx(texFlag,
                          (Vector2){tmp_1.x,tmp_1.y},
                          0.0f,
                          scale,
                          WHITE
                );               
            }
            else if(0 < tmp_field.number_of_mines_around_you)
            {
                DrawTextureEx(texExposed,
                              (Vector2){tmp_1.x,tmp_1.y},
                              0.0f,
                              scale,
                              WHITE
                    );
            	char buf[10];
            	sprintf(buf,"%d", tmp_field.number_of_mines_around_you);
  
            	DrawTextEx(NumberFont, buf, (Vector2){.x=tmp_1.x + number_font_offset_x,.y=tmp_1.y + number_font_offset_y}, scale*40.0, 0.0, BLACK);
            }
            else
            {
                DrawTextureEx(texExposed,
                              (Vector2){tmp_1.x,tmp_1.y},
                              0.0f,
                              scale,
                              WHITE
                    );
            }
        }
    }
    
}

int main()
{
    
    //SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME);
    loadSound();
    loadTexture();
    
    SetTargetFPS(60);

    mine_field main_field;
    initMineField(&main_field,300,200,12000);

    init_zoom(&global_zoom, SCREEN_WIDTH, SCREEN_HEIGHT);
    init_zoom(&global_zoom_ref, SCREEN_WIDTH, SCREEN_HEIGHT);
    


    while(!WindowShouldClose())
    {
        BeginDrawing();
        updateField(&main_field);
        
        ClearBackground(BLACK);
        
        renderField(&main_field);
        
        EndDrawing();
    }

    deleteMineField(&main_field);

    
    
    CloseWindow();
    return 0;
}


