#include<windows.h>
#include<math.h>
#include<vector>
#include<GL/glut.h>
#include<iostream>
#include<algorithm>
#define SPACE 32
using namespace std;

//spaceship object
struct spaceShip{
    double vx,vy;
    int x,y;
    double angle;
    vector<int> xcoords,ycoords;
};
struct bullet{
    double bx,by;
    int x,y,xcoords,ycoords;
};
spaceShip s;
vector<bullet> vecBullet;
bullet b;
//configurations
const int w = 640, h = 480;
int score,lives = 3;
bool gameOver = false,gameStart = false;


void init(int,int);

void initBg(){
     glClearColor(0.0f,0.0f,0.0f,1.0f);
}

//initialize spaceship coordinates
void initSpaceShip(){
    s.angle = 0.0f;
    s.vx = 0;
    s.vy = 0;
    s.x = w/2;
    s.y = h/2;



    s.xcoords.push_back(0+s.x);
    s.ycoords.push_back(10+s.y);
    s.xcoords.push_back(10+s.x);
    s.ycoords.push_back(-20+s.y);
    s.xcoords.push_back(-10+s.x);
    s.ycoords.push_back(-20+s.y);

}

int mx[3] = {0,10,-10};
int my[3] = {10,-20,-20};
double dx,dy;
int sx[3],sy[3];
void rotateShip(){
    for(int i=0;i<3;i++){
        dx = mx[i] * cos(s.angle) - my[i] * sin(s.angle);
        dy = mx[i] * sin(s.angle) + my[i] * cos(s.angle);
        sx[i] = floor(dx);
        sy[i] = floor(dy);
        s.xcoords[i] = s.x + sx[i];
        s.ycoords[i] = s.y + sy[i];
    }
}

void limitSpeed(){
    if(s.vx > 2.0 ){
        s.vx = 2.0;
    }
    if(s.vx< -2.0){
        s.vx = -2.0;
    }
    if(s.vy > 2.0){
        s.vy = 2.0;
    }
    if(s.vy < -2.0){
        s.vy = -2.0;
    }
}
void moveShip(){
    s.x += floor(s.vx);
    s.y += floor(s.vy);
    for(int i=0;i<3;i++){
        s.xcoords[i] = s.x + sx[i];
        s.ycoords[i] = s.y + sy[i];
    }
    limitSpeed();
}

void checkOutOfRange(){
    if(s.x < 0){
        s.x += w;
    }
    if(s.x >= w){
        s.x -= w;
    }
    if(s.y < 0){
        s.y += h;
    }
    if(s.y >= h){
        s.y -= h;
    }
}

void shootBullet(){
    for(int i=0;i<vecBullet.size();i++){
        vecBullet[i].x += vecBullet[i].bx;
        vecBullet[i].y += vecBullet[i].by;
     }
}

bool removBullets(bullet b){
   return (b.x < 1 || b.x >= w || b.y < 1 || b.y >= h);
}


void drawBullet(){
    if(vecBullet.size() > 0){
        for(int i=0;i<vecBullet.size();i++){
            glPushMatrix();
            glPointSize(5);
            glBegin(GL_POINTS);
            glColor3ub(255,255,255);
            glVertex2i(vecBullet[i].x,vecBullet[i].y);
            glEnd();
            glPopMatrix();

        }
        shootBullet();
        cout<<vecBullet.size()<<endl;
        auto it = remove_if(vecBullet.begin(),vecBullet.end(),removBullets);
        if(it != vecBullet.end()){
            vecBullet.erase(it);
        }
    }
}

void drawSpaceShip(){

    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    glColor3ub(255,255,255);
    glVertex2i(s.xcoords[0],s.ycoords[0]);
    glVertex2i(s.xcoords[1],s.ycoords[1]);
    glVertex2i(s.xcoords[2],s.ycoords[2]);
    glEnd();
    glPopMatrix();
    rotateShip();
    moveShip();
    checkOutOfRange();

}






void renderGame(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    if(!gameStart){
        initSpaceShip();
        drawSpaceShip();
    }
    else{
        drawSpaceShip();
        drawBullet();
    }

    glutSwapBuffers();

}

void keyInput(int key,int x,int y){
    if(key == GLUT_KEY_RIGHT){
        if(gameStart == true && gameOver == false){
            //rotate ship
            s.angle += 0.05;
        }
    }
    else if(key == GLUT_KEY_LEFT){
        if(gameStart == true && gameOver == false){
            //rotate ship
            s.angle -= 0.05;

        }
    }
    else if(key == GLUT_KEY_UP){
        if(gameStart == true && gameOver == false){
            //move ship
            s.vy += cos(s.angle) * 1.0;
            s.vx += -sin(s.angle) * 1.0;

        }
    }
    else if(key==GLUT_KEY_F1){
        gameStart = true;

    }
}

void gameKeys(unsigned char key,int x,int y){
    if(key == SPACE){
        if(gameStart == true && gameOver == false){
            cout<<"pressed"<<endl;
            bullet nb;
            nb.bx = -sin(s.angle) * 4.0;
            nb.by = cos(s.angle) * 4.0;
            nb.x = s.x;
            nb.y = s.y;
            vecBullet.push_back(nb);
            shootBullet();
        }
    }
}

void timer(int){

    glutPostRedisplay();
    glutTimerFunc(1000/30,timer,0);

}


int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(480,150);
    glutCreateWindow("Asteroids");
    glutDisplayFunc(renderGame);
    glutReshapeFunc(init);
    glutTimerFunc(0,timer,0);
    glutSpecialFunc(keyInput);
    glutKeyboardFunc(gameKeys);
    initBg();
    glutMainLoop();
}


void init(int wd,int hi){
    glViewport(0,0,(GLsizei)wd,(GLsizei)hi);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,640.0,0.0,480.0);
    glMatrixMode(GL_MODELVIEW);
}
