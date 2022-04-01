#include<windows.h>
#include<math.h>
#include<vector>
#include<GL/glut.h>
#include<iostream>
#include<time.h>
#include<algorithm>
#include<stdlib.h>
#define SPACE 32
using namespace std;

//spaceship object
struct spaceShip{
    double vx,vy,x,y,angle;
    vector<double> xcoords,ycoords;
};

//asteroids object
struct asteroid{
    int nVertices;
    bool isDestroyed = false;
    double vx,vy,x,y,aSize;
    vector<double> xcoords,ycoords;
};

struct bullet{
    bool isDestroyed = false;
    double bx,by,x,y;
};



spaceShip s;
vector<bullet> vecBullet;
vector<asteroid> asteroids;
//configurations
const int w = 640, h = 480;
double maxW,maxH;
int score,lives = 3;
bool gameOver = false,gameStart = false;
int upKey=0,downKey = 0,leftKey = 0,rightKey = 0;

void init(int,int);

double generateRandD(double fmin,double fmax){
    double f;
    f = fmin+(fmax-fmin)*(rand()%0x7fff)/32767.0;
    return f;
}


void initBg(){
     glClearColor(172/255.0f,172/255.0f,166/255.0f,1.0f);
}

double toRadians(double angle){
    return 3.1416/180 * angle;
}
double toDegrees(double rad){
    return 180/3.1416 * rad;
}

void translate2D(double x,double y){
    glTranslated(x,y,0.0);
}

void rotate2D(double angle){
    glRotated(toDegrees(angle),0.0,0.0,1.0);
}
//initialize spaceship coordinates
void initSpaceShip(){
    s.angle = 0.0;
    s.vx = 0.0;
    s.vy = 0.0;
    s.x = maxW/2.0;
    s.y = maxH/2.0;
    int scaleX =2;
    int scaleY = 3.5;
    s.xcoords.push_back(cos(toRadians(90))*scaleX);
    s.ycoords.push_back(sin(toRadians(90))*scaleY);
    s.xcoords.push_back(cos(toRadians(225))*scaleX);
    s.ycoords.push_back(sin(toRadians(225))*scaleY);
    s.xcoords.push_back(cos(toRadians(315))*scaleX);
    s.ycoords.push_back(sin(toRadians(315))*scaleY);

}





void initAsteroids(double xval,double yval,int numberOfAsteroids,double aSize){
    double theta,r,x,y;

    for(int i=0;i<numberOfAsteroids;i++){
        asteroid a;
        a.aSize = aSize;
        a.isDestroyed = false;
        a.x = xval;
        a.y = yval;
        a.vx = generateRandD(-0.8,0.8);
        a.vy = generateRandD(-0.8,0.8);
        a.nVertices = 6 + rand() % 5;
        for(int i=0;i<a.nVertices;i++){
            theta = 2.0*3.1416*i/a.nVertices;
            cout<<"theta :"<<theta<<endl;
            r = a.aSize * generateRandD(2.0,3.0);
            x = - r * sin(theta);
            y = r * cos(theta);
            a.xcoords.push_back(x);
            a.ycoords.push_back(y);
        }
        asteroids.push_back(a);
    }

}



void createRandomAsteroids(int noOfAsteroids){
    for(int i=0;i<noOfAsteroids;i++){
        if(generateRandD(-1,1)<0){
            initAsteroids(0,generateRandD(0.0,maxH),1,3.0);
        }
        else{
            initAsteroids(generateRandD(0,maxW),0,1,3.0);
        }
    }
}


//void asteroidOutOfBoundCheck(){
//    int x1,y1;
//    for(int i=0;i<asteroids.size();i++){
//            if(asteroids[i].x >= w){
//               asteroids[i].x -=  w;
//            }
//            if(asteroids[i].x <= 0){
//                asteroids[i].x +=  w;
//            }
//            if(asteroids[i].y >= h){
//                asteroids[i].y -= h;
//            }
//            if(asteroids[i].y <= 0){
//                asteroids[i].y += h;
//            }
//
//       }
//    //drawAsteroids();
//}



