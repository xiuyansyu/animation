/***********************************************************
             CSC418, Winter 2016

	     Assignment 3
		Student Name: Xiuyan Yu, Chu Xi Huang
		Student ID:  1000756719, 998912262
 
	     Modified from:
                 penguin.cpp
                 author: Mike Pratscher
                 based on code by: Eron Steger, J. Radulovich

		Main source file for assignment 2
		Uses OpenGL, GLUT and GLUI libraries
 
COMPLETED:
obtained model from http://www.turbosquid.com/3d-models/free-max-mode-baymax-big-hero-6/881191
OBJ file loader added to dir objloader.cpp objloader.h
parsed into 7 body parts and rendered parts separately
added articulation of parts
added DoF light and camera movement
cubic interpolation
Skybox


***********************************************************/

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glui.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector.h>
#include <math.h>

#include "keyframe.h"
#include "timer.h"
#include "vector.h"
#include "objloader.h"

// *************** GLOBAL VARIABLES *************************

// const float light_source_plane_Z = 30;

const float radius = 500;	// distance of light source from the origin on the xy plane

const float PI = 3.14159;

const float SPINNER_SPEED = 0.1;


// --------------- OBJECT VARIABLES AND DRAWING FUNCTIONS -----------------

void loadAll();
void drawBaymax();
void drawChestBelly();
void drawHead();
void drawLeftLeg();
void drawLeftArm();
void drawRightArm();
void drawRightLeg();
void drawStamp();
void drawGround();
std::vector < Vertex > chestbelly_vertices;
std::vector < UV > chestbelly_uvs;
std::vector < Normal > chestbelly_normals;
std::vector < Vertex > head_vertices;
std::vector < UV > head_uvs;
std::vector < Normal > head_normals;
std::vector < Vertex > leftarm_vertices;
std::vector < UV > leftarm_uvs;
std::vector < Normal > leftarm_normals;
std::vector < Vertex > leftleg_vertices;
std::vector < UV > leftleg_uvs;
std::vector < Normal > leftleg_normals;
std::vector < Vertex > rightarm_vertices;
std::vector < UV > rightarm_uvs;
std::vector < Normal > rightarm_normals;
std::vector < Vertex > rightleg_vertices;
std::vector < UV > rightleg_uvs;
std::vector < Normal > rightleg_normals;
std::vector < Vertex > stamp_vertices;
std::vector < UV > stamp_uvs;
std::vector < Normal > stamp_normals;


// --------------- USER INTERFACE VARIABLES -----------------

// Window settings
int windowID;				// Glut window ID (for display)
int Win[2];					// window (x,y) size

GLUI* glui_joints;			// Glui window with joint controls
GLUI* glui_keyframe;		// Glui window with keyframe controls
GLUI* glui_render;			// Glui window for render style
GLUI* glui_lightsource;			// Glui window for light source control

char msg[256];				// String used for status message
GLUI_StaticText* status;	// Status message ("Status: <msg>")


// ---------------- ANIMATION VARIABLES ---------------------

// Outline variable
bool outline = false;

// Camera settings
bool updateCamZPos = false;
int  lastX = 0;
int  lastY = 0;
const float ZOOM_SCALE = 0.01;

GLdouble camXPos =  0.0;
GLdouble camYPos =  0.0;
GLdouble camZPos = -7.5;

const GLdouble CAMERA_FOVY = 60.0;
const GLdouble NEAR_CLIP   = 0.1;
const GLdouble FAR_CLIP    = 1000.0;

// Render settings
enum { WIREFRAME, SOLID, OUTLINED, METALLIC, MATTE };	// README: the different render styles
int renderStyle = MATTE;			// README: the selected render style

// Animation settings
int animate_mode = 0;			// 0 = no anim, 1 = animate

// Keyframe settings
const char filenameKF[] = "keyframes.txt";	// file for loading / saving keyframes

Keyframe* keyframes;			// list of keyframes

int maxValidKeyframe   = 0;		// index of max VALID keyframe (in keyframe list)
const int KEYFRAME_MIN = 0;
const int KEYFRAME_MAX = 32;	// README: specifies the max number of keyframes

// Frame settings
char filenameF[128];			// storage for frame filename

int frameNumber = 0;			// current frame being dumped
int frameToFile = 0;			// flag for dumping frames to file

const float DUMP_FRAME_PER_SEC = 24.0;		// frame rate for dumped frames
const float DUMP_SEC_PER_FRAME = 1.0 / DUMP_FRAME_PER_SEC;

// Time settings
Timer* animationTimer;
Timer* frameRateTimer;

const float TIME_MIN = 0.0;
const float TIME_MAX = 10.0;	// README: specifies the max time of the animation
const float SEC_PER_FRAME = 1.0 / 60.0;

// Joint settings

// README: This is the key data structure for
// updating keyframes in the keyframe list and
// for driving the animation.
//   i) When updating a keyframe, use the values
//      in this data structure to update the
//      appropriate keyframe in the keyframe list.
//  ii) When calculating the interpolated pose,
//      the resulting pose vector is placed into
//      this data structure. (This code is already
//      in place - see the animate() function)
// iii) When drawing the scene, use the values in
//      this data structure (which are set in the
//      animate() function as described above) to
//      specify the appropriate transformations.
Keyframe* joint_ui_data;

// README: To change the range of a particular DOF,
// simply change the appropriate min/max values below
const float ROOT_TRANSLATE_X_MIN = -20.0;
const float ROOT_TRANSLATE_X_MAX =  20.0;
const float ROOT_TRANSLATE_Y_MIN = -20.0;
const float ROOT_TRANSLATE_Y_MAX =  20.0;
const float ROOT_TRANSLATE_Z_MIN = -20.0;
const float ROOT_TRANSLATE_Z_MAX =  20.0;
const float ROOT_ROTATE_X_MIN    = -180.0;
const float ROOT_ROTATE_X_MAX    =  180.0;
const float ROOT_ROTATE_Y_MIN    = -180.0;
const float ROOT_ROTATE_Y_MAX    =  180.0;
const float ROOT_ROTATE_Z_MIN    = -180.0;
const float ROOT_ROTATE_Z_MAX    =  180.0;
const float BODY_TRANSLATE_X_MIN = -20.0;
const float BODY_TRANSLATE_X_MAX =  20.0;
const float BODY_TRANSLATE_Y_MIN = -20.0;
const float BODY_TRANSLATE_Y_MAX =  20.0;
const float BODY_TRANSLATE_Z_MIN = -20.0;
const float BODY_TRANSLATE_Z_MAX =  20.0;
const float BODY_ROTATE_X_MIN    = -180.0;
const float BODY_ROTATE_X_MAX    =  180.0;
const float BODY_ROTATE_Y_MIN    = -180.0;
const float BODY_ROTATE_Y_MAX    =  180.0;
const float BODY_ROTATE_Z_MIN    = -180.0;
const float BODY_ROTATE_Z_MAX    =  180.0;
const float HEAD_MIN             = -180.0;
const float HEAD_MAX             =  180.0;
const float SHOULDER_PITCH_MIN   = -180;
const float SHOULDER_PITCH_MAX   =  180;
const float SHOULDER_YAW_MIN     = -180.0;
const float SHOULDER_YAW_MAX     =  180.0;
const float SHOULDER_ROLL_MIN    = -180.0;
const float SHOULDER_ROLL_MAX    =  180.0;
const float HIP_PITCH_MIN        = -180.0;
const float HIP_PITCH_MAX        =  180.0;
const float HIP_YAW_MIN          = -180.0;
const float HIP_YAW_MAX          =  180.0;
const float HIP_ROLL_MIN         = -180.0;
const float HIP_ROLL_MAX         =  180.0;


