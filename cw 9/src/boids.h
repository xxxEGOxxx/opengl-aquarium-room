/*#include <vector>
#include <cmath>
#include <glut.h>


//const int WIDTH = 500;
//const int HEIGHT = 500;
const int DEPTH = 600;
const int NUM_BOIDS = 100;
const float BOID_SPEED = 0.01f;
const float BOID_SIZE = 0.50f;

struct Boid
{
    float x, y, z;
    float dx, dy, dz;
};

std::vector<Boid> boids;

void init()
{
    for (int i = 0; i < NUM_BOIDS; i++)
    {
        Boid b;
        b.x = float(rand()) / RAND_MAX * 2 - 1;
        b.y = float(rand()) / RAND_MAX * 2 - 1;
        b.z = float(rand()) / RAND_MAX * 2 - 1;
        b.dx = float(rand()) / RAND_MAX * BOID_SPEED - BOID_SPEED / 2;
        b.dy = float(rand()) / RAND_MAX * BOID_SPEED - BOID_SPEED / 2;
        b.dz = float(rand()) / RAND_MAX * BOID_SPEED - BOID_SPEED / 2;
        boids.push_back(b);
    }
}

void updateBoids()
{
    for (int i = 0; i < NUM_BOIDS; i++)
    {
        Boid& b1 = boids[i];
        float avgX = 0;
        float avgY = 0;
        float avgZ = 0;
        int count = 0;
        for (int j = 0; j < NUM_BOIDS; j++)
        {
            if (i == j)
                continue;

            Boid& b2 = boids[j];
            float dx = b2.x - b1.x;
            float dy = b2.y - b1.y;
            float dz = b2.z - b1.z;
            float distance = sqrt(dx * dx + dy * dy + dz * dz);
            if (distance < BOID_SIZE)
            {
                b1.dx -= dx / NUM_BOIDS;
                b1.dy -= dy / NUM_BOIDS;
                b1.dz -= dz / NUM_BOIDS;
            }
            avgX += b2.dx;
            avgY += b2.dy;
            avgZ += b2.dz;
            count++;
        }
        avgX /= count;
        avgY /= count;
        avgZ /= count;
        b1.dx += avgX / NUM_BOIDS;
        b1.dy += avgY / NUM_BOIDS;
        b1.dz += avgZ / NUM_BOIDS;

        b1.x += b1.dx;
        b1.y += b1.dy;
        b1.z += b1.dz;

        if (b1.x < 0 || b1.x > 1)
            b1.dx *= -1;
        if (b1.y < 0 || b1.y > 1)
            b1.dy *= -1;
        if (b1.z < 0 || b1.z > 1)
            b1.dz *= -1;
    }
}


void renderBoids()
{
    for (int i = 0; i < NUM_BOIDS; i++)
    {
        glBegin(GL_POINTS);
        glVertex3f(boids[i].x, boids[i].y, boids[i].z);
        glEnd();
    }
}
/*
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
    updateBoids();
    renderBoids();
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, float(width) / height, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Boids");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    init();
    glutMainLoop();
    return 0;
}
*/