//int ax[11] = {0,-20,10,30,70,70,50,30,30,10,-20};
//int ay[11] = {10,20,50,40,50,-10,-30,-20,5,-40,0};
//void moveAsteroids(){
//    int nx,ny;
//    asteroid a1;
//    for(int i=0;i<asteroids.size();i++){
//       asteroids[i].x  += floor(asteroids[i].vx);
//       asteroids[i].y  += floor(asteroids[i].vy);
//       for(int j=0;j<12;j++){
//            asteroids[i].xcoords[j] = ax[j] + asteroids[i].x;
//            asteroids[i].ycoords[j] = ay[j] + asteroids[i].y;
//       }
//    }
//
//}

void drawAsteroids(asteroid* a){
    glColor3ub(100,100,100);
    glBegin(GL_POLYGON);
    for(int j=0;j<a->nVertices;j++){
        glVertex2d(a->xcoords[j],a->ycoords[j]);
    }
    glEnd();

}






//int mx[3] = {0,10,-10};
//int my[3] = {10,-20,-20};
//double dx,dy;
//int sx[3],sy[3];
//void rotateShip(){
//    for(int i=0;i<3;i++){
//        dx = mx[i] * cos(s.angle) - my[i] * sin(s.angle);
//        dy = mx[i] * sin(s.angle) + my[i] * cos(s.angle);
//        sx[i] = floor(dx);
//        sy[i] = floor(dy);
//        s.xcoords[i] = s.x + sx[i];
//        s.ycoords[i] = s.y + sy[i];
//    }
//}

//void limitSpeed(){
//    if(s.vx > 2.0 ){
//        s.vx = 2.0;
//    }
//    if(s.vx< -2.0){
//        s.vx = -2.0;
//    }
//    if(s.vy > 2.0){
//        s.vy = 2.0;
//    }
//    if(s.vy < -2.0){
//        s.vy = -2.0;
//    }
//}
void accelerateShip(){
    double acc = 0.1;
    if(pow((s.vx - acc * sin(toRadians(s.angle))),2)
       + pow((s.vy + acc * cos(toRadians(s.angle))),2) < 1.0){
            s.vx += acc * sin(toRadians(s.angle));
            s.vy += acc * cos(toRadians(s.angle));
       }
    else{
        if((s.vx-acc * sin(toRadians(s.angle)) >= s.vx &&
            s.vy+acc * cos(toRadians(s.angle)) >= s.vy) ||
           (pow((s.vx - acc * sin(toRadians(s.angle))),2)+ pow((s.vy + acc * cos(toRadians(s.angle))),2) >= 1.0)){
                s.vx = -1.0 * sin(toRadians(s.angle));
                s.vy = 1.0 * cos(toRadians(s.angle));
           }
        else if(s.vx-acc * sin(toRadians(s.angle)) >= s.vx &&
                s.vy+acc * cos(toRadians(s.angle)) < s.vy){
                s.vx = -1.0 * sin(toRadians(s.angle));
                s.vy += acc * cos(toRadians(s.angle));
          }
        else if(s.vx-acc * sin(toRadians(s.angle)) < s.vx &&
                s.vy+acc * cos(toRadians(s.angle)) >= s.vy){
                s.vx += acc * sin(toRadians(s.angle));
                s.vy = 1.0 * cos(toRadians(s.angle));
        }
        else{
            s.vx += acc * sin(toRadians(s.angle));
            s.vy += acc * cos(toRadians(s.angle));
        }
    }
}
//
//void checkOutOfRange(){
//    if(s.x < 0){
//        s.x += w;
//    }
//    if(s.x >= w){
//        s.x -= w;
//    }
//    if(s.y < 0){
//        s.y += h;
//    }
//    if(s.y >= h){
//        s.y -= h;
//    }
//}

int detectCollisionAB(bullet* b,asteroid* a){
    double x1,y1,x2,y2,bx,by,xi;
    int intersect_no = 0;
    int lines = a->nVertices;
    bx = b->x;
    by = b->y;
    for(int j=0;j<a->nVertices;j++){
            x1 =(double) a->x + a->xcoords[j%(lines)];
            y1 =(double) a->y + a->ycoords[j%(lines)];
            x2 =(double) a->x + a->xcoords[(j+1)%(lines)];
            y2 =(double) a->y + a->ycoords[(j+1)%(lines)];
            if((by <= y1 && by >= y2) || (by >= y1 && by <= y2)){
                xi = (((by-y1)/(y2-y1))*x2) + (((y2-by)/(y2-y1))*x1);
                if((xi>=bx) &&(((xi<=x1)&&(xi>=x2))||((xi>=x1)&&(xi<=x2)))){
                        intersect_no++;
                }
            }
    }
    return intersect_no % 2;
}