// Light source settings: Choose a circle of fixed radius, on plane z = something
GLfloat light_pos[] = {radius*cos(2.5),radius*sin(2.5),40,1};
GLfloat LIGHT_SOURCE_ANGLE = 2.5;
const float LIGHT_SOURCE_MIN = -1*PI;
const float LIGHT_SOURCE_MAX =  PI;

const float LIGHT_SOURCE_Z_MIN = -200;
const float LIGHT_SOURCE_Z_MAX =  200;

// Skybox
enum {SKY_LEFT=0,SKY_BACK,SKY_RIGHT,SKY_FRONT,SKY_TOP,SKY_BOTTOM};      //constants for the skybox faces, so we don't have to remember so much number
unsigned int skybox[6]; //the ids for the textures

// ***********  FUNCTION HEADER DECLARATIONS ****************


// Initialization functions
void initDS();
void initGlut(int argc, char** argv);
void initGlui();
void initGl();


// Callbacks for handling events in glut
void reshape(int w, int h);
void animate();
void display(void);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);


// Functions to help draw the object
Vector getInterpolatedJointDOFS(float time);

void drawBody();
void drawHead();
void drawBeak();
void drawChin();
void drawPenguin();
void drawLeg();
void drawFoot();
void drawWing();
void drawHand();
void rotateCamera();
void rotateBaymax();

// Skybox functions
void initskybox();
void killskybox();
void Draw_Skybox(float x, float y, float z, float width, float height, float length);
void drawSkybox(float size);
void drawCube(float size);
GLuint loadBMP_custom(const char * imagepath);

// Image functions
void writeFrame(char* filename, bool pgm, bool frontBuffer);


// ******************** FUNCTIONS ************************

// Skybox functions
//load all of the textures, to the skybox array
void initskybox()
{
    skybox[SKY_LEFT]=loadBMP_custom("data/left.bmp");
    skybox[SKY_BACK]=loadBMP_custom("data/back.bmp");
    skybox[SKY_RIGHT]=loadBMP_custom("data/right.bmp");
    skybox[SKY_FRONT]=loadBMP_custom("data/front.bmp");
    skybox[SKY_TOP]=loadBMP_custom("data/top.bmp");
    skybox[SKY_BOTTOM]=loadBMP_custom("data/bottom.bmp");
}
//delete all of the textures from the skybox array (to avoid memory leaks)
void killskybox()
{
    glDeleteTextures(6,&skybox[0]);
}


void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
    
    // Center the Skybox around the given x,y,z position
    x = x - width  / 2;
    y = y - height / 2;
    z = z - length / 2;
    
    bool b1=glIsEnabled(GL_TEXTURE_2D);     //new, we left the textures turned on, if it was turned on
    glDisable(GL_LIGHTING); //turn off lighting, when making the skybox
    glDisable(GL_DEPTH_TEST);       //turn off depth texting
    glEnable(GL_TEXTURE_2D);        //and turn on texturing
    
    // Draw Front side
    glBindTexture(GL_TEXTURE_2D, skybox[SKY_FRONT]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
    glEnd();
    
    // Draw Back side
    glBindTexture(GL_TEXTURE_2D, skybox[SKY_BACK]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
    glEnd();
    
    // Draw Left side
    glBindTexture(GL_TEXTURE_2D,skybox[SKY_LEFT]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);
    glEnd();
    
    // Draw Right side
    glBindTexture(GL_TEXTURE_2D, skybox[SKY_RIGHT]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
    glEnd();
    
    // Draw Up side
    glBindTexture(GL_TEXTURE_2D, skybox[SKY_TOP]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
    glEnd();
    
    // Draw Down side
    glBindTexture(GL_TEXTURE_2D, skybox[SKY_BOTTOM]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
    glEnd();
    
    glEnable(GL_LIGHTING);  //turn everything back, which we turned on, and turn everything off, which we have turned on.
    glEnable(GL_DEPTH_TEST);
    if(!b1)
        glDisable(GL_TEXTURE_2D);

    
}



void drawSkybox(float size)
{
    bool b1=glIsEnabled(GL_TEXTURE_2D);     //new, we left the textures turned on, if it was turned on
    glDisable(GL_LIGHTING); //turn off lighting, when making the skybox
    glDisable(GL_DEPTH_TEST);       //turn off depth texting
    glEnable(GL_TEXTURE_2D);        //and turn on texturing
    glBindTexture(GL_TEXTURE_2D,skybox[SKY_BACK]);  //use the texture we want
    glBegin(GL_QUADS);      //and draw a face
    //back face
    glTexCoord2f(0,0);      //use the correct texture coordinate
    glVertex3f(size/2,size/2,size/2);       //and a vertex
    glTexCoord2f(1,0);      //and repeat it...
    glVertex3f(-size/2,size/2,size/2);
    glTexCoord2f(1,1);
    glVertex3f(-size/2,-size/2,size/2);
    glTexCoord2f(0,1);
    glVertex3f(size/2,-size/2,size/2);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,skybox[SKY_LEFT]);
    glBegin(GL_QUADS);
    //left face
    glTexCoord2f(0,0);
    glVertex3f(-size/2,size/2,size/2);
    glTexCoord2f(1,0);
    glVertex3f(-size/2,size/2,-size/2);
    glTexCoord2f(1,1);
    glVertex3f(-size/2,-size/2,-size/2);
    glTexCoord2f(0,1);
    glVertex3f(-size/2,-size/2,size/2);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,skybox[SKY_FRONT]);
    glBegin(GL_QUADS);
    //front face
    glTexCoord2f(1,0);
    glVertex3f(size/2,size/2,-size/2);
    glTexCoord2f(0,0);
    glVertex3f(-size/2,size/2,-size/2);
    glTexCoord2f(0,1);
    glVertex3f(-size/2,-size/2,-size/2);
    glTexCoord2f(1,1);
    glVertex3f(size/2,-size/2,-size/2);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,skybox[SKY_RIGHT]);
    glBegin(GL_QUADS);
    //right face
    glTexCoord2f(0,0);
    glVertex3f(size/2,size/2,-size/2);
    glTexCoord2f(1,0);
    glVertex3f(size/2,size/2,size/2);
    glTexCoord2f(1,1);
    glVertex3f(size/2,-size/2,size/2);
    glTexCoord2f(0,1);
    glVertex3f(size/2,-size/2,-size/2);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,skybox[SKY_TOP]);
    glBegin(GL_QUADS);                      //top face
    glTexCoord2f(1,0);
    glVertex3f(size/2,size/2,size/2);
    glTexCoord2f(0,0);
    glVertex3f(-size/2,size/2,size/2);
    glTexCoord2f(0,1);
    glVertex3f(-size/2,size/2,-size/2);
    glTexCoord2f(1,1);
    glVertex3f(size/2,size/2,-size/2);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,skybox[SKY_BOTTOM]);
    glBegin(GL_QUADS);
    //bottom face
    glTexCoord2f(1,1);
    glVertex3f(size/2,-size/2,size/2);
    glTexCoord2f(0,1);
    glVertex3f(-size/2,-size/2,size/2);
    glTexCoord2f(0,0);
    glVertex3f(-size/2,-size/2,-size/2);
    glTexCoord2f(1,0);
    glVertex3f(size/2,-size/2,-size/2);
    glEnd();
    glEnable(GL_LIGHTING);  //turn everything back, which we turned on, and turn everything off, which we have turned on.
    glEnable(GL_DEPTH_TEST);
    if(!b1)
        glDisable(GL_TEXTURE_2D);
}


