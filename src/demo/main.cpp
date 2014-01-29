#ifdef __APPLE__
# define __gl3_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif

#include <nbody/Simulation.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glut.h>

using namespace std;

// Globals.
static float scale = 1.0; // Size of the viewing box
static size_t currentFrame = 0; // Current frame
static const size_t totalFrame = 50; // Total number of frames
static vector<Vector3d> positions[totalFrame]; // Array storing position of bodies 
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene
static int isAnimate = 1; // Animated?
static int animationPeriod = 100; // Time interval between frames

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
   auto posIterator = positions[currentFrame].begin();
   while(posIterator != positions[currentFrame].end() ) 
   {	
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
   if(isAnimate){
     ++currentFrame;
     if( currentFrame >= totalFrame ) currentFrame = 0;
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
    size_t nBodies = sim.numBodies();
    for( size_t i = 0; i < totalFrame; ++i ) {
      std::cout << "==EVOLUTION " << i + 1 << "\n";
      sim.saveRun();
      for( size_t j = 0; j < nBodies; ++j ) {
        positions[i].push_back( sim.getPosition(j) );
      }
      sim.evolveSystem( 1e4, 0.000001 );      
    }
    sim.saveRun();    
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


