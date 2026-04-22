#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include <raylib.h>



#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 1100
#define WINDOW_NAME "Minesweeper"

typedef char BOOL;

#define TRUE 1
#define FALSE 0




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
    NumberFont = LoadFont("/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed.ttf");

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

    for(size_t y = 0; y < height; y++)
    {
        for(size_t x = 0; x < width; x++)
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
    for(size_t y = 0; y < height; y++)
    {
        for(size_t x = 0; x < width; x++)
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
    for(size_t y = 0; y < height; y++)
    {
        for(size_t x = 0; x < width; x++)
        {

            f->fields[x + y *width].is_Mine = FALSE;
            f->fields[x + y *width].is_Hidden = TRUE;
            f->fields[x + y *width].is_Flagged = FALSE;
            f->fields[x + y *width].number_of_mines_around_you = 0;            
        }
    }
    
    generateMines(f, number_of_mines);
    
    //generateNumbers
    for(size_t y = 0; y < height; y++)
    {
        for(size_t x = 0; x < width; x++)
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
    if(IsKeyPressed(KEY_R))
    {
        resetMineField(f);
    }
}

void renderField(mine_field *f)
{
    const int width_field = 50;
    const int height_field = 50;

    const int xstart = 10;
    const int ystart = 10;
    
    for(long y = 0; y < f->height; y++)
    {
        for(long x = 0; x < f->width; x++)
        {
            float xpos = xstart + x * (width_field + 2);
            float ypos = ystart + y * (height_field + 2);
            DrawRectangle(xpos,
                          ypos,
                          width_field,
                          height_field,
                          GRAY);
            field tmp_field = getField(f, x, y);

            if(!tmp_field.is_Hidden)
            {
                //continue;
            }
            
            if(tmp_field.is_Mine)
            {
            	DrawTextEx(NumberFont, "X", (Vector2){.x=xpos,.y=ypos}, 40.0, 0.0, BLACK);
            }
            else if(0 < tmp_field.number_of_mines_around_you)
            {
            	char buf[10];
            	sprintf(buf,"%d", tmp_field.number_of_mines_around_you);
            	
            	DrawTextEx(NumberFont, buf, (Vector2){.x=xpos,.y=ypos}, 40.0, 0.0, RED);
            }
        }
    }
}

int main()
{
    
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME);
    loadSound();
    loadTexture();
    
    SetTargetFPS(60);

    mine_field main_field;

    initMineField(&main_field,20,15,100);
    

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