void drawCube(float size)
{
    float difamb[]={1.0,0.5,0.3,1.0};
    glBegin(GL_QUADS);
    //front face
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,difamb);
    glNormal3f(0.0,0.0,1.0);
    glVertex3f(size/2,size/2,size/2);
    glVertex3f(-size/2,size/2,size/2);
    glVertex3f(-size/2,-size/2,size/2);
    glVertex3f(size/2,-size/2,size/2);
    
    //left face
    glNormal3f(-1.0,0.0,0.0);
    glVertex3f(-size/2,size/2,size/2);
    glVertex3f(-size/2,size/2,-size/2);
    glVertex3f(-size/2,-size/2,-size/2);
    glVertex3f(-size/2,-size/2,size/2);
    
    //back face
    glNormal3f(0.0,0.0,-1.0);
    glVertex3f(size/2,size/2,-size/2);
    glVertex3f(-size/2,size/2,-size/2);
    glVertex3f(-size/2,-size/2,-size/2);
    glVertex3f(size/2,-size/2,-size/2);
    
    //right face
    glNormal3f(1.0,0.0,0.0);
    glVertex3f(size/2,size/2,-size/2);
    glVertex3f(size/2,size/2,size/2);
    glVertex3f(size/2,-size/2,size/2);
    glVertex3f(size/2,-size/2,-size/2);
    
    //top face
    glNormal3f(0.0,1.0,0.0);
    glVertex3f(size/2,size/2,size/2);
    glVertex3f(-size/2,size/2,size/2);
    glVertex3f(-size/2,size/2,-size/2);
    glVertex3f(size/2,size/2,-size/2);
    
    //bottom face
    glNormal3f(0.0,-1.0,0.0);
    glVertex3f(size/2,-size/2,size/2);
    glVertex3f(-size/2,-size/2,size/2);
    glVertex3f(-size/2,-size/2,-size/2);
    glVertex3f(size/2,-size/2,-size/2);
    glEnd();
}



GLuint loadBMP_custom(const char * imagepath){
    
    printf("Reading image %s\n", imagepath);
    
    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    // Actual RGB data
    unsigned char * data;
    
    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file)							    {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0;}
    
    // Read the header, i.e. the 54 first bytes
    
    // If less than 54 bytes are read, problem
    if ( fread(header, 1, 54, file)!=54 ){
        printf("Not a correct BMP file\n");
        return 0;
    }
    // A BMP files always begins with "BM"
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }
    // Make sure this is a 24bpp file
    if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
    if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}
    
    // Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way
    
    // Create a buffer
    data = new unsigned char [imageSize];
    
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);
    
    // Everything is in memory now, the file wan be closed
    fclose (file);
    
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //if the texture is smaller, than the image, we get the avarege of the pixels next to it
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //same if the image bigger
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);      //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);      //we do it for vertically and horizontally (previous line)
     
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    // OpenGL has now copied the data. Free our own version
    delete [] data;
    
    // Return the ID of the texture we just created
    return textureID;
}



// main() function
// Initializes the user interface (and any user variables)
// then hands over control to the event handler, which calls
// display() whenever the GL window needs to be redrawn.
int main(int argc, char** argv)
{

    // Process program arguments
    if(argc != 3) {
        printf("Usage: demo [width] [height]\n");
        printf("Using 640x480 window by default...\n");
        Win[0] = 640;
        Win[1] = 480;
    } else {
        Win[0] = atoi(argv[1]);
        Win[1] = atoi(argv[2]);
    }

    loadAll();

    // Initialize data structs, glut, glui, and opengl
	initDS();
    initGlut(argc, argv);
    initGlui();
    initGl();
    initskybox();

    // Invoke the standard GLUT main event loop
    glutMainLoop();

    return 0;         // never reached
}


// Create / initialize global data structures
void initDS()
{
	keyframes = new Keyframe[KEYFRAME_MAX];
	for( int i = 0; i < KEYFRAME_MAX; i++ )
		keyframes[i].setID(i);

	animationTimer = new Timer();
	frameRateTimer = new Timer();
	joint_ui_data  = new Keyframe();
}


// Initialize glut and create a window with the specified caption 
void initGlut(int argc, char** argv)
{
	// Init GLUT
	glutInit(&argc, argv);

    // Set video mode: double-buffered, color, depth-buffered
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Create window
    glutInitWindowPosition (0, 0);
    glutInitWindowSize(Win[0],Win[1]);
    windowID = glutCreateWindow(argv[0]);

    // Setup callback functions to handle events
    glutReshapeFunc(reshape);	// Call reshape whenever window resized
    glutDisplayFunc(display);	// Call display whenever new frame needed
	glutMouseFunc(mouse);		// Call mouse whenever mouse button pressed
	glutMotionFunc(motion);		// Call motion whenever mouse moves while button pressed
}


// Load Keyframe button handler. Called when the "load keyframe" button is pressed
void loadKeyframeButton(int)
{
	// Get the keyframe ID from the UI
	int keyframeID = joint_ui_data->getID();

	// Update the 'joint_ui_data' variable with the appropriate
	// entry from the 'keyframes' array (the list of keyframes)
	*joint_ui_data = keyframes[keyframeID];

	// Sync the UI with the 'joint_ui_data' values
	glui_joints->sync_live();
	glui_keyframe->sync_live();
	glui_lightsource->sync_live();

	// Let the user know the values have been loaded
	sprintf(msg, "Status: Keyframe %d loaded successfully", keyframeID);
	status->set_text(msg);
}

// Update Keyframe button handler. Called when the "update keyframe" button is pressed
void updateKeyframeButton(int)
{
	///////////////////////////////////////////////////////////
	// TODO:
	//   Modify this function to save the UI joint values into
	//   the appropriate keyframe entry in the keyframe list
	//   when the user clicks on the 'Update Keyframe' button.
	//   Refer to the 'loadKeyframeButton' function for help.
	///////////////////////////////////////////////////////////

	// Get the keyframe ID from the UI
	int keyframeID = joint_ui_data->getID();

	// Update the 'maxValidKeyframe' index variable
	// (it will be needed when doing the interpolation)
	if (keyframeID > maxValidKeyframe) maxValidKeyframe = keyframeID;

	// Update the appropriate entry in the 'keyframes' array
	// with the 'joint_ui_data' data
	keyframes[keyframeID] = *joint_ui_data;

	// Let the user know the values have been updated
	sprintf(msg, "Status: Keyframe %d updated successfully", keyframeID);
	status->set_text(msg);
}

