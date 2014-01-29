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

// Globals

// TODO: add timeStep, Integration method to save/load.
// Until then, next two lines are stubbed out

// static nbody::integrator_t integrator = nbody::Euler;
// static double timeStep = 1e-6; // time step of the integrator


static int numSteps = 1e4; // number of time steps to evaluate the simulator
static size_t currentFrame = 0; // Current frame
static const size_t totalFrame = 5000; // Total number of frames

static nbody::Simulation *sim;
static vector<Vector3d> positions [totalFrame]; // Array storing position of bodies

volatile static bool EXITING = false; // Global exit flag

static pthread_t tStep;
volatile static size_t threadFrame = 0; // Frame that the thread is processing
static const size_t padding = 100; // #frames around currentFrame that shouldn't be touched by the threads

static bool showAxes = true; 
static float scale = 1.0; // Size of the viewing box
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene
static int isAnimate = 1; // Animated?
static int animationPeriod = 100; // Time interval between frames

void *addSteps(void *) {
  while( !EXITING ) {
    if( currentFrame > threadFrame+padding || currentFrame < threadFrame) {
      positions[threadFrame].clear();
      for( size_t i=0; i < sim->numBodies(); ++i ) {
        positions[threadFrame].push_back( sim->getPosition( i ) );
      }
      sim->evolveSystem( numSteps ); 
      sim->saveRun();
      threadFrame++;
      if( threadFrame >= totalFrame) threadFrame=0;
    }
  }
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
   
   if( showAxes ){
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
   }

   // Draw bodies
   glColor3f(1.0, 1.0, 0.0);
   auto posIterator = positions[currentFrame].begin();
   while( posIterator != positions[currentFrame].end() ) {  
     glTranslatef((float)posIterator->x(),(float)posIterator->y() , (float)posIterator->z());
     glutSolidSphere(0.05,20,20);
     glTranslatef(-(float)posIterator->x(),-(float)posIterator->y() , -(float)posIterator->z());
     posIterator++;
   } 

   glutSwapBuffers();
}

// Timer function
void animate(int)
{
   if( isAnimate && threadFrame-currentFrame > padding ) {
    ++currentFrame;
    if( currentFrame >= totalFrame ) {
      currentFrame=0;
    }
   }
   //cout << currentFrame << "	" << threadFrame << endl;
   glutTimerFunc( animationPeriod, animate, 1 );
   glutPostRedisplay();
}

// Initialization routine
void setup(void) 
{
   glClearColor(0.0, 0.0, 0.0, 0.0); 
   glEnable(GL_DEPTH_TEST); // Enable depth testing
   glOrtho( -scale, scale, -scale, scale, -scale, scale ); 
   //glutFullScreen();
}

// OpenGL window reshape routine
void resize( int w, int h )
{
   glViewport( 0, 0, (GLsizei)w, (GLsizei)h ); 
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();

   glMatrixMode( GL_MODELVIEW );
}

// Keyboard input processing routine
void keyInput( unsigned char key, int , int )
{
   switch( key ) 
   {
      case 27:
         EXITING = true;
         exit(0);
         break;
      case ' ': 
         isAnimate = !isAnimate;
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
      case 'w':
         showAxes=!showAxes;
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
void specialKeyInput( int key, int , int )
{
   if (key == GLUT_KEY_DOWN) animationPeriod += 5;
   if( key == GLUT_KEY_UP) if (animationPeriod > 5) animationPeriod -= 5;
   glutPostRedisplay();
}



int main(int argc, char *argv[]) {
  // Calculate positions and store in positions array
  try {
    ifstream input{ "resources/nbody/binary-system-simple.txt" };
    sim = new nbody::Simulation{input};
    
    //Fill in some starting frames
    for( ; threadFrame <= padding; ++threadFrame ) {
      for( size_t i=0; i < sim->numBodies(); ++i ) {
        positions[threadFrame].push_back( sim->getPosition( i ) );
      }
      sim->evolveSystem( numSteps );
      sim->saveRun();
    }
   
    //Create thread that keeps generating new frames
    pthread_create(&tStep, NULL, addSteps, NULL);  
  } catch( const exception &e ) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }  
  
  // Create graphics and animations
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100); 
  glutCreateWindow("N-Body Simulation"); 
  setup(); 
  glutDisplayFunc(drawScene); 
  glutReshapeFunc(resize);  
  glutKeyboardFunc(keyInput);
  glutSpecialFunc(specialKeyInput);
  glutTimerFunc(5, animate, 1);
  glutMainLoop(); 
   
  EXITING=true;
  return 0;
}


