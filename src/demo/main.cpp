#ifdef __APPLE__
# define __gl3_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif

#include <nbody/Simulation.h>
#include <nbody/Integrator.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <GL/glut.h>

using namespace std;

// Globals.

// TODO: add timeStep, Integration method to save/load.
// Until then, next two lines are stubbed out

// static nbody::integrator_t integrator = nbody::Euler;
// static double timeStep = 1e-6; // time step of the integrator


static int numSteps = 1e4; // number of time steps to evaluate the simulator
static float scale = 1.0; // Size of the viewing box
static size_t currentFrame = 0; // Current frame
static const size_t totalFrame = 200; // Total number of frames
static nbody::Simulation *simulation;
static vector<vector<Vector3d>> positions; // Array storing position of bodies 
static vector<vector<Vector3d>> positions2;
volatile static bool onPositionList1 = true;
volatile static bool creating = false;
volatile static bool nextReady = false;
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene
static int isAnimate = 1; // Animated?
static int animationPeriod = 100; // Time interval between frames
static pthread_t first, second;

void *addSteps(void *) {
  nbody::Simulation *sim;
  sim = simulation;
  size_t nBodies = sim->numBodies();
  if (onPositionList1) {
    positions2.clear();
    for( size_t i = 0; i < totalFrame; ++i ) {
      //sim->saveRun();
      positions2.push_back( vector<Vector3d>{} );
      for( size_t j = 0; j < nBodies; ++j ) {
        positions2[i].push_back( sim->getPosition(j) );
      }
      sim->evolveSystem( numSteps ); 
    }
  } else {
    positions.clear();
    for( size_t i = 0; i < totalFrame; ++i ) {
      sim->saveRun();
      positions.push_back( vector<Vector3d>{} );
      for( size_t j = 0; j < nBodies; ++j ) {
        positions[i].push_back( sim->getPosition(j) );
      }
      sim->evolveSystem( numSteps ); 
    }
  }
  sim->saveRun(); 
  creating = false;
  nextReady = true;
  cout << "Finished creation\n";

  return NULL;
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   // Zoom scene    
   glOrtho(-scale, scale, -scale, scale, -scale, scale); 

   // Rotate scene
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);
   
   // Draw coordinate axes
   glColor3f(1.0, 0.0, 0.0); 
   glBegin(GL_LINES);
     glVertex3f(0.0,0.0,0.0);
     glVertex3f(0.5,0.0,0.0);
   glEnd();
   glColor3f(0.0, 1.0, 0.0); 
   glBegin(GL_LINES);
     glVertex3f(0.0,0.0,0.0);
     glVertex3f(0.0,0.5,0.0);
   glEnd();
   glColor3f(0.0, 0.0, 1.0); 
   glBegin(GL_LINES);
     glVertex3f(0.0,0.0,0.0);
     glVertex3f(0.0,0.0,0.5);
   glEnd();

   // Draw bodies
   glColor3f(1.0, 1.0, 0.0);
   if (onPositionList1) {
      cout << "In list 1\n";
      auto posIterator = positions[currentFrame].begin();
      while( posIterator != positions[currentFrame].end() ) {  
        glTranslatef((float)posIterator->x(),(float)posIterator->y() , (float)posIterator->z());
        glutSolidSphere(0.05,20,20);
        glTranslatef(-(float)posIterator->x(),-(float)posIterator->y() , -(float)posIterator->z());
        posIterator++;
      }
   } else {
      cout << "In list 2\n";
      auto posIterator = positions2[currentFrame].begin();
      while( posIterator != positions2[currentFrame].end() ) {  
        glTranslatef((float)posIterator->x(),(float)posIterator->y() , (float)posIterator->z());
        glutSolidSphere(0.05,20,20);
        glTranslatef(-(float)posIterator->x(),-(float)posIterator->y() , -(float)posIterator->z());
        posIterator++;
     }
   } 

   glutSwapBuffers();
}

// Timer function
void animate(int)
{
   if(isAnimate){
     ++currentFrame;
     cout << "Steps left: " << totalFrame - currentFrame << "\n";
     if( currentFrame == totalFrame && nextReady) {
        onPositionList1 = 1 - onPositionList1; //Flip bool
        nextReady = false;
        currentFrame = 0;
     }

     if (totalFrame - currentFrame < 75 && !creating && !nextReady) {
      creating = 1;
      cout << "Creating\n";
      if (onPositionList1) {
        cout << "Starting thread\n";
        pthread_create(&first, NULL, addSteps, NULL);
        //std::thread first (addSteps, positions2, simulation); 
      } else {
        pthread_create(&second, NULL, addSteps, NULL);
        //std::thread second (addSteps, positions, simulation);
      }
      //addSteps(positions, simulation);
     }
   }

   glutTimerFunc(animationPeriod, animate, 1);
   glutPostRedisplay();
}

// Initialization routine
void setup(void) 
{
   glClearColor(0.0, 0.0, 0.0, 0.0); 
   glEnable(GL_DEPTH_TEST); // Enable depth testing
   glOrtho(-scale, scale, -scale, scale, -scale, scale); 
   glutFullScreen();
}

// OpenGL window reshape routine
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine
void keyInput(unsigned char key, int , int )
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
	  case ' ': 
         if (isAnimate) isAnimate = 0;
		 else isAnimate = 1;
         glutPostRedisplay();
		 break;
      case 'x':
         Xangle += 5.0;
		 if (Xangle > 360.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
		 if (Xangle < 0.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
		 if (Yangle > 360.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Y':
         Yangle -= 5.0;
		 if (Yangle < 0.0) Yangle += 360.0;
         glutPostRedisplay();
         break;
      case 'z':
         Zangle += 5.0;
		 if (Zangle > 360.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Z':
         Zangle -= 5.0;
		 if (Zangle < 0.0) Zangle += 360.0;
         glutPostRedisplay();
         break;
      case 'r':
         Xangle=0;
         Yangle=0;
         Zangle=0;
         glutPostRedisplay();
         break;
      case '+':
         if(scale>0.1){
           scale -=0.1;
         }
         glutPostRedisplay();
         break;
      case '-':
         scale +=0.1;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry
void specialKeyInput(int key, int , int )
{
   if (key == GLUT_KEY_DOWN) animationPeriod += 5;
   if( key == GLUT_KEY_UP) if (animationPeriod > 5) animationPeriod -= 5;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
	    << "Press the up/down arrow keys to speed up/slow down animation." << endl
        << "Press the x, X, y, Y, z, Z keys to rotate the scene." << endl;
}

int main(int argc, char *argv[]) {
  // Calculate positions and store in positions array
  try {
    std::ifstream input{ "resources/nbody/binary-system-simple.txt" };
    nbody::Simulation sim{input};
    simulation = &sim;
    size_t nBodies = sim.numBodies();
    for( size_t i = 0; i < totalFrame; ++i ) {
      std::cout << "==EVOLUTION " << i + 1 << "\n";
      sim.saveRun();
      positions.push_back(vector<Vector3d>{});
      for( size_t j = 0; j < nBodies; ++j ) {
        positions[i].push_back( sim.getPosition(j) );
      }
      sim.evolveSystem( numSteps );      
    }
    //sim.saveRun();    
  } catch( const std::exception &e ) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  
  // Create graphics and animations
  printInteraction();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100); 
  glutCreateWindow("ballAndTorus.cpp"); 
  setup(); 
  glutDisplayFunc(drawScene); 
  glutReshapeFunc(resize);  
  glutKeyboardFunc(keyInput);
  glutSpecialFunc(specialKeyInput);
  glutTimerFunc(5, animate, 1);
  glutMainLoop(); 
}


