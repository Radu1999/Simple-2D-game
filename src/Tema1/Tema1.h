#pragma once
#include "Character.h"
#include <queue>

class Tema1 : public gfxc::SimpleScene
{
public:
    Tema1();
    ~Tema1();

    void Init() override;

    struct ViewportSpace
    {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };

    struct LogicSpace
    {
        LogicSpace() : x(0), y(0), width(1), height(1) {}
        LogicSpace(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}
        float x;
        float y;
        float width;
        float height;
    };


private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;
    void movePlayer(float deltaTime);
    glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
    void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear);
    void deleteOutOfBoundsBullets();
    void deleteObstructedBullets();
    bool checkPlayerObstacle(float pX, float pY);
    bool checkCircleCollision(Character* ob1, Character* ob2);
    bool checkCircleRectCollision(Character* opCirc, Character* obRec);
    void eliminateEnemies();
    void checkIfDamaged();
    void deleteBulletsOutOfRange();

protected:
    

    // TODO(student): If you need any other class variables, define them here.
    Character *player;
    float primitiveSize;
    float playerSpeed;
    ViewportSpace viewSpace;
    LogicSpace logicSpace;
    glm::mat3 visMatrix;
    std::vector<Character*> bullets;
    float bulletSpeed;
    std::vector<Character*> obstacles;
    std::vector<Character*> enemies;
    double  spawnTime;
    float mapSize;
    float healthBar;
    bool gameOver;
    float fireRate;
    float lastShot;
    //Score will be displayed in command line
    int score;
    //Top scores will be displayed in command line at the end
    std::vector<int> top_scores;
    //gun range
    float bulletRange;
};