// Load Keyframes From File button handler. Called when the "load keyframes from file" button is pressed
//
// ASSUMES THAT THE FILE FORMAT IS CORRECT, ie, there is no error checking!
//
void loadKeyframesFromFileButton(int)
{
	// Open file for reading
	FILE* file = fopen(filenameKF, "r");
	if( file == NULL )
	{
		sprintf(msg, "Status: Failed to open file %s", filenameKF);
		status->set_text(msg);
		return;
	}

	// Read in maxValidKeyframe first
	fscanf(file, "%d", &maxValidKeyframe);

	// Now read in all keyframes in the format:
	//    id
	//    time
	//    DOFs
	//
	for( int i = 0; i <= maxValidKeyframe; i++ )
	{
		fscanf(file, "%d", keyframes[i].getIDPtr());
		fscanf(file, "%f", keyframes[i].getTimePtr());

		for( int j = 0; j < Keyframe::NUM_JOINT_ENUM; j++ )
			fscanf(file, "%f", keyframes[i].getDOFPtr(j));
	}

	// Close file
	fclose(file);

	// Let the user know the keyframes have been loaded
	sprintf(msg, "Status: Keyframes loaded successfully");
	status->set_text(msg);
}

// Save Keyframes To File button handler. Called when the "save keyframes to file" button is pressed
void saveKeyframesToFileButton(int)
{
	// Open file for writing
	FILE* file = fopen(filenameKF, "w");
	if( file == NULL )
	{
		sprintf(msg, "Status: Failed to open file %s", filenameKF);
		status->set_text(msg);
		return;
	}

	// Write out maxValidKeyframe first
	fprintf(file, "%d\n", maxValidKeyframe);
	fprintf(file, "\n");

	// Now write out all keyframes in the format:
	//    id
	//    time
	//    DOFs
	//
	for( int i = 0; i <= maxValidKeyframe; i++ )
	{
		fprintf(file, "%d\n", keyframes[i].getID());
		fprintf(file, "%f\n", keyframes[i].getTime());

		for( int j = 0; j < Keyframe::NUM_JOINT_ENUM; j++ )
			fprintf(file, "%f\n", keyframes[i].getDOF(j));

		fprintf(file, "\n");
	}

	// Close file
	fclose(file);

	// Let the user know the keyframes have been saved
	sprintf(msg, "Status: Keyframes saved successfully");
	status->set_text(msg);
}

// Animate button handler.  Called when the "animate" button is pressed.
void animateButton(int)
{
  // synchronize variables that GLUT uses
  glui_keyframe->sync_live();

  // toggle animation mode and set idle function appropriately
  if( animate_mode == 0 )
  {
	// start animation
	frameRateTimer->reset();
	animationTimer->reset();

	animate_mode = 1;
	GLUI_Master.set_glutIdleFunc(animate);

	// Let the user know the animation is running
	sprintf(msg, "Status: Animating...");
	status->set_text(msg);
  }
  else
  {
	// stop animation
	animate_mode = 0;
	GLUI_Master.set_glutIdleFunc(NULL);

	// Let the user know the animation has stopped
	sprintf(msg, "Status: Animation stopped");
	status->set_text(msg);
  }
}

// Render Frames To File button handler. Called when the "Render Frames To File" button is pressed.
void renderFramesToFileButton(int)
{
	// Calculate number of frames to generate based on dump frame rate
	int numFrames = int(keyframes[maxValidKeyframe].getTime() * DUMP_FRAME_PER_SEC) + 1;

	// Generate frames and save to file
	frameToFile = 1;
	for( frameNumber = 0; frameNumber < numFrames; frameNumber++ )
	{
		// Get the interpolated joint DOFs
		joint_ui_data->setDOFVector( getInterpolatedJointDOFS(frameNumber * DUMP_SEC_PER_FRAME) );

		// Let the user know which frame is being rendered
		sprintf(msg, "Status: Rendering frame %d...", frameNumber);
		status->set_text(msg);

		// Render the frame
		display();
	}
	frameToFile = 0;

	// Let the user know how many frames were generated
	sprintf(msg, "Status: %d frame(s) rendered to file", numFrames);
	status->set_text(msg);
}

// Quit button handler.  Called when the "quit" button is pressed.
void quitButton(int)
{
  exit(0);
}

