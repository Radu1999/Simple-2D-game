#include "Tema1.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Obstacle.h"

#include <vector>
#include <iostream>

using namespace std;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{

    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    glDisable(GL_DEPTH_TEST);

    camera->Update();
    spawnTime = Engine::GetElapsedTime();
    healthBar = 5;
    score = 0;
    cout << "Your score is: " + to_string(score) + "\n";

    //Initialize fireRate
    fireRate = 0.5;
    lastShot = Engine::GetElapsedTime();

    //Initialize bulletRange
    bulletRange = 500.f;

    // Initial size of circle radius and square len
    primitiveSize = 30;
    mapSize = 60 * primitiveSize;
    gameOver = 0;
    
    // Speed of Player
    playerSpeed = 300;
    bulletSpeed = 600;

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = resolution.x;   // logic width
    logicSpace.height = resolution.y;  // logic height


    GetCameraInput()->SetActive(false);
    {
        Mesh* mesh = shapes2D::CreateCircle("circle_full", glm::vec3(0, 0, 1), primitiveSize, glm::vec3(2, 1, 0), true);
        AddMeshToList(mesh);
    }
    {
        Mesh* mesh = shapes2D::CreateCircle("circle_empty", glm::vec3(0, 0, 1), primitiveSize, glm::vec3(2, 1, 0), false);
        AddMeshToList(mesh);
    }
    {
        Mesh* mesh = shapes2D::CreateSquare("square_full", glm::vec3(0, 0, 1), primitiveSize, glm::vec3(2, 1, 0), true);
        AddMeshToList(mesh);
    }

    {
        Mesh* mesh = shapes2D::CreateSquare("square_empty", glm::vec3(0, 0, 1), primitiveSize, glm::vec3(2, 1, 0), false);
        AddMeshToList(mesh);
    }

    {
        Mesh* mesh = shapes2D::CreateTriangle("triangle_empty", glm::vec3(0, 0, 1), primitiveSize, glm::vec3(2, 1, 0), false);
        AddMeshToList(mesh);
    }

    player = new Player(glm::vec3(0, 0, 1), primitiveSize);
   // player->move(resolution.x / 2, resolution.y / 2);


    //Create obstacles
    for (int i = 0; i < 20; i++) {
        int sx = rand() % 5 + 1;
        int sy = rand() % 5 + 1;
        int sgnX = rand() % 2;
        int sgnY = rand() % 2;
        int offset = primitiveSize;
        if (!sgnX) {
            sgnX--;
            offset += primitiveSize * sx;
        }
        if (!sgnY) {
            sgnY--;
            offset += primitiveSize * sy;
        }
        Character* obstacle = new Obstacle(glm::vec3(sgnX * (offset + rand() % (int)(mapSize / 2 - offset - sx * primitiveSize)),
                                                     sgnY * (offset + rand() % (int)(mapSize / 2 - offset - sy * primitiveSize)), 1),
                                                    primitiveSize, sx, sy);
        obstacles.push_back(obstacle);
    }

    
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
    
}


