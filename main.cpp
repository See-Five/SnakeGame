#include <raylib.h>
#include <iostream>
#include<deque>
#include<queue>
#include<raymath.h>
using namespace std;
 
Color green = Color{143, 225, 80, 255};
Color greenL = Color{190, 255, 115, 255};
Color LightBlue = Color{131, 215, 198, 255};
Color Purple = Color{98, 13, 104, 255};
Color Yellow = Color{175, 163, 0, 255};
Color DarkGreen = Color{81, 168, 110, 255};

vector<Color> colors = 
{
    DarkGreen,
    Purple,
    Yellow,
    LightBlue
};

float cellsize = 30;
const int cellcount = 25;
double LastUpdateTime = 0;
bool running = false ,GameOver = false;
int Score = 0, FoodEaten = 0;
int CurrentColor = 0;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - LastUpdateTime >= interval)
    {
        LastUpdateTime = currentTime;
        return true;
    };
    return false;
};

class Snake 
{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} }; 
    Vector2 direction = {1, 0};
    Vector2 DirectionOfLastRec ;
    queue<Vector2> saveDirection;
    Vector2 OldTail;

    Snake()
    {
        saveDirection.push(direction);
    };
     
    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{x * cellsize, y * cellsize, cellsize, cellsize};
            DrawRectangleLinesEx(segment, 12, colors[CurrentColor]);
        };
    };

    void Update()
    {
        DirectionOfLastRec = body.back();
        OldTail = body.back();
        body.pop_back();
        while (!saveDirection.empty())
        {
            direction = saveDirection.front();
            saveDirection.pop();   
        };
        body.push_front(Vector2Add(body[0], direction)); 

    };
};

class Food
{
public:   

    Vector2 position;
    Texture2D texture; 
    int i;
    Food(deque<Vector2> SnakeBody)
    {
        Image image = LoadImage("apple.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GeneratRandomPos(SnakeBody);
    };

    ~Food()
    {
        UnloadTexture(texture);
    };

    void Draw()
    {
        DrawTexture(texture, position.x * cellsize, position.y * cellsize, WHITE);
    };
   
    Vector2 GeneratRandomPos(deque<Vector2> SnakeBody)
    {
        i = 0;
        float x = GetRandomValue(1, cellcount - 2);
        float y = GetRandomValue(3, cellcount - 2);
        Vector2 element = {x, y};
        for(i=0; i<SnakeBody.size()-1; i++)
        {
            if(element == SnakeBody[i]){
                return GeneratRandomPos(SnakeBody);
            }
        };
        return element;
    };
};
class MainMenu{
public:
    Rectangle Restart;
    float Mini_cellsize = 0.4*cellsize;  
    float width_restart = cellsize , width_Menu = 60 , width_MenuTexture = 1;  
    bool CollisionPointRec = true , CollisionPointMenu = true;
    Texture2D texture ,MenuTexture;
    MainMenu(deque<Vector2> SnakeBody, Vector2 LastRecDirection){
        Image image = LoadImage("MiniApple.png");
        Image MenuIcon = LoadImage("Menu.png");
        texture = LoadTextureFromImage(image);
        MenuTexture = LoadTextureFromImage(MenuIcon);
        UnloadImage(image);
        UnloadImage(MenuIcon);
    };
    ~MainMenu()
    {
        UnloadTexture(texture);
        UnloadTexture(MenuTexture);
    };