//void shootBullet(){
//    for(int i=0;i<vecBullet.size();i++){
//        vecBullet[i].x += floor(vecBullet[i].bx);
//        vecBullet[i].y += floor(vecBullet[i].by);
//     }
//
//
//}

bool removBullets(bullet b){
   return b.isDestroyed;
}
bool removeAsteroid(asteroid a){
    return a.isDestroyed;
}

void drawBullet(bullet* b){
//    if(vecBullet.size() > 0){
//        //cout<<"bullet :"<<vecBullet[0].x<<","<<vecBullet[0].y<<endl;
//        for(int i=0;i<vecBullet.size();i++){
//            glPushMatrix();
//            glPointSize(5);
//            glBegin(GL_POINTS);
//            glColor3ub(255,255,255);
//            glVertex2i(vecBullet[i].x,vecBullet[i].y);
//            glEnd();
//            glPopMatrix();
//
//        }
//        shootBullet();
//        auto it = remove_if(vecBullet.begin(),vecBullet.end(),removBullets);
//        if(it != vecBullet.end()){
//            vecBullet.erase(it);
//        }
//    }
    glColor3ub(255,255,255);
    glBegin(GL_POINTS);
    glVertex2d(b->x,b->y);
    glEnd();
}



void drawSpaceShip(spaceShip* s){
    glColor3ub(255,255,255);
    glBegin(GL_TRIANGLES);
    glVertex2d(s->xcoords[0],s->ycoords[0]);
    glVertex2d(s->xcoords[1],s->ycoords[1]);
    glVertex2d(s->xcoords[2],s->ycoords[2]);
    glEnd();

}

void renderGame(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(4.0);
    glLoadIdentity();
    translate2D(s.x,s.y);
    rotate2D(toRadians(s.angle));
    drawSpaceShip(&s);
    for(int i=0;i<asteroids.size();i++){
        if(!asteroids[i].isDestroyed){
            glLoadIdentity();
            translate2D(asteroids[i].x,asteroids[i].y);
            drawAsteroids(&asteroids[i]);
        }

    }
    for(int i=0;i<vecBullet.size();i++){
        if(!vecBullet[i].isDestroyed){
            glLoadIdentity();
            drawBullet(&vecBullet[i]);
        }
    }
    glutSwapBuffers();
}


void initGame(){
    createRandomAsteroids(1);
    initSpaceShip();
}

//void renderGame(){
//
//    glClear(GL_COLOR_BUFFER_BIT);
//    glLoadIdentity();
//    if(!gameStart){
//        initSpaceShip();
//        createRandomAsteroids(1);
//        drawSpaceShip();
//    }
//    else{
//
//        drawSpaceShip();
//        drawBullet();
//        for(int i=0;i<asteroids.size();i++){
//            glLoadIdentity();
//            translate2D(asteroids[i].x,asteroids[i].y);
//            drawAsteroids(&asteroids[i]);
//        }
//        //moveAsteroids();
//        //asteroidOutOfBoundCheck();
//
//
//    }
////    for(int i =0;i<asteroids.size();i++){
////            for(int j=0;j<vecBullet.size();j++){
////                if(detectCollisionAB(vecBullet[i],asteroids[j])){
////                    cout<<"hit :"<<asteroids[i].x<<" "<<asteroids[i].y<<endl;
////                }
////            }
////    }
//    glutSwapBuffers();
//
//}

//void timer(int){
//
//    glutPostRedisplay();
//    glutTimerFunc(33,timer,0);
//
//}
//

void keyInput(int key,int x,int y){
    if(key == GLUT_KEY_RIGHT){
        rightKey = 1;
    }
    else if(key == GLUT_KEY_LEFT){
        leftKey = 1;
    }
    else if(key == GLUT_KEY_UP){
        upKey = 1;
    }
    else if(key == GLUT_KEY_F1){
        if(gameStart == false && gameOver == false){
            gameStart = true;
            //glutTimerFunc(33,timer,0);
        }

    }
}

void releseInput(int key,int x,int y){
     if(key == GLUT_KEY_RIGHT){
        rightKey = 0;
    }
    else if(key == GLUT_KEY_LEFT){
        leftKey = 0;
    }
    else if(key == GLUT_KEY_UP){
        upKey = 0;
    }
}

