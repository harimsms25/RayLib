#pragma once
#include <iostream>
#include <cstdlib>
#include "raymath.h"
#include "raylib.h"

using namespace std;

Vector2 setMag(Vector2 v, float length);
#ifndef MY_STRUCT_H
#define MY_STRUCT_H
struct target
{
    float x;
    float y;
    //Vector2 allcoords[10];
};
#endif // !1

#ifndef MY_CLASS_F
#define MY_CLASS_F

class Projectile {
public:
    float xPos;
    float yPos;
    float Speed;
    float AngleOfShot;
    float xVel;
    float yVel;
    bool IsProjected;
    float ogSpeed;
    Color color;
    Projectile::Projectile(float xpos, float ypos, float speed, float angleOfShot, bool isProjected) {
        xPos = xpos;
        yPos = ypos;
        ogSpeed = speed;
        Speed = speed;
        AngleOfShot = angleOfShot;
        xVel = Speed * sin(PI - AngleOfShot * PI / 180.0f);
        yVel = Speed * cos(PI - AngleOfShot * PI / 180.0f);
        color = RED;

        IsProjected = isProjected;
    }
    void Projectile::update() {
        xPos += xVel;
        yPos += yVel;
        //xPos++;

    }
    void Projectile::changeProjection() {
        IsProjected = !IsProjected;
    }
    void Projectile::increaseSpeed() {
        Speed += 2;
        xVel = Speed * sin(PI - AngleOfShot * PI / 180.0f);
        yVel = Speed * cos(PI - AngleOfShot * PI / 180.0f);
        if (Speed == ogSpeed+2) {
            color = ORANGE;
        }
        else if (Speed == ogSpeed + 4) {
            color = YELLOW;
        }
        else if (Speed == ogSpeed + 6) {
            color = GREEN;
        }
        else if (Speed == ogSpeed + 8) {
            color = BLUE;
        }
    }
};

#endif

#ifndef MY_CLASS_H
#define MY_CLASS_H

class vehicle {
public:
    vehicle::vehicle(float x, float y, float mv, float mf, Color c, Texture2D img,string colorstring) {
        position = { x,y };
        velocity = { 0.0f,0.0f };
        acceleration = { 0.0f, 0.0f };
        desiredClamped = { 0.0f,0.0f };
        float rotation = 0;
        maxV = mv;
        ogMaxSpeed = mv;
        maxF = mf;
        speedToString = "Speed I";
        shipColor = c;
        shipImage = img;
        bulletHitSomething = false;
        randomlyGeneratedPos = { (float)GetRandomValue(50, 750),(float)GetRandomValue(50, 400) };
        color = colorstring;
        
        //randomlyGeneratedPos = random;
    }
    void vehicle::seek(int xPos, int yPos) {
        Vector2 desired = { xPos - position.x,yPos - position.y };
        desiredClamped = setMag(desired, maxV);
        Vector2 steering = Vector2Subtract(desiredClamped, velocity);
        Vector2 steeringClamped = Vector2ClampValue(steering, 0, maxF);

        acceleration = Vector2Add(acceleration, steeringClamped);
    }
    void vehicle::update() {
        velocity = Vector2Add(velocity, acceleration);
        Vector2 dVelocity = Vector2ClampValue(velocity, 0, maxV);

        position = Vector2Add(position, dVelocity);
        acceleration = Vector2{ 0,0 };
    }
    void vehicle::floatTo(int xPos,int yPos) {
        Vector2 desired = { xPos - position.x,yPos - position.y };
        Vector2 vel = setMag(desired, maxV);
        position = Vector2Add(position, vel);
    }
    void vehicle::addScore() {
        score++;
    }
    void vehicle::increaseRadius() {
        sightRadius += 15.0f;
        if (sightRadius == 75.0f) {
            colorOfVehicle = ORANGE;
        }
        else if(sightRadius == 90.0f){
            colorOfVehicle = YELLOW;
        }
        else if (sightRadius == 105.0f) {
            colorOfVehicle = GREEN;
        }
        else if (sightRadius == 120.0f) {
            colorOfVehicle = BLUE;
        }
    }
    void vehicle::increasemaxV() {
        maxV+=2;
        if (maxV == ogMaxSpeed+2) {
            speedToString = "Speed II";
        }
        else if (maxV == ogMaxSpeed + 4) {
            speedToString = "Speed III";
        }
        else if (maxV == ogMaxSpeed + 6) {
            speedToString = "Speed IV";
        }
        else if (maxV == ogMaxSpeed + 8) {
            speedToString = "Speed V";
        }
    }
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    Vector2 desiredClamped;
    //Vector2 desiredClampedFloater;
    float rotation;
    float maxV;
    float maxF;
    float sightRadius = 60.0f;
    Color colorOfVehicle = RED;
    int score = 0;
    bool bulletHitSomething;
    Vector2 randomlyGeneratedPos;
    //Vector2 randomlyGeneratedPos;
    target floatPoint;
    string speedToString;
    float ogMaxSpeed;
    Color shipColor;
    Texture2D shipImage;
    string color;
};

#endif