    void DrawGameOverScreen(deque<Vector2> SnakeBody ,Vector2 LastRecDirection){
        ClearBackground(WHITE);
        Vector2 direvtion;
        DrawLine(cellsize*3, cellsize*3, GetScreenWidth() - cellsize*3, cellsize*3, BLACK);
        DrawText("GAME OVER", GetScreenWidth()/2 - cellsize*3, cellsize*2 - 15, 30, BLACK);
        DrawRectangle(cellsize*3, cellsize*5, Mini_cellsize*cellcount, Mini_cellsize*cellcount, greenL);
        DrawRectangle(cellsize*3, cellsize*5, Mini_cellsize*cellcount, Mini_cellsize*3, green);
        DrawRectangle(cellsize*3, cellsize*5, Mini_cellsize, cellcount*Mini_cellsize, green);
        DrawRectangle(cellsize*3 + Mini_cellsize*cellcount - Mini_cellsize, cellsize*5, Mini_cellsize, Mini_cellsize*cellcount, green);
        DrawRectangle(cellsize*3, cellsize*5 + Mini_cellsize*cellcount - Mini_cellsize, Mini_cellsize*cellcount, Mini_cellsize, green);
        
        Rectangle background = {cellsize*3 + Mini_cellsize - 4, cellsize*5 + Mini_cellsize*3 - 4, Mini_cellsize*cellcount - Mini_cellsize*3+20, Mini_cellsize*cellcount - Mini_cellsize*5+20};
        DrawRectangleLinesEx(background, 4,Fade(BLACK, 0.15f));
        DrawRectangle(cellsize*3 + Mini_cellsize - 3.5,cellsize*5 + Mini_cellsize-7, Mini_cellsize*cellcount -Mini_cellsize*2+7, Mini_cellsize + 0.4*cellsize, Fade(BLACK, 0.25f));
        DrawTexture(texture, cellsize*3 + Mini_cellsize-10 + Mini_cellsize,cellsize*5 + Mini_cellsize - Mini_cellsize/10, WHITE);
        DrawText(TextFormat("%i",FoodEaten), cellsize*3 + Mini_cellsize + Mini_cellsize/2 + 0.4*cellsize, cellsize*5 + Mini_cellsize, 8,BLACK);
        DrawText(TextFormat("SCORE : %0.5d",Score),cellsize*3 + Mini_cellsize*cellcount - Mini_cellsize*8, cellsize*5 + Mini_cellsize, 8,BLACK);
    
        DrawText(TextFormat("SCORE :  : %0.5d",Score), GetScreenWidth()/2 + 3*cellsize, cellsize*6, 20, BLACK);
        DrawText(TextFormat("LENGTH : %i",SnakeBody.size()), GetScreenWidth()/2 + 3*cellsize, cellsize*9, 20, BLACK);
        DrawText(TextFormat("That was too fast for you"), GetScreenWidth()/2 + 3*cellsize, cellsize*12, 20, Fade(BLACK, 0.35f));

        Rectangle Restart = {(float)GetScreenWidth()/2, (float)3*GetScreenHeight()/4 , width_restart*8, width_restart*2};
        DrawRectangleRounded(Restart, 0.2, 12, {48, 73, 42, 255});
        DrawRectangleRoundedLinesEx(Restart, 0.2, 32, 1, Fade(BLACK, 0.8f));
        DrawRectangleRoundedLinesEx({(float)GetScreenWidth()/2 , (float)3*GetScreenHeight()/4 , width_restart*8, width_restart*2}, 0.2, 32, 5, Fade(BLACK, 0.3f));
        DrawText(TextFormat("Restart"), (float)GetScreenWidth()/2 + 2*width_restart + Mini_cellsize, (float)3*GetScreenHeight()/4 + width_restart - 12, 24, WHITE);
        
        DrawRectangleRounded({GetScreenWidth()/3 - 2*cellsize , (float)3*GetScreenHeight()/4 , width_Menu ,width_Menu},0.3, 12, greenL);
        DrawRectangleRoundedLinesEx({GetScreenWidth()/3 - 2*cellsize , (float)3*GetScreenHeight()/4 , width_Menu ,width_Menu}, 0.3, 12, 3, Fade(BLACK, 0.3f));
        DrawTextureEx(MenuTexture,{GetScreenWidth()/3 -2*cellsize  , (float)3*GetScreenHeight()/4}, 0, width_MenuTexture, WHITE);
        
        SnakeBody.pop_front();
        SnakeBody.push_back(LastRecDirection);
        for(int i=0; i < SnakeBody.size(); i++){
            DrawRectangleLinesEx({cellsize*3 + SnakeBody[i].x*Mini_cellsize, cellsize*5 + SnakeBody[i].y*Mini_cellsize, Mini_cellsize, Mini_cellsize}, 4.3, colors[CurrentColor]);
        };

    };

    void DrawMainMenuScreen(){
        ClearBackground(WHITE);
        DrawLine(cellsize*3, cellsize*3, GetScreenWidth() - cellsize*3, cellsize*3, BLACK);
        DrawText("SNAKE GAME", GetScreenWidth()/2 - 3*cellsize, cellsize*2 - cellsize/2, cellsize, BLACK);
        DrawRectangleRounded({GetScreenWidth()/2 - cellsize*3, cellsize*5, cellsize*6, (float)GetScreenHeight()/2}, 0.2, 12, Fade(BLACK, 0.3f));
        DrawRectangleRoundedLinesEx({GetScreenWidth()/2 - cellsize*3, cellsize*5, cellsize*6, (float)GetScreenHeight()/2}, 0.2, 12, 2, Fade(BLACK, 0.8f));
        DrawText(TextFormat("COLORS"), GetScreenWidth()/2 - 4*cellsize/3 - Mini_cellsize, cellsize*6, cellsize - cellsize/6, BLACK);
        DrawRectangle(GetScreenWidth()/2 - cellsize/2, cellsize*8, cellsize, cellsize, DarkGreen);
        if(CurrentColor == 0){
            DrawRectangleLinesEx({GetScreenWidth()/2 - cellsize/2, cellsize*8, cellsize, cellsize}, 4, BLACK);
        };
        DrawRectangle(GetScreenWidth()/2 - cellsize/2, cellsize*10, cellsize, cellsize, Purple);
        if(CurrentColor == 1){
            DrawRectangleLinesEx({GetScreenWidth()/2 - cellsize/2, cellsize*10, cellsize, cellsize}, 4, BLACK);
        };
        DrawRectangle(GetScreenWidth()/2 - cellsize/2, cellsize*12, cellsize, cellsize, Yellow);
        if(CurrentColor == 2){
            DrawRectangleLinesEx({GetScreenWidth()/2 - cellsize/2, cellsize*12, cellsize, cellsize}, 4, BLACK);
        };
        DrawRectangle(GetScreenWidth()/2 - cellsize/2, cellsize*14, cellsize, cellsize, LightBlue);
        if(CurrentColor == 3){
            DrawRectangleLinesEx({GetScreenWidth()/2 - cellsize/2, cellsize*14, cellsize, cellsize}, 4, BLACK);
        };
        DrawRectangleRounded({GetScreenWidth()/2 - cellsize*3, (float)3*GetScreenHeight()/4, cellsize*6, cellsize*2}, 0.2, 12, {48, 73, 42, 255});
        DrawRectangleRoundedLinesEx({GetScreenWidth()/2 - cellsize*3, (float)3*GetScreenHeight()/4, cellsize*6, cellsize*2}, 0.2, 12, 5, Fade(BLACK, 0.3f));
        DrawText(TextFormat("PLAY"), GetScreenWidth()/2 - 4*cellsize/3,(float)3*GetScreenHeight()/4 + cellsize/2, cellsize, WHITE);
    };

    void CheckCollisionWithRestart(Rectangle r){

        if(CheckCollisionPointRec(GetMousePosition(), r)&& CollisionPointRec == true){
            width_restart = width_restart + 1.02;
            CollisionPointRec = false;
        };
        if(CheckCollisionPointRec(GetMousePosition(), r) == false){
            width_restart = cellsize;
            CollisionPointRec = true;
        };
    };

    void CheckCollisionWithMenuIcon(){
        if(CheckCollisionPointRec(GetMousePosition(), {GetScreenWidth()/3 - 2*cellsize , (float)3*GetScreenHeight()/4 , width_Menu ,width_Menu })&& CollisionPointMenu == true){
            width_Menu = width_Menu + 2;
            width_MenuTexture = 1.02;
            CollisionPointMenu = false;
        };
        if(CheckCollisionPointRec(GetMousePosition(), {GetScreenWidth()/3 - 2*cellsize , (float)3*GetScreenHeight()/4 , width_Menu ,width_Menu }) == false){
            width_Menu = cellsize*2;
            width_MenuTexture = 1;
            CollisionPointMenu = true;
        };
    };

    bool ButtonPressed(Rectangle r){
        return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), r);
    };
    
    void CheckColorSelection(Rectangle r ,int ColorNumber){
        if(ButtonPressed(r)){
            CurrentColor = ColorNumber;
            DrawRectangleLinesEx(r, 5, BLACK);
        };
    };
};

class Game{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    MainMenu Mmenu = MainMenu(snake.body, snake.DirectionOfLastRec); 
    int i=0;
    Vector2 OldTail;

    void Draw()
    {
        if(running == true){
            DrawBackground();
            snake.Draw();
            food.Draw();
        };
        if(running == false && GameOver == true){
            Mmenu.DrawGameOverScreen(snake.body, snake.DirectionOfLastRec);
        };
        if(running == false && GameOver == false){
            Mmenu.DrawMainMenuScreen();
        };
    };
    void DrawBackground(){
        ClearBackground(greenL);
        DrawRectangle(0, 0, GetScreenWidth(), cellsize*3, green);
        DrawRectangle(0, 0, cellsize, GetScreenHeight(), green);
        DrawRectangle(GetScreenWidth()-cellsize, 0, cellsize, GetScreenHeight(), green);
        DrawRectangle(0, GetScreenHeight()-cellsize, GetScreenWidth(), cellsize, green);
        Rectangle background = {cellsize-10, cellsize*3-10, GetScreenWidth()-cellsize*2+20, GetScreenHeight()-cellsize*4+20};
        DrawRectangleLinesEx(background, 10,Fade(BLACK, 0.15f));
        DrawRectangle(cellsize-10, cellsize-20, GetScreenWidth()-cellsize*2+20, cellsize + 30, Fade(BLACK, 0.25f));  
        DrawTexture(food.texture, cellsize-10 + cellsize/2, cellsize-20 + cellsize/2 - cellsize/10, WHITE);
        DrawText(TextFormat("%i",FoodEaten), cellsize-10 + cellsize/2 + 40, cellsize-20 + cellsize/2 + 2.5, 25,BLACK);
        DrawText(TextFormat("SCORE : %0.5d",Score), cellsize-10 + GetScreenWidth()-cellsize*2+20 - cellsize*6, cellsize-20 + cellsize/2 + 5, 20,BLACK);      
    };

    void Update()
    {
        if(running == true){
            if(EventTriggered(0.2)){
                snake.Update();
            };

            if(IsKeyPressed(KEY_UP) && snake.direction.y != 1){
                snake.saveDirection.push(Vector2({0, -1}));
            };
            if(IsKeyPressed(KEY_DOWN) && snake.direction.y != -1){
                snake.saveDirection.push(Vector2({0, 1}));
            };
            if(IsKeyPressed(KEY_RIGHT) && snake.direction.x != -1){  
                snake.saveDirection.push(Vector2({1, 0}));
            };
            if(IsKeyPressed(KEY_LEFT) && snake.direction.x != 1){
                snake.saveDirection.push(Vector2({-1, 0}));
            };

            CheckcollisionSnakeFood();
            CheckCollisionWithEdges();
            CheckCollisionwithHead();
        };
        if(running == false && GameOver == true){
            Mmenu.CheckCollisionWithRestart({(float)GetScreenWidth()/2, (float)3*GetScreenHeight()/4 , Mmenu.width_restart*8, Mmenu.width_restart*2});
            Mmenu.CheckCollisionWithMenuIcon();
            if(Mmenu.ButtonPressed({(float)GetScreenWidth()/2, (float)3*GetScreenHeight()/4 , cellsize*8, cellsize*2})){
                running = true;
                snake.body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
                FoodEaten = 0;
                Score = 0 ;
            };
            if(Mmenu.ButtonPressed({GetScreenWidth()/3 - 2*cellsize , (float)3*GetScreenHeight()/4 , Mmenu.width_Menu ,Mmenu.width_Menu})){
                GameOver = false;
            };
        };
        if(running == false && GameOver == false){
            Mmenu.CheckColorSelection({GetScreenWidth()/2 - cellsize/2, cellsize*8, cellsize, cellsize}, 0);
            Mmenu.CheckColorSelection({GetScreenWidth()/2 - cellsize/2, cellsize*10, cellsize, cellsize}, 1);
            Mmenu.CheckColorSelection({GetScreenWidth()/2 - cellsize/2, cellsize*12, cellsize, cellsize}, 2);
            Mmenu.CheckColorSelection({GetScreenWidth()/2 - cellsize/2, cellsize*14, cellsize, cellsize}, 3);
            if(Mmenu.ButtonPressed({GetScreenWidth()/2 - cellsize*3, (float)3*GetScreenHeight()/4, cellsize*6, cellsize*2})){
                running = true;
                snake.body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
                FoodEaten = 0;
                Score = 0 ;
            };
        };
    };
    void CheckcollisionSnakeFood(){
        if(snake.body[0] == food.position){
            food.position = food.GeneratRandomPos(snake.body);
            snake.body.push_back(snake.OldTail);
            FoodEaten++ ;
            Score = Score + 10 ;
        };
    };

    void CheckCollisionWithEdges()
    {
        if(snake.body[0].x == cellcount-1 || snake.body[0].x == 0)
        {
            ResetSnake();
        };
        if(snake.body[0].y == 2 || snake.body[0].y == cellcount-1){
            ResetSnake();
        };
    };

    void CheckCollisionwithHead(){
        for(int i=1; i <= snake.body.size() - 1; i++){
            if(snake.body[i] == snake.body[0]){
                ResetSnake();
            };
        };
    };

    void ResetSnake(){
        for(int i=0 ;i<snake.saveDirection.size(); i++){
            snake.saveDirection.pop();
        };
        snake.direction = {1, 0};
        food.position = food.GeneratRandomPos(snake.body);
        running = false;
        GameOver = true;
    };
};

int main()
{   
    SetConfigFlags(FLAG_MSAA_4X_HINT);  //For smoother -anti aliased edges of shaps

    InitWindow(cellsize*cellcount, cellsize*cellcount, "Snake Game!");
    SetTargetFPS(60);
    Game game = Game();

    while(!WindowShouldClose())
    {
        BeginDrawing();

        //Updating
        game.Update();

        //Drawing
        ClearBackground(BLACK);
        game.Draw();

        EndDrawing(); 
    };
    CloseWindow();
    return 0;
};