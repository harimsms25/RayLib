#include "raylib.h"
#include "raymath.h"
#include <cstdlib>
#include <iostream>
#include <time.h>
#include "Header.h"
#include <vector>
#include <string>
#include <cstring>


using namespace std;

vector<target> populateArrayWithRandomPositions();
bool foodInRadius(vehicle ship);
target retrieveClosestTarget(vehicle ship);

//vehicle ship(800, 800, 4.5f, 0.2f);

vector<vehicle>ships;
//vector<vehicle>ships = { vehicle(150, 150, 4.5f, 0.2f) };
// create an array of ships
time_t t = time(0);
time_t tWandering = time(0);
time_t projectileTcooldown = time(0);

//Projectile projInMotion = projInMotion = Projectile(ship.position.x, ship.position.y, 10.0f, ship.rotation, false);


vector<Projectile>projectileList;

//create an array of projectiles

int time_difference;
vector<target>foodList;



int main(void)
{

    InitWindow(800, 450, "raylib [core] example - basic window");
    SetTargetFPS(30);
    

    ships = { vehicle(200, 200, 4.5f, 0.2f,RED, LoadTexture("C:\\redship.png"),"red"),
    vehicle(550, 400, 4.5f, 0.2f,YELLOW, LoadTexture("C:\\yellowship.png"),"yellow"),
    vehicle(200, 400, 4.5f, 0.2f,GREEN, LoadTexture("C:\\greenship.png"),"green"),
    vehicle(550, 200, 4.5f, 0.2f,BLUE, LoadTexture("C:\\blueship.png"),"blue"),
    };
    for (int i = 0; i < ships.size(); i++) {
        projectileList.push_back(Projectile(ships[i].position.x, ships[i].position.y, 10.0f, ships[i].rotation, false));
    }
    foodList = populateArrayWithRandomPositions();

    //Texture2D img = LoadTexture("C:\\ship.png");




    while (!WindowShouldClose())
    {

        ClearBackground(WHITE);

        BeginDrawing();
        ClearBackground(WHITE);


        //for (int i = 0; i < ships.size(); i++) {
        
            for (int x = 0; x < ships.size(); x++) {
                //cout << "hi";
                if (foodInRadius(ships[x])) {
                    target tar = retrieveClosestTarget(ships[x]);
                    ships[x].seek(tar.x, tar.y);
                }
                else {
                    ships[x].seek(ships[x].randomlyGeneratedPos.x, ships[x].randomlyGeneratedPos.y);
                }
                //cout << "hi";
                //ship.seek(tar.x, tar.y);

                //ship.update();
                if (projectileList[x].IsProjected) {
                    projectileList[x].update();
                    DrawCircle(projectileList[x].xPos, projectileList[x].yPos, 5.0f, projectileList[x].color);

                }
                //cout << "hi";
                for (int i = foodList.size() - 1; i >= 0; i--) {
                    //cout << "hi";

                    if (sqrt(pow(projectileList[x].xPos - foodList[i].x, 2) + pow(projectileList[x].yPos - foodList[i].y, 2)) < 5.0f && projectileList[x].IsProjected) {
                        projectileList[x].xVel = 0;
                        projectileList[x].yVel = 0;
                        ships[x].bulletHitSomething = true;
                        ships[x].floatPoint = target{ foodList[i].x,foodList[i].y };
                    }

                    //cout << "hi";
                    if (Vector2Distance({ ships[x].position.x,ships[x].position.y }, { foodList[i].x,foodList[i].y }) < 20) {
                        foodList.erase(foodList.begin() + i);
                        ships[x].addScore();
                    }


                }

                if (ships[x].bulletHitSomething) {
                    ships[x].floatTo(ships[x].floatPoint.x, ships[x].floatPoint.y);
                    if (Vector2Distance({ ships[x].position.x,ships[x].position.y }, { ships[x].floatPoint.x,ships[x].floatPoint.y }) < 20) {
                        ships[x].bulletHitSomething = false;
                        projectileList[x].IsProjected = false;
                    }
                }
                else
                {
                    ships[x].update();
                }


                if (projectileList[x].xPos < 0 || projectileList[x].xPos > 800 || projectileList[x].yPos < 0 || projectileList[x].yPos > 450) {
                    projectileList[x].IsProjected = false;
                }
            }

            time_t ct = time(0);
            time_t ctWandering = time(0);
            time_t projectileTime = time(0);

            time_difference = 30.0f - (ct - t);
            if (ct - t >= 30) {


                foodList = populateArrayWithRandomPositions();
                int indexToRemove = 0;
                int smallestScore = ships[0].score;
                for (int i = 1; i < ships.size(); i++) {
                    if (ships[i].score < smallestScore) {
                        smallestScore = ships[i].score;
                        indexToRemove = i;
                    }
                }

                ships.erase(ships.begin() + indexToRemove);
                projectileList.erase(projectileList.begin() + indexToRemove);

                for (int i = 0; i < ships.size(); i++) {
                    int evolveID = GetRandomValue(1, 3);
                    cout << evolveID << endl;
                    if (evolveID == 1) {
                        ships[i].increasemaxV();
                    }
                    else if (evolveID == 2) {
                        ships[i].increaseRadius();
                    }
                    else if (evolveID == 3) {
                        projectileList[i].increaseSpeed();
                    }
                    ships[i].score = 0;
                }

                t = ct;


            }

            if (ctWandering - tWandering >= 1.5f) {
                for (int x = 0; x < ships.size(); x++) {
                    ships[x].randomlyGeneratedPos = { (float)GetRandomValue(50,750),(float)GetRandomValue(50,400) };
                }
                tWandering = ctWandering;
            }

            for (int x = 0; x < ships.size(); x++) {

                Vector2 velocity = ships[x].velocity;
                Vector2 dVelocity = Vector2ClampValue(velocity, 0, ships[x].maxV);

                for (int i = 0; i < foodList.size(); i++) {
                    DrawCircle(foodList[i].x, foodList[i].y, 5.0f, BLACK);
                }
                if (velocity.x == 0) {
                    if (velocity.y > 0) {
                        ships[x].rotation = 180;
                    }
                    else if (velocity.y < 0) {
                        ships[x].rotation = 0;
                    }
                }
                else {
                    float angle = atanf((float)abs(dVelocity.y) / abs(dVelocity.x)) * 180 / PI;

                    if (velocity.x > 0) {
                        if (velocity.y >= 0) {
                            ships[x].rotation = angle + 90;
                        }
                        else {
                            ships[x].rotation = 90 - angle;
                        }
                    }
                    else {
                        if (velocity.y > 0) {
                            ships[x].rotation = 270 - angle;
                        }
                        else {
                            ships[x].rotation = angle + 270;
                        }
                    }
                }
                Texture2D img = ships[x].shipImage;
                float scale = 0.075f;
                float rOfImage = sqrt((img.height / 2.0f) * (img.height / 2.0f) + (img.width / 2.0f) * (img.width / 2.0f));
                float angleOfRAtNorm = atanf((float)img.height / img.width) * 180 / PI;
                float fixedAngle = ships[x].rotation + angleOfRAtNorm;
                float xDiff = scale * rOfImage * cosf(fixedAngle * PI / 180.0f);
                float yDiff = scale * rOfImage * sinf(fixedAngle * PI / 180.0f);
                DrawTextureEx(img, { ships[x].position.x - xDiff,ships[x].position.y - yDiff }, ships[x].rotation, scale, WHITE);
                DrawCircleLines(ships[x].position.x, ships[x].position.y, ships[x].sightRadius, ships[x].colorOfVehicle);


                char buffer[20];
                sprintf(buffer, "%d", ships[x].score);
                char* char_ptr = buffer;

                DrawText(char_ptr, 25, 25 + x * 30, 36, ships[x].shipColor);
                string speedString = ships[x].speedToString;
                const char* speedShow = speedString.c_str();
                DrawText(speedShow, ships[x].position.x - 15.0f, ships[x].position.y + 30.0f, 8, BLACK);
            }
            if (projectileTime - projectileTcooldown >= 15) {
                for (int x = 0; x < projectileList.size(); x++) {
                    projectileList[x] = Projectile(ships[x].position.x, ships[x].position.y, 10.0f, ships[x].rotation, true);
                }
                projectileTcooldown = projectileTime;
            }

            char bufferTime[3];
            sprintf(bufferTime, "%d", time_difference);
            char* chat_ptrToTime = bufferTime;
            DrawText(chat_ptrToTime, 750, 25, 36, BLACK);

            if (ships.size() == 1) {
                string winner = ships[0].color+"  has won the game!";
                const char* w = winner.c_str();
                DrawText(w, 0, 200, 64, BLACK);
            }
        //}
        

        EndDrawing();
        
    }
    
    CloseWindow();

    return 0;
}

vector<target> populateArrayWithRandomPositions() {
    
    vector<target> tempList;
    for (int i = 0; i < 100; i++) {
        target tar;
        tar.x = (float)GetRandomValue(50, 750);
        tar.y = (float)GetRandomValue(50, 400);
        tempList.push_back(tar);
    }
    return tempList;
}

bool foodInRadius(vehicle ship) {
    for (int i = 0; i < foodList.size(); i++) {
        if (sqrt(pow(ship.position.x - foodList[i].x, 2) + pow(ship.position.y - foodList[i].y, 2)) < ship.sightRadius)
            return true;
    }
    return false;
}

target retrieveClosestTarget(vehicle ship) {
    float smallestDistance = sqrt(pow(ship.position.x - foodList[0].x, 2) + pow(ship.position.y - foodList[0].y, 2));
    int index = 0;
    for (int i = 1; i < foodList.size(); i++) {
        if (sqrt(pow(ship.position.x - foodList[i].x, 2) + pow(ship.position.y - foodList[i].y, 2)) < smallestDistance) {
            smallestDistance = sqrt(pow(ship.position.x - foodList[i].x, 2) + pow(ship.position.y - foodList[i].y, 2));
            index = i;
        }
    }
    return foodList[index];
}