void Tema1::Update(float deltaTimeSeconds)
{
    


    auto camera = GetSceneCamera();

    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);
    visMatrix = VisualizationTransf2D(logicSpace, viewSpace);

    
    glm::vec3 transfPlayerPos = visMatrix * glm::vec3(player->getPosition().x, player->getPosition().y, 1);
    //player->updatePosition(glm::vec3(transfPlayerPos.x, transfPlayerPos.y, 0));
    

    //render world field

    glm::mat3 modelMatrix = visMatrix
        * transf2D::Scale(mapSize / primitiveSize, mapSize / primitiveSize)
        * transf2D::Translate(-primitiveSize / 2, -primitiveSize / 2);
    RenderMesh2D(meshes["square_full"], modelMatrix, glm::vec3(0, 1, 1));

    


    if (!gameOver) {
        //set camera on player
        camera->SetPosition(glm::vec3(transfPlayerPos.x - resolution.x / 2, transfPlayerPos.y - resolution.y / 2, 50));

        player->orientateCharacter(resolution.x / 2, resolution.y / 2);

        //render player
        player->render(this, visMatrix);



        //render bullets

        for (auto bullet : bullets) {
            bullet->render(this, visMatrix);
            float angle = bullet->getAngle();
            ((Bullet*)bullet)->incrementTraversedRange(bulletSpeed * deltaTimeSeconds);
            bullet->move(cos(-angle - RADIANS(90)) * bulletSpeed * deltaTimeSeconds,
                sin(-angle - RADIANS(90)) * bulletSpeed * deltaTimeSeconds);
        }
        deleteBulletsOutOfRange();
        deleteOutOfBoundsBullets();
        deleteObstructedBullets();

        //Add a new enemy every 2 seconds
        if (Engine::GetElapsedTime() - spawnTime > 2.f) {
            int size = mapSize / 4;
            int sgn = rand() % 2;
            if (!sgn) sgn--;
            int pos = sgn * rand() % (size - (int)primitiveSize) + sgn * size;
            Character* enemy = new Enemy(glm::vec3(pos, pos, 1), primitiveSize);
            enemies.push_back(enemy);
            spawnTime = Engine::GetElapsedTime();
        }      

        //render obstacles
        for (auto obstacle : obstacles) {
            obstacle->render(this, visMatrix);
        }

        //make enemies follow player 

        for (auto enemy : enemies) {
            enemy->changeOrientation(player->getPosition().x, player->getPosition().y);
            enemy->orientateCharacter(enemy->getPosition().x, enemy->getPosition().y);
            float angle = enemy->getAngle();
            enemy->move(cos(-angle - RADIANS(90)) * playerSpeed / 2 * deltaTimeSeconds,
                sin(-angle - RADIANS(90)) * playerSpeed / 2 * deltaTimeSeconds);

            enemy->render(this, visMatrix);
        }
    }
    else {
        //Render play again button
        camera->SetPosition(glm::vec3(-resolution.x / 2, -resolution.y / 2, 50));
        RenderMesh2D(meshes["square_full"], transf2D::Scale(10, 10) *
            transf2D::Translate(-primitiveSize / 2, -primitiveSize / 2) *
            glm::mat3(1) , glm::vec3(1, 0, 0));
        RenderMesh2D(meshes["triangle_empty"],  transf2D::Scale(5, 5) *   
                                                transf2D::Rotate(RADIANS(45)) *
                                                transf2D::Translate(-primitiveSize / 2, - primitiveSize / 2) *
                                                glm::mat3(1), glm::vec3(0, 1, 0));

    }
    
  

    //eliminate enemies
    eliminateEnemies();
    checkIfDamaged();

    //Render healthBar
    float diagX = player->getPosition().x + 400;
    float diagY = player->getPosition().y + 300;
    RenderMesh2D(meshes["square_full"], visMatrix * transf2D::Translate(diagX, diagY) * transf2D::Scale(healthBar, 1), glm::vec3(1, 0, 0));


   
    
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    movePlayer(deltaTime);
}

void Tema1::movePlayer(float deltaTime) {
    if (gameOver) {
        return;
    }
    glm::vec3 playerCenter =  player->getCenter();
    float radius = player->getRadius();

    if (window->KeyHold(GLFW_KEY_D) && playerCenter.x + radius + playerSpeed * deltaTime < mapSize / 2 &&
        !checkPlayerObstacle(playerSpeed * deltaTime, 0)) {
            player->move(playerSpeed * deltaTime, 0);   
    }
    else  if (window->KeyHold(GLFW_KEY_A) && playerCenter.x - radius - playerSpeed * deltaTime > -mapSize / 2 &&
        !checkPlayerObstacle(- playerSpeed * deltaTime, 0)) {
            player->move(-playerSpeed * deltaTime, 0);
    }
    else  if (window->KeyHold(GLFW_KEY_S) && playerCenter.y - radius - playerSpeed * deltaTime > -mapSize / 2 &&
        !checkPlayerObstacle(0,- playerSpeed * deltaTime)) {
        player->move(0, -playerSpeed * deltaTime);
    }
    else  if (window->KeyHold(GLFW_KEY_W) && playerCenter.y + radius + playerSpeed * deltaTime < mapSize / 2 &&
        !checkPlayerObstacle(0, playerSpeed * deltaTime)) {
        player->move(0, playerSpeed * deltaTime);
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    player->changeOrientation(mouseX, window->GetResolution().y - mouseY);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (!gameOver) {
        // Create Bullet
        if (Engine::GetElapsedTime() - lastShot > fireRate) {
            bullets.push_back(new Bullet(glm::vec3(player->getPosition().x - primitiveSize / 2,
                player->getPosition().y - primitiveSize / 2, 1), primitiveSize, player->getAngle()));
            lastShot = Engine::GetElapsedTime();
        }
        
    }
   

    //Reset Game
    if (gameOver) {
        mouseY = window->GetResolution().y - mouseY;
        glm::vec3 center = glm::vec3(primitiveSize / 2, primitiveSize / 2, 1);
        center = visMatrix*
            transf2D::Scale(10, 10)*
            transf2D::Translate(-primitiveSize / 2, -primitiveSize / 2)*
            center;
        center = transf2D::Translate(window->GetResolution().x / 2, window->GetResolution().y / 2) * center;
        float len = (visMatrix * glm::vec3(primitiveSize * 10, primitiveSize * 10, 1)).x;
        if (mouseX >= center.x - len / 2 && mouseX <= center.x + len / 2 &&
            mouseY <= center.y + len / 2 && mouseY >= center.y - len / 2) {
            this->Init();
        }
    }
    
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

void Tema1::deleteOutOfBoundsBullets() {
    vector<Character*>::iterator it = bullets.begin();
    glm::vec3 map_top_corner = glm::vec3(mapSize / 2, mapSize / 2, 1);
    while (it != bullets.end()) {
        if (((Bullet*)(*it))->checkOutOfBounds(-map_top_corner.x, map_top_corner.x, map_top_corner.y, -map_top_corner.y)) {
            it = bullets.erase(it);
        }
        else {
            it++;
        }
        
    }
}

bool Tema1::checkPlayerObstacle(float pX, float pY) {
    player->move(pX, pY);
    for (auto obstacle : obstacles) {
        if (checkCircleRectCollision(player, obstacle)) {
            player->move(-pX, -pY);
            return 1;
        }
    }
    player->move(-pX, -pY);
    return 0;
}


void Tema1::deleteObstructedBullets() {
    vector<Character*>::iterator it = bullets.begin();
    while (it != bullets.end()) {
        int ok = 0;
        for (auto obstacle : obstacles) {
            if (checkCircleRectCollision(*it, obstacle)) {
                it = bullets.erase(it);
                ok = 1;
                break;
            }
        }
        if (!ok) {
            it++;
        }
    }
}

bool Tema1::checkCircleCollision(Character* ch1, Character* ch2) {
    glm::vec3 ch1Center = ch1->getCenter();
    float ch1Radius = ch1->getRadius();

    glm::vec3 ch2Center = ch2->getCenter();
    float ch2Radius = ch2->getRadius();

    if (sqrt((ch1Center.x - ch2Center.x) * (ch1Center.x - ch2Center.x) +
        (ch1Center.y - ch2Center.y) * (ch1Center.y - ch2Center.y)) <=
        ch2Radius + ch1Radius) {
        return 1;
    }
    
    return 0;
}

void Tema1::eliminateEnemies() {
    if (bullets.size() == 0) {
        return;
    }
    vector<Character*>::iterator it = bullets.begin();
   
    while (it != bullets.end()) {
        int ok = 0;
        vector<Character*>::iterator it2 = enemies.begin();
        while (it2 != enemies.end()) {
            if (checkCircleCollision((*it2), (*it))) {
                it2 = enemies.erase(it2);
                ok = 1;
                score += 10;
                cout << "Score increased to: " + to_string(score) + "\n";
            }
            else {
                it2++;
            }
        }
        if (ok) {
            it = bullets.erase(it);
        }
        else {
            it++;
        }
    }
}

void Tema1::checkIfDamaged() {
    if (enemies.size() == 0) {
        return;
    }
    auto it = enemies.begin();
    while (it != enemies.end()) {
        if (checkCircleCollision(*it, player)) {
            healthBar--;
            it = enemies.erase(it);
            if (healthBar == 0) {
                gameOver = 1;
                //Display top scores
                if (top_scores.empty() || top_scores.size() < 3) {
                    cout << "Congrats! You made it to top 3! \n";
                    top_scores.push_back(score);
                    sort(top_scores.begin(), top_scores.end());
                } else {
                    if (top_scores[0] < score) {
                        cout << "Congrats! You made it to top 3! \n";
                        top_scores[0] = score;
                        sort(top_scores.begin(), top_scores.end());

                    }
                }
                cout << "Top Scores are:\n";
                for (int i = top_scores.size() - 1; i >= 0; i--) {
                    cout << top_scores.size() - i << ". " << top_scores[i] << "\n";
                }
            }
        }
        else {
            it++;
        }
    }
}

bool Tema1::checkCircleRectCollision(Character* obCirc, Character* obRect) {
    glm::vec3 dist = glm::vec3(0, 0, 1);
    dist.x = abs(obCirc->getCenter().x - obRect->getCenter().x);
    dist.y = abs(obCirc->getCenter().y - obRect->getCenter().y);
    float width = ((Obstacle*)obRect)->getWidth(visMatrix);
    float height = ((Obstacle*)obRect)->getHeight(visMatrix);
    if (dist.x > width / 2 + obCirc->getRadius()) {
        return 0;
    }

    if (dist.y > height / 2 + obCirc->getRadius()) {
        return 0;
    }

    if (dist.x <= width / 2) {
        return 1;
    }

    if (dist.y <= height / 2) {
        return 1;
    }
    
    float cornerDistance_sq = (dist.x - width / 2) *
        (dist.x - width / 2) +
        (dist.y - height / 2) *
        (dist.y - height / 2);

    return cornerDistance_sq <= obCirc->getRadius() * obCirc->getRadius();

    return 0;
}

void Tema1::deleteBulletsOutOfRange() {
    vector<Character*>::iterator it = bullets.begin();
    while (it != bullets.end()) {
        if (((Bullet*)*it)->getTraversedRange() > bulletRange) {
            it = bullets.erase(it);
        }
        else {
            it++;
        }
    }
}