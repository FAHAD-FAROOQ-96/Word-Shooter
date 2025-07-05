//============================================================================
// Name        : Word-Shooter.cpp
// Author      : Fahad Farooq 24i-2071 AI-C
// Version     : 2024
// Copyright   : (c) Reserved
// Description : Basic 2D game of Word Shooter...
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include "util.h"
#include <cstdlib>
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) 
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 60
int t_rows;
int t_columns;
string * dictionary;
int dictionarysize = 369646;
#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // ball radius in pixels...
const int tiles = 60;
int width = 930, height = 660;
int byoffset = bradius;

/// my variables ///
int arr[11][15]; // 2D-array
int i=0;
int balls[100] ;
int TimeLeft=150000;
bool BallinMotion=false;
int X1=width/2 ,Y1=8;
float vx ,vy;
int checking_space;
bool game_ended=false;
/// till here ///


int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows 
float score = 0;
int **board; 
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
const int nalphabets = 26;
enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};

GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...

//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels

	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
int GetAlphabet() {
	return GetRandInRange(1, 26);
}

void Pixels2Cell(int px, int py, int & cx, int &cy) {
// converts the pixel coordinates to cell coordinates...
cx = px / 60;
cy = py / 60;
}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
px = cx * 60;
py = cy * 60;
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
/*
* Main Canvas drawing function.
* */
void DisplayFunction() {
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	glClearColor(1/*Red Component*/, 1/*Green Component*/,
		1/*Blue Component*/, 1/*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors
	//write your drawing commands here or call your drawing functions...
	if(TimeLeft>0){
	
	DrawAlphabet((alphabets)balls[i], width/2, 8, awidth, aheight);
	
	if(BallinMotion){
	DrawAlphabet((alphabets)balls[i], X1, Y1, awidth, aheight);
	}
	DrawAlphabet((alphabets)balls[i+1], width/4, 8, awidth, aheight);
	
	
	for(int i = 0 ; i<t_rows;i++)
	{
	for(int j = 0; j<t_columns;j++)
	{
	 int x = 10 + j * tiles; //for filling rows 
         int y = 550 - i * tiles;  //for filling cols 
        if(arr[i][j]!=-1){ 
        DrawAlphabet((alphabets)arr[i][j], x, y, awidth, aheight);}
    }
}
        DrawString(700, height - 20, width, height + 5, "Fahad 24i-2071", colors[BLUE_VIOLET]);
	DrawString(40, height - 20, width, height + 5, "Score " + Num2Str(score), colors[BLUE_VIOLET]);
	
	DrawString(400 - 30, height - 25, width, height,
		"Time Left:" + Num2Str(TimeLeft/1000) + " secs", colors[RED]);

	// #----------------- Write your code till here ----------------------------#
	//DO NOT MODIFY THESE LINES
	DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	}
	else{
	DrawString(width / 2 - 120, height -330, width, height,"OH NO! YOU LOST!!!",colors[RED]);
	
	}
	glutSwapBuffers();
	//DO NOT MODIFY THESE LINES..
}

/* Function sets canvas size (drawing area) in pixels...

*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)

* */
void SetCanvasSize(int width, int height) {
	/*glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)

* is pressed from the keyarr
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...

*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the

* program coordinates of mouse pointer when key was pressed.
*
* */


void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call

	* this function*/
	/*

	glutPostRedisplay();

	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting

*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls

}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window

*

* You will have to add the necessary code here for shooting, etc.

*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),

* x & y that tells the coordinate of current position of move mouse
*
* */

void MouseClicked(int button, int state, int x, int y) {


	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) // dealing only with left button
	{
	    if(!BallinMotion){
		
        float dx = x - X1;
        float dy = (height - y)- Y1;
        float length = sqrt(dx * dx + dy * dy);
        // reseting 
        
        float speed = 20.0;
            vx = (dx / length) *speed;
            vy = (dy / length) *speed;
            BallinMotion = true; }
        
    }	   
	
	else if (button == GLUT_RIGHT_BUTTON  && state== GLUT_UP && X1==width/2 && Y1==8) // dealing with right button
	{  //swapping
          int temp = balls[i];
          balls[i]=balls[i+1];
          balls[i+1]=temp;
	}
	
	glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyarr

* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
}

/*

* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).

* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.

*

* */

void Timer(int m) {
      if (BallinMotion) {
         X1 += vx;
         Y1 += vy;
         // bouncing 
         if(X1<0||X1>width-50){
         vx=-vx;
         }
      
         int row ;
         int col ;
  
         row=((545)- Y1)/tiles;
         col=(X1)/tiles;
        
         bool space_available=false;
         if( arr[row][col] !=-1){
         // check space near it
         if( arr[row+1][col]==-1){
         checking_space=1;
         space_available=true;
         }
         if(space_available==false&&col>t_columns/2){
         checking_space=4;
         space_available=true;
         }
         
         // checking sides
         if(  space_available==false){
          if(  col+1<t_columns && arr[row][col+1]==-1){
         checking_space=2;
         space_available=true;
         }
         
         if( col-1>0&&arr[row][col-1]==-1&&space_available==false){
         checking_space=3;
          space_available=true;
         }
         }
         // checking sides
         
         switch(checking_space)
         {
         
         case 1:
         arr[++row][col]=balls[i]; //next row
         BallinMotion=false;
         i++; 
         X1=width/2;
         Y1=8;
         break;
        
         case 2:
         arr[++row][++col]=balls[i]; //next row , right column
         BallinMotion=false;
         i++; 
         X1=width/2;
         Y1=8;
         break;
        
         case 3:
         arr[++row][--col]=balls[i]; //next row , left column
         BallinMotion=false;
         i++; 
         X1=width/2;
         Y1=8;
         break;
        
         case 4:
         arr[++row][--col]=balls[i]; //next row , left column
         BallinMotion=false;
         i++; 
         X1=width/2;
         Y1=8;
         break;
                
    } 
  }
}

        TimeLeft -= 1000/FPS; 
        glutPostRedisplay();
	glutTimerFunc(1000.0/FPS, Timer, 0);
}

void myboard(){
t_rows=11;
t_columns=15;

// first two rows
for( int rows=0; rows<2;rows++){
for(int cols=0; cols<t_columns;cols++){
arr[rows][cols]=GetAlphabet(); }
  }
// remaining all are empty 
for( int rows=2; rows<t_rows;rows++){
for( int cols=0; cols<t_columns;cols++){
arr[rows][cols]=-1; }
  }
}

char num2char(int x)
{
	return static_cast<char>(x+97);
}

void two_rows_popping() {
    ofstream file("poppedout.txt");
    string row1 = "", row2 = "";

    
    for (int col = 0; col < 15; col++) {
        row1 += num2char(arr[0][col]);
        row2 += num2char(arr[1][col]);
    }

    file << "First Row: " << row1 << "\n";
    file << "Second Row: " << row2 << "\n";
    cout << "First Row: " << row1 << "\n";
    cout << "Second Row: " << row2 << "\n";

    for (int i = 0; i < dictionarysize; i++) {
        string word = dictionary[i];
        int len = word.size();

        if (len > 2) {
            int pos1 = row1.find(word);
            int pos2 = row2.find(word);

            if (pos1 != -1) {
                file << "word found in row 1 : " << word << " (at position " << pos1 << ")\n";
		cout << "word found in Row 1: " << word << " (at position " << pos1 << ")\n";
                for (int j = 0; j < len; j++) {
                    arr[0][pos1 + j] = -1;  
                    score++;
                }
            }

            if (pos2 != string::npos) {
                file << "word found in Row 2: " << word << " (at position " << pos2 << ")\n";
		cout << "word found in Row 2: " << word << " (at position " << pos2 << ")\n";
                for (int j = 0; j < len; j++) {
                    arr[1][pos2 + j] = -1; 
                    score++;
                }
            }
        }
		file.close();
    }

    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 15; col++) {
            if (arr[row][col] == -1) {
                arr[row][col] = GetAlphabet();
            }
        }
    }

    cout << "~~~New two rows after popping words~~~" << "\n";
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 15; col++) {
            cout << num2char(arr[row][col]) << " ";
			file << num2char(arr[row][col]) << " ";
        }
        cout << "\n";
	file<<"\n";
    }
}
/*
* our gateway main function
* */
int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...
	
	system("aplay music.wav &"); //for music

	//Dictionary for matching the words. It contains the 369646 words.
	dictionary = new string[dictionarysize]; 
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary
	for(int i=0; i < 5; ++i)
	cout<< " word "<< i << " =" << dictionary[i] <<"\n";

	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets
	
	for(int f = 0; f<100;f++){
        balls[f]=GetAlphabet();}
        myboard();
        two_rows_popping();
        
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Word Shooter"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0/FPS, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();
  
	return 1;
}
#endif /* */