void gameKeys(unsigned char key,int x,int y){
    if(key == SPACE){
//        if(gameStart == true && gameOver == false){
            bullet nb;
            nb.isDestroyed = false;
            nb.x = s.x;
            nb.y = s.y;
            nb.bx = -5 * sin(toRadians(s.angle));
            nb.by = 5*cos(toRadians(s.angle));
            vecBullet.push_back(nb);
//            shootBullet();
//        }
    }
}

void gameTimer(int){

    if(leftKey == 1){
        s.angle += 10;
    }
    if(rightKey == 1){
        s.angle -= 10;
    }
    if(upKey == 1){
        accelerateShip();
    }
    if(s.x > maxW){
        s.x -= maxW;
    }
    if(s.x < 0){
        s.x += maxW;
    }
    if(s.y > maxH){
        s.y -= maxH;
    }
    if(s.y < 0){
        s.y += maxH;
    }
    s.x += s.vx;
    s.y += s.vy;
    for(int i=0;i<asteroids.size();i++){
        if(asteroids[i].isDestroyed == false){
            asteroids[i].x += asteroids[i].vx;
            asteroids[i].y += asteroids[i].vy;
            if(asteroids[i].x > maxW){
                asteroids[i].x -= maxW;
            }
            if(asteroids[i].x < 0){
                asteroids[i].x += maxW;
            }
            if(asteroids[i].y > maxH){
                asteroids[i].y -= maxH;
            }
            if(asteroids[i].y < 0){
                asteroids[i].y += maxH;
            }
        }
    }

    for(int i=0;i<vecBullet.size();i++){
        if(!vecBullet[i].isDestroyed){
            vecBullet[i].x += vecBullet[i].bx;
            vecBullet[i].y += vecBullet[i].by;

            if(vecBullet[i].x > maxW || vecBullet[i].y < 0 ||
               vecBullet[i].y > maxH || vecBullet[i].y < 0
               )
            {

                vecBullet[i].isDestroyed = true;
            }
        }
    }

    for(int i=0;i<vecBullet.size();i++){
        if(vecBullet[i].isDestroyed == false){
            for(int j=0;j<asteroids.size();j++){
                if(asteroids[i].isDestroyed == false){
                    if(detectCollisionAB(&vecBullet[i],&asteroids[j])){
                        asteroids[j].isDestroyed = true;
                        vecBullet[i].isDestroyed = true;
                        if(asteroids[j].aSize == 3.0){
                            initAsteroids(asteroids[j].x,asteroids[j].y,1,2.0);
                            initAsteroids(asteroids[j].x,asteroids[j].y,1,2.0);
                        }
                        else if(asteroids[j].aSize == 2.0){
                            initAsteroids(asteroids[j].x,asteroids[j].y,1,1.5);
                            initAsteroids(asteroids[j].x,asteroids[j].y,1,1.5);
                        }
                        break;
                    }
                }
            }
        }
    }

    auto it = remove_if(vecBullet.begin(),vecBullet.end(),removBullets);
    if(it != vecBullet.end()){
        vecBullet.erase(it);
    }
    auto it1 = remove_if(asteroids.begin(),asteroids.end(),removeAsteroid);
    if(it1 != asteroids.end()){
        asteroids.erase(it1);
    }
    glutPostRedisplay();
    glutTimerFunc(33,gameTimer,0);
}

void startMenuTimer(int){
    glutPostRedisplay();
    if(gameStart == false && gameOver == false){
        glutTimerFunc(33,startMenuTimer,0);
    }
    else{
        glutTimerFunc(33,gameTimer,0);
        glutDisplayFunc(renderGame);
        initGame();
    }
}


void menu(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glutSwapBuffers();

}

int main(int argc,char** argv){
    srand((unsigned int)time(NULL));
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(480,150);
    glutCreateWindow("Asteroids");
    glutDisplayFunc(menu);
    glutReshapeFunc(init);
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(gameKeys);
    glutSpecialFunc(keyInput);
    glutSpecialUpFunc(releseInput);
    glutTimerFunc(33,startMenuTimer,0);
    initBg();
    glutMainLoop();
    return 0;
}


void init(int wd,int hi){
    maxW = 100.0*wd/hi;
    maxH = 100.0;
    glViewport(0,0,wd,hi);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,maxW,0.0,maxH,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
}