// Initialize GLUI and the user interface
void initGlui()
{
	GLUI_Panel* glui_panel;
	GLUI_Spinner* glui_spinner;
	GLUI_RadioGroup* glui_radio_group;

    GLUI_Master.set_glutIdleFunc(NULL);


	// Create GLUI window (joint controls) ***************
	//
	glui_joints = GLUI_Master.create_glui("Joint Control", 0, Win[0]+12, 0);

    // Create controls to specify root position and orientation
	glui_panel = glui_joints->add_panel("Camera");

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate x:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_TRANSLATE_X));
	glui_spinner->set_float_limits(ROOT_TRANSLATE_X_MIN, ROOT_TRANSLATE_X_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate y:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_TRANSLATE_Y));
	glui_spinner->set_float_limits(ROOT_TRANSLATE_Y_MIN, ROOT_TRANSLATE_Y_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_TRANSLATE_Z));
	glui_spinner->set_float_limits(ROOT_TRANSLATE_Z_MIN, ROOT_TRANSLATE_Z_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate x:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_ROTATE_X));
	glui_spinner->set_float_limits(ROOT_ROTATE_X_MIN, ROOT_ROTATE_X_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate y:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_ROTATE_Y));
	glui_spinner->set_float_limits(ROOT_ROTATE_Y_MIN, ROOT_ROTATE_Y_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::ROOT_ROTATE_Z));
	glui_spinner->set_float_limits(ROOT_ROTATE_Z_MIN, ROOT_ROTATE_Z_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_joints->add_column(false);


    // Create controls to specify BODY position and orientation
	glui_panel = glui_joints->add_panel("Body");

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate x:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::BODY_TRANSLATE_X));
	glui_spinner->set_float_limits(BODY_TRANSLATE_X_MIN, BODY_TRANSLATE_X_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate y:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::BODY_TRANSLATE_Y));
	glui_spinner->set_float_limits(BODY_TRANSLATE_Y_MIN, BODY_TRANSLATE_Y_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "translate z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::BODY_TRANSLATE_Z));
	glui_spinner->set_float_limits(BODY_TRANSLATE_Z_MIN, BODY_TRANSLATE_Z_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate x:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::BODY_ROTATE_X));
	glui_spinner->set_float_limits(BODY_ROTATE_X_MIN, BODY_ROTATE_X_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate y:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::BODY_ROTATE_Y));
	glui_spinner->set_float_limits(BODY_ROTATE_Y_MIN, BODY_ROTATE_Y_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "rotate z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::BODY_ROTATE_Z));
	glui_spinner->set_float_limits(BODY_ROTATE_Z_MIN, BODY_ROTATE_Z_MAX, GLUI_LIMIT_WRAP);
	glui_spinner->set_speed(SPINNER_SPEED);

	// Create controls to specify head rotation
	glui_panel = glui_joints->add_panel("Head");

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "head:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::HEAD));
	glui_spinner->set_float_limits(HEAD_MIN, HEAD_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);


	glui_joints->add_column(false);

	// Create controls to specify right arm
	glui_panel = glui_joints->add_panel("Right arm");
	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "shoulder pitch:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::R_SHOULDER_PITCH));
	glui_spinner->set_float_limits(SHOULDER_PITCH_MIN, SHOULDER_PITCH_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "shoulder yaw:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::R_SHOULDER_YAW));
	glui_spinner->set_float_limits(SHOULDER_YAW_MIN, SHOULDER_YAW_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "shoulder roll:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::R_SHOULDER_ROLL));
	glui_spinner->set_float_limits(SHOULDER_ROLL_MIN, SHOULDER_ROLL_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	// Create controls to specify left arm
	glui_panel = glui_joints->add_panel("Left arm");

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "shoulder pitch:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::L_SHOULDER_PITCH));
	glui_spinner->set_float_limits(SHOULDER_PITCH_MIN, SHOULDER_PITCH_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "shoulder yaw:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::L_SHOULDER_YAW));
	glui_spinner->set_float_limits(SHOULDER_YAW_MIN, SHOULDER_YAW_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "shoulder roll:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::L_SHOULDER_ROLL));
	glui_spinner->set_float_limits(SHOULDER_ROLL_MIN, SHOULDER_ROLL_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_joints->add_column(false);


	// Create controls to specify right leg
	glui_panel = glui_joints->add_panel("Right leg");

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "hip pitch:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::R_HIP_PITCH));
	glui_spinner->set_float_limits(HIP_PITCH_MIN, HIP_PITCH_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "hip yaw:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::R_HIP_YAW));
	glui_spinner->set_float_limits(HIP_YAW_MIN, HIP_YAW_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "hip roll:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::R_HIP_ROLL));
	glui_spinner->set_float_limits(HIP_ROLL_MIN, HIP_ROLL_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	// Create controls to specify left leg
	glui_panel = glui_joints->add_panel("Left leg");

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "hip pitch:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::L_HIP_PITCH));
	glui_spinner->set_float_limits(HIP_PITCH_MIN, HIP_PITCH_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "hip yaw:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::L_HIP_YAW));
	glui_spinner->set_float_limits(HIP_YAW_MIN, HIP_YAW_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_joints->add_spinner_to_panel(glui_panel, "hip roll:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::L_HIP_ROLL));
	glui_spinner->set_float_limits(HIP_ROLL_MIN, HIP_ROLL_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);



	// ***************************************************
	glui_lightsource = GLUI_Master.create_glui("Light Source Control", 0, Win[0]+12, 360);
	glui_panel = glui_lightsource->add_panel("", GLUI_PANEL_NONE);

	glui_spinner = glui_lightsource->add_spinner_to_panel(glui_panel, "theta:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::LIGHT_SOURCE_ANGLE));
	glui_spinner->set_float_limits(LIGHT_SOURCE_MIN, LIGHT_SOURCE_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

/*	for 3D control of lightsource position:
	glui_spinner = glui_lightsource->add_spinner_to_panel(glui_panel, "light x:", GLUI_SPINNER_FLOAT, &light_pos[0]);
	glui_spinner->set_float_limits(LIGHT_SOURCE_X_MIN, LIGHT_SOURCE_X_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_spinner = glui_lightsource->add_spinner_to_panel(glui_panel, "light y:", GLUI_SPINNER_FLOAT, &light_pos[1]);
	glui_spinner->set_float_limits(LIGHT_SOURCE_Y_MIN, LIGHT_SOURCE_Y_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);
*/
	glui_spinner = glui_lightsource->add_spinner_to_panel(glui_panel, "z:", GLUI_SPINNER_FLOAT, joint_ui_data->getDOFPtr(Keyframe::LIGHT_SOURCE_Z));
	glui_spinner->set_float_limits(LIGHT_SOURCE_Z_MIN, LIGHT_SOURCE_Z_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);


	// Create GLUI window (keyframe controls) ************
	//
	glui_keyframe = GLUI_Master.create_glui("Keyframe Control", 0, 0, Win[1]+64);

	// Create a control to specify the time (for setting a keyframe)
	glui_panel = glui_keyframe->add_panel("", GLUI_PANEL_NONE);
	glui_spinner = glui_keyframe->add_spinner_to_panel(glui_panel, "Time:", GLUI_SPINNER_FLOAT, joint_ui_data->getTimePtr());
	glui_spinner->set_float_limits(TIME_MIN, TIME_MAX, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	// Create a control to specify a keyframe (for updating / loading a keyframe)
	glui_keyframe->add_column_to_panel(glui_panel, false);
	glui_spinner = glui_keyframe->add_spinner_to_panel(glui_panel, "Keyframe ID:", GLUI_SPINNER_INT, joint_ui_data->getIDPtr());
	glui_spinner->set_int_limits(KEYFRAME_MIN, KEYFRAME_MAX-1, GLUI_LIMIT_CLAMP);
	glui_spinner->set_speed(SPINNER_SPEED);

	glui_keyframe->add_separator();

	// Add buttons to load and update keyframes
	// Add buttons to load and save keyframes from a file
	// Add buttons to start / stop animation and to render frames to file
	glui_panel = glui_keyframe->add_panel("", GLUI_PANEL_NONE);
	glui_keyframe->add_button_to_panel(glui_panel, "Load Keyframe", 0, loadKeyframeButton);
	glui_keyframe->add_button_to_panel(glui_panel, "Load Keyframes From File", 0, loadKeyframesFromFileButton);
	glui_keyframe->add_button_to_panel(glui_panel, "Start / Stop Animation", 0, animateButton);
	glui_keyframe->add_column_to_panel(glui_panel, false);
	glui_keyframe->add_button_to_panel(glui_panel, "Update Keyframe", 0, updateKeyframeButton);
	glui_keyframe->add_button_to_panel(glui_panel, "Save Keyframes To File", 0, saveKeyframesToFileButton);
	glui_keyframe->add_button_to_panel(glui_panel, "Render Frames To File", 0, renderFramesToFileButton);

	glui_keyframe->add_separator();

	// Add status line
	glui_panel = glui_keyframe->add_panel("");
	status = glui_keyframe->add_statictext_to_panel(glui_panel, "Status: Ready");

	// Add button to quit
	glui_panel = glui_keyframe->add_panel("", GLUI_PANEL_NONE);
	glui_keyframe->add_button_to_panel(glui_panel, "Quit", 0, quitButton);
	//
	// ***************************************************


	// Create GLUI window (render controls) ************
	//
	glui_render = GLUI_Master.create_glui("Render Control", 0, 367, Win[1]+64);

	// Create control to specify the render style
	glui_panel = glui_render->add_panel("Render Style");
	glui_radio_group = glui_render->add_radiogroup_to_panel(glui_panel, &renderStyle);
	glui_render->add_radiobutton_to_group(glui_radio_group, "Wireframe");
	glui_render->add_radiobutton_to_group(glui_radio_group, "Solid");
	glui_render->add_radiobutton_to_group(glui_radio_group, "Solid w/ outlines");
	glui_render->add_radiobutton_to_group(glui_radio_group, "Metallic");
	glui_render->add_radiobutton_to_group(glui_radio_group, "Matte");
	//
	// ***************************************************


	// Tell GLUI windows which window is main graphics window
	glui_joints->set_main_gfx_window(windowID);
	glui_keyframe->set_main_gfx_window(windowID);
	glui_render->set_main_gfx_window(windowID);
	glui_lightsource->set_main_gfx_window(windowID);
}


// Performs most of the OpenGL intialization
void initGl(void)
{
    // glClearColor (red, green, blue, alpha)
    // Ignore the meaning of the 'alpha' value for now
    glClearColor(0.7f,0.7f,0.9f,1.0f);
}


// Calculates the interpolated joint DOF vector
// using Catmull-Rom interpolation of the keyframes
Vector getInterpolatedJointDOFS(float time)
{
	// Need to find the keyframes bewteen which
	// the supplied time lies.
	// At the end of the loop we have:
	//    keyframes[i-1].getTime() < time <= keyframes[i].getTime()
	//
	int i = 0;
	while( i <= maxValidKeyframe && keyframes[i].getTime() < time )
		i++;

	// If time is before or at first defined keyframe, then
	// just use first keyframe pose
	if( i == 0 )
		return keyframes[0].getDOFVector();

	// If time is beyond last defined keyframe, then just
	// use last keyframe pose
	if( i > maxValidKeyframe )
		return keyframes[maxValidKeyframe].getDOFVector();

	// Need to normalize time to (0, 1]
	time = (time - keyframes[i-1].getTime()) / (keyframes[i].getTime() - keyframes[i-1].getTime());
    float time2 = time * time;

	// Get appropriate data points and tangent vectors
	// for computing the interpolation
	Vector p1 = keyframes[i-1].getDOFVector();
	Vector p2 = keyframes[i].getDOFVector();
    Vector p0, p3;
	
    if (i == 1) {
        p0 = keyframes[i-1].getDOFVector();
        p3 = keyframes[i+1].getDOFVector();
    } else if (i == maxValidKeyframe) {
        p0 = keyframes[i-2].getDOFVector();
        p3 = keyframes[i].getDOFVector();
    } else {
        p0 = keyframes[i-2].getDOFVector();
        p3 = keyframes[i+1].getDOFVector();
    }
    
    Vector a0 = p0*(-0.5) + p1*1.5 - p2*1.5 + p3*0.5;
    Vector a1 = p0 - p1*2.5 + p2*2 - p3*0.5;
    Vector a2 = p0*(-0.5) + p2*0.5;
    Vector a3 = p1;
    
	// Cubic interpolation.
	return a0*time*time2 + a1*time2 + a2*time + a3;
}


// Callback idle function for animating the scene
void animate()
{
	// Only update if enough time has passed
	// (This locks the display to a certain frame rate rather
	//  than updating as fast as possible. The effect is that
	//  the animation should run at about the same rate
	//  whether being run on a fast machine or slow machine)
	if( frameRateTimer->elapsed() > SEC_PER_FRAME )
	{
		// Tell glut window to update itself. This will cause the display()
		// callback to be called, which renders the object (once you've written
		// the callback).
		glutSetWindow(windowID);
		glutPostRedisplay();

		// Restart the frame rate timer
		// for the next frame
		frameRateTimer->reset();
	}
}


// Handles the window being resized by updating the viewport
// and projection matrices
void reshape(int w, int h)
{
	// Update internal variables and OpenGL viewport
	Win[0] = w;
	Win[1] = h;
	glViewport(0, 0, (GLsizei)Win[0], (GLsizei)Win[1]);

    // Setup projection matrix for new window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(CAMERA_FOVY, (GLdouble)Win[0]/(GLdouble)Win[1], NEAR_CLIP, FAR_CLIP);
}



// display callback
//
// README: This gets called by the event handler
// to draw the scene, so this is where you need
// to build your scene -- make your changes and
// additions here. All rendering happens in this
// function. For Assignment 2, updates to the
// joint DOFs (joint_ui_data) happen in the
// animate() function.
void display(void)
{

	glTranslatef(0,-40.00,-105);


    // Clear the screen with the background colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

    // Setup the model-view transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// Specify camera transformation
	glTranslatef(camXPos, camYPos, camZPos);
	rotateCamera();

	// setup transformation for camera
	glTranslatef(joint_ui_data->getDOF(Keyframe::ROOT_TRANSLATE_X),
			 joint_ui_data->getDOF(Keyframe::ROOT_TRANSLATE_Y),
			 joint_ui_data->getDOF(Keyframe::ROOT_TRANSLATE_Z));

	// Get the time for the current animation step, if necessary
	if( animate_mode )
	{
		float curTime = animationTimer->elapsed();

		if( curTime >= keyframes[maxValidKeyframe].getTime() )
		{
			// Restart the animation
			animationTimer->reset();
			curTime = animationTimer->elapsed();
		}

		joint_ui_data->setDOFVector( getInterpolatedJointDOFS(curTime) );

		// Update user interface
		joint_ui_data->setTime(curTime);
		glui_keyframe->sync_live();
	}
    
    Draw_Skybox(0,0,0,200,200,200);	// Draw the Skybox
    
	// LIGHTING SETTINGS
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	light_pos[0] = radius*cos(joint_ui_data->getDOF(Keyframe::LIGHT_SOURCE_ANGLE));
	light_pos[1] = radius*sin(joint_ui_data->getDOF(Keyframe::LIGHT_SOURCE_ANGLE));
	light_pos[2] = joint_ui_data->getDOF(Keyframe::LIGHT_SOURCE_Z);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);


	GLfloat specular[] = {1,1,1,1};
	GLfloat diffuse[] = {1,1,1,1};
	GLfloat ambient[] = {0.1,0.1,0.1,1};
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 4);

	// make the light source a spotlight always directed at the penguin
	GLfloat light_dir[] = {0-light_pos[0],0-light_pos[1],0-light_pos[2]};
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);


	// DRAWING
	glPushMatrix();


		glEnable(GL_NORMALIZE);
		glEnable(GL_RESCALE_NORMAL);
		glShadeModel(GL_SMOOTH);

		drawGround();

		glTranslatef(joint_ui_data->getDOF(Keyframe::BODY_TRANSLATE_X),
					 joint_ui_data->getDOF(Keyframe::BODY_TRANSLATE_Y),
					 joint_ui_data->getDOF(Keyframe::BODY_TRANSLATE_Z));

		// determine render style and set glPolygonMode appropriately
		// for each style, enable or disable GL parameters
		if (renderStyle == WIREFRAME){
			glEnable(GL_LINE_SMOOTH);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			drawBaymax();
		}
		else if (renderStyle == SOLID){
			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			drawBaymax();
		}
		else if (renderStyle == OUTLINED){
			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			drawBaymax();	// draw solid penguin

			outline = true;
			glPolygonOffset(-0.5,-0.5);
			glDisable(GL_LIGHTING);
			glEnable(GL_POLYGON_OFFSET_LINE);
			glEnable(GL_LINE_SMOOTH);
			glPolygonMode(GL_FRONT, GL_LINE);
			//glLineWidth(0.1);
			glColor3f(0,0,0);

			drawBaymax();	// draw penguin outline
			glDisable(GL_POLYGON_OFFSET_LINE);
			glEnable(GL_LIGHTING);
			glColor3f(1,1,1);
			outline = false;
			glPolygonMode(GL_FRONT, GL_FILL);
		}
		else if (renderStyle == METALLIC){
			
			GLfloat material_specular[] = {1,1,1,1};	// highly specular
			GLfloat material_diffuse[] = {0.5,0.5,0.5,1};	// medium gray diffuse
			glMaterialf(GL_FRONT,GL_SHININESS, 100);	// very shiny
			glMaterialfv(GL_FRONT,GL_SPECULAR,material_specular);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,material_diffuse);

			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			drawBaymax();

			outline = true;
			glPolygonOffset(-0.5,-0.5); // to avoid fill and outline fighting over pixels
			glDisable(GL_LIGHTING);
			glEnable(GL_POLYGON_OFFSET_LINE);
			glEnable(GL_LINE_SMOOTH);
			glPolygonMode(GL_FRONT, GL_LINE);
			drawBaymax();
			glDisable(GL_POLYGON_OFFSET_LINE);
			glEnable(GL_LIGHTING);
			outline = false;
			glPolygonMode(GL_FRONT, GL_FILL);
		}
		else if (renderStyle == MATTE){ // MAIN MODE TO RENDER BAYMAX IN

			GLfloat material_specular[] = {0.1,0.1,0.1,1};	// no specularity
			GLfloat material_diffuse[] = {1,1,1,1};	// 
			GLfloat material_ambient[] = {0.5,0.5,0.5,1};	// 
			glMaterialf(GL_FRONT,GL_SHININESS, 40);		//	
			glMaterialfv(GL_FRONT,GL_SPECULAR,material_specular);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,material_diffuse);
			glMaterialfv(GL_FRONT,GL_AMBIENT,material_ambient);

			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			drawBaymax();
/*
			outline = true;
			glPolygonOffset(-0.5,-0.5);
			glDisable(GL_LIGHTING);
			glEnable(GL_POLYGON_OFFSET_LINE);
			glEnable(GL_LINE_SMOOTH);
			glPolygonMode(GL_FRONT, GL_LINE);
			drawBaymax();
			glDisable(GL_POLYGON_OFFSET_LINE);
			glEnable(GL_LIGHTING);
			outline = false;
			glPolygonMode(GL_FRONT, GL_FILL);*/
		}

	glPopMatrix();


    // Execute any GL functions that are in the queue just to be safe
    glFlush();

	// Dump frame to file, if requested
	if( frameToFile )
	{
		sprintf(filenameF, "frame%03d.ppm", frameNumber);
		writeFrame(filenameF, false, false);
	}

    // Now, show the frame buffer that we just drew into.
    // (this prevents flickering).
    glutSwapBuffers();
}


// Handles mouse button pressed / released events
void mouse(int button, int state, int x, int y)
{
	// If the RMB is pressed and dragged then zoom in / out
	if( button == GLUT_RIGHT_BUTTON )
	{
		if( state == GLUT_DOWN )
		{
			lastX = x;
			lastY = y;
			updateCamZPos = true;
		}
		else
		{
			updateCamZPos = false;
		}
	}
}


// Handles mouse motion events while a button is pressed
void motion(int x, int y)
{
	// If the RMB is pressed and dragged then zoom in / out
	if( updateCamZPos )
	{
		// Update camera z position
		camZPos += (x - lastX) * ZOOM_SCALE;
		lastX = x;

		// Redraw the scene from updated camera position
		glutSetWindow(windowID);
		glutPostRedisplay();
	}
}


///////////////////////////////////////////////////////////
//
// BELOW ARE FUNCTIONS FOR GENERATING IMAGE FILES (PPM/PGM)
//
///////////////////////////////////////////////////////////

void writePGM(char* filename, GLubyte* buffer, int width, int height, bool raw=true)
{
	FILE* fp = fopen(filename,"wt");

	if( fp == NULL )
	{
		printf("WARNING: Can't open output file %s\n",filename);
		return;
	}

	if( raw )
	{
		fprintf(fp,"P5\n%d %d\n%d\n",width,height,255);
		for(int y=height-1;y>=0;y--)
		{
			fwrite(&buffer[y*width],sizeof(GLubyte),width,fp);
			/*
			for(int x=0;x<width;x++)
			{
				fprintf(fp,"%c",int(buffer[x+y*width];
			}
			*/
		}
	}
	else
	{
		fprintf(fp,"P2\n%d %d\n%d\n",width,height,255);
		for(int y=height-1;y>=0;y--)
		{
			for(int x=0;x<width;x++)
			{
				fprintf(fp,"%d ",*((int *)(&buffer[x+y*width])));
			}
			fprintf(fp,"\n");
		}
	}

	fclose(fp);
}

#define RED_OFFSET   0
#define GREEN_OFFSET 1
#define BLUE_OFFSET  2

void writePPM(char* filename, GLubyte* buffer, int width, int height, bool raw=true)
{
	FILE* fp = fopen(filename,"wt");

	if( fp == NULL )
	{
		printf("WARNING: Can't open output file %s\n",filename);
		return;
	}

	if( raw )
	{
		fprintf(fp,"P6\n%d %d\n%d\n",width,height,255);
		for(int y=height-1;y>=0;y--)
		{
			for(int x=0;x<width;x++)
			{
				GLubyte* pix = &buffer[4*(x+y*width)];

				fprintf(fp,"%c%c%c",int(pix[RED_OFFSET]),
									int(pix[GREEN_OFFSET]),
									int(pix[BLUE_OFFSET]));
			}
		}
	}
	else
	{
		fprintf(fp,"P3\n%d %d\n%d\n",width,height,255);
		for(int y=height-1;y>=0;y--)
		{
			for(int x=0;x<width;x++)
			{
				GLubyte* pix = &buffer[4*(x+y*width)];

				fprintf(fp,"%d %d %d ",int(pix[RED_OFFSET]),
									   int(pix[GREEN_OFFSET]),
									   int(pix[BLUE_OFFSET]));
			}
			fprintf(fp,"\n");
		}
	}

	fclose(fp);
}

void writeFrame(char* filename, bool pgm, bool frontBuffer)
{
	static GLubyte* frameData = NULL;
	static int currentSize = -1;

	int size = (pgm ? 1 : 4);

	if( frameData == NULL || currentSize != size*Win[0]*Win[1] )
	{
		if (frameData != NULL)
			delete [] frameData;

		currentSize = size*Win[0]*Win[1];

		frameData = new GLubyte[currentSize];
	}

	glReadBuffer(frontBuffer ? GL_FRONT : GL_BACK);

	if( pgm )
	{
		glReadPixels(0, 0, Win[0], Win[1],
					 GL_LUMINANCE, GL_UNSIGNED_BYTE, frameData);
		writePGM(filename, frameData, Win[0], Win[1]);
	}
	else
	{
		glReadPixels(0, 0, Win[0], Win[1],
					 GL_RGBA, GL_UNSIGNED_BYTE, frameData);
		writePPM(filename, frameData, Win[0], Win[1]);
	}
}


//////////////////////////////////


void loadAll(){

	loadOBJ("baymax/chestbelly.obj", chestbelly_vertices, chestbelly_uvs, chestbelly_normals,0,0,0);
	loadOBJ("baymax/leftleg.obj", leftleg_vertices, leftleg_uvs, leftleg_normals,150,224,0);
	loadOBJ("baymax/leftarm.obj", leftarm_vertices, leftarm_uvs, leftarm_normals,432,696,0);
	loadOBJ("baymax/rightarm.obj", rightarm_vertices, rightarm_uvs, rightarm_normals,760,1161,0);
	loadOBJ("baymax/rightleg.obj", rightleg_vertices, rightleg_uvs, rightleg_normals,1088,1626,0);
	loadOBJ("baymax/stamp.obj", stamp_vertices, stamp_uvs, stamp_normals,1370,2097,0);
	loadOBJ("baymax/head.obj", head_vertices, head_uvs, head_normals,1546,2174,28);

}

void drawHead() {
	glPushMatrix();
	// transform
	glTranslatef(0,69,-3); // move to actual position
	glRotatef(joint_ui_data->getDOF(Keyframe::HEAD),0,1,0);
	glTranslatef(0,-69,3); // move to origin

	// draw
	glBegin(GL_QUADS);
	int number_of_vertices = head_vertices.size();
	for (int j = 0; j < number_of_vertices; j++){
		glNormal3f( head_normals[j].x, head_normals[j].y, head_normals[j].z);
		glVertex3f( head_vertices[j].x, head_vertices[j].y, head_vertices[j].z);
	}
	glEnd();
	glPopMatrix();
}

void drawChestBelly() {
	glBegin(GL_QUADS);
	int number_of_vertices = chestbelly_vertices.size();
	for (int j = 0; j < number_of_vertices; j++){
		glNormal3f( chestbelly_normals[j].x, chestbelly_normals[j].y, chestbelly_normals[j].z);
		glVertex3f( chestbelly_vertices[j].x, chestbelly_vertices[j].y, chestbelly_vertices[j].z);
	}
	glEnd();
}

void drawLeftArm() {
	glPushMatrix();

	// apply transformations
	glTranslatef(15,58,-3); // move back to actual position
	glRotatef(-1*joint_ui_data->getDOF(Keyframe::L_SHOULDER_PITCH),1,0,0);
	glRotatef(-1*joint_ui_data->getDOF(Keyframe::L_SHOULDER_ROLL),0,1,0);
	glRotatef(-1*joint_ui_data->getDOF(Keyframe::L_SHOULDER_YAW),0,0,1);
	glTranslatef(-15,-58,3); // move to origin for rotations

	// draw object
	glBegin(GL_QUADS);
	int number_of_vertices = leftarm_vertices.size();
	for (int j = 0; j < number_of_vertices; j++){
		glNormal3f( leftarm_normals[j].x, leftarm_normals[j].y, leftarm_normals[j].z);
		glVertex3f( leftarm_vertices[j].x, leftarm_vertices[j].y, leftarm_vertices[j].z);
	}
	glEnd();
	glPopMatrix();
}
void drawLeftLeg(){

	glPushMatrix();
	glTranslatef(10,19,-3);
	glRotatef(joint_ui_data->getDOF(Keyframe::L_HIP_YAW),1,0,0);
	glRotatef(joint_ui_data->getDOF(Keyframe::L_HIP_ROLL),0,1,0);
	glRotatef(joint_ui_data->getDOF(Keyframe::L_HIP_PITCH),0,0,1);
	glTranslatef(-10,-19,3);

	glBegin(GL_QUADS);
	int number_of_vertices = leftleg_vertices.size();
	for (int j = 0; j < number_of_vertices; j++){
		glNormal3f( leftleg_normals[j].x, leftleg_normals[j].y, leftleg_normals[j].z);
		glVertex3f( leftleg_vertices[j].x, leftleg_vertices[j].y, leftleg_vertices[j].z);
	}
	glEnd();
	glPopMatrix();
}

void drawRightArm(){

	glPushMatrix();
	// apply transformations
	glTranslatef(-15,58,-3);
	glRotatef(-1*joint_ui_data->getDOF(Keyframe::R_SHOULDER_PITCH),1,0,0);
	glRotatef(-1*joint_ui_data->getDOF(Keyframe::R_SHOULDER_ROLL),0,1,0);
	glRotatef(-1*joint_ui_data->getDOF(Keyframe::R_SHOULDER_YAW),0,0,1);
	glTranslatef(15,-58,3);

	// draw object
	glBegin(GL_QUADS);
	int number_of_vertices = rightarm_vertices.size();
	for (int j = 0; j < number_of_vertices; j++){
		glNormal3f( rightarm_normals[j].x, rightarm_normals[j].y, rightarm_normals[j].z);
		glVertex3f( rightarm_vertices[j].x, rightarm_vertices[j].y, rightarm_vertices[j].z);
	}
	glEnd();
	glPopMatrix();
}

void drawRightLeg(){

	glPushMatrix();
	glTranslatef(-10,19,-3);
	glRotatef(joint_ui_data->getDOF(Keyframe::R_HIP_YAW),1,0,0);
	glRotatef(joint_ui_data->getDOF(Keyframe::R_HIP_ROLL),0,1,0);
	glRotatef(joint_ui_data->getDOF(Keyframe::R_HIP_PITCH),0,0,1);
	glTranslatef(10,-19,3);

	glBegin(GL_QUADS);
	int number_of_vertices = rightleg_vertices.size();
	for (int j = 0; j < number_of_vertices; j++){
		glNormal3f( rightleg_normals[j].x, rightleg_normals[j].y, rightleg_normals[j].z);
		glVertex3f( rightleg_vertices[j].x, rightleg_vertices[j].y, rightleg_vertices[j].z);
	}
	glEnd();
	glPopMatrix();
}

void drawStamp(){
	glPushMatrix();
	glTranslatef(0,0,1);
	glBegin(GL_QUADS);
	int number_of_vertices = stamp_vertices.size();
	for (int j = 0; j < number_of_vertices; j++){
		glNormal3f( stamp_normals[j].x, stamp_normals[j].y, stamp_normals[j].z);
		glVertex3f( stamp_vertices[j].x, stamp_vertices[j].y, stamp_vertices[j].z);
	}
	glEnd();
	glPopMatrix();
}

void drawBaymax() {
	rotateBaymax();
	glColor3f(0,0,0);
	if (outline == false) glColor3f(1,1,1);	// draw outlines in black
	drawChestBelly();
	drawHead();
	drawLeftLeg();
	drawLeftArm();
	drawRightArm();
	drawRightLeg();
	drawStamp();
}

void drawGround(){
	glPushMatrix();

	GLfloat material_specular[] = {1,1,1,1};	// highly specular
	GLfloat material_diffuse[] = {0.5,0.5,0.5,1};	// medium gray diffuse
	glMaterialf(GL_FRONT,GL_SHININESS, 100);	// very shiny
	glMaterialfv(GL_FRONT,GL_SPECULAR,material_specular);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,material_diffuse);
	glColor3f(0.5,0.3,0.1);
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glVertex3f(-50,0,-50);
	glVertex3f(-50,0,50);
	glVertex3f(50,0,50);
	glVertex3f(50,0,-50);
	glEnd();
	glPopMatrix();

}
void rotateCamera(){
	glRotatef(joint_ui_data->getDOF(Keyframe::ROOT_ROTATE_X), 1.0, 0.0, 0.0);
	glRotatef(joint_ui_data->getDOF(Keyframe::ROOT_ROTATE_Y), 0.0, 1.0, 0.0);
	glRotatef(joint_ui_data->getDOF(Keyframe::ROOT_ROTATE_Z), 0.0, 0.0, 1.0);
}

void rotateBaymax(){
	glRotatef(joint_ui_data->getDOF(Keyframe::BODY_ROTATE_X), 1.0, 0.0, 0.0);
	glRotatef(joint_ui_data->getDOF(Keyframe::BODY_ROTATE_Y), 0.0, 1.0, 0.0);
	glRotatef(joint_ui_data->getDOF(Keyframe::BODY_ROTATE_Z), 0.0, 0.0, 1.0);
}
