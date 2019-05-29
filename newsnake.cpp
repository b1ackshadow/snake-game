#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<glut.h>

#define COLUMNS 40
#define ROWS 40
// FPS -> frames per second
// here greater the FPS faster is the game speed
#define FPS 10

//	#####################################################
//this are the GLUT_KEY_UP values we can use these to compute snake direction
#define UP 101
#define DOWN 103
#define RIGHT 102
#define LEFT 100
#define MAX 60 //max size of the body of snake


//	#####################################################


int gridX, gridY;	// for grid coordinates
int snake_length = 5; //init snake body length
int snake_direction = RIGHT;	// initial snake direction is right
int posX[60] = { 20, 20, 20, 20, 20 }, posY[60] = { 20, 19, 18, 17, 16 };	// initial position of snake

int gameOver = 0;	//bool
int food = 1;
int bigFood = 0;		//bool
int bigFoodCount = 0;
int foodX, foodY;	// food position
int bigTimer = 0;
int bigX,bigY;
int score = 0;
bool clicker = true;

void unit(int, int);
//this sets the maximum value of the grid to check boundary condition for collision of snake
void initGrid(int x, int y) {
	gridX = x;
	gridY = y;
}


void unit(int x, int y) {	// for drawing each square block (grid) on the display

	if (x == 0 || y == 0 || x == gridX - 1 || y == gridY - 1) {	
// if square is to be drawn at the edges of window change the color to RED and increase thickness.
//this means its the outer wall
		glLineWidth(3.0);
		glColor3f(1.0, 0.0, 0.0);
		//drawing a square of length 1 unit 
		glBegin(GL_POLYGON);//outer wall solid 
	}
	else {
		glLineWidth(1.0);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_LOOP);

	}
	glVertex2f(x, y);
	glVertex2f(x + 1, y);
	glVertex2f(x + 1, y + 1);
	glVertex2f(x, y + 1);
	glEnd();
}

void drawGrid() {	// for drawing square grids
	for (int x = 0; x < gridX; x++) {
		for (int y = 0; y < gridY; y++) {
			unit(x, y);
		}
	}
}


//	=====================================================

int bodyCollision() {
	// it is used to detect body collision of snake
	for (int i = snake_length - 1; i > 0; i--) {
		if (posX[i] == posX[0] && posY[i] == posY[0])	//if head position matches(coincides) with any of body position then GAMEOVER!
			return 1;
	}
	return 0;
}


void drawSnake() {
	/*
	draw snake with updated values
	to move the body of the snake we just have to shift all the body parts from back to front except head
	head will goto a new position based on the key pressed.
	So just by changing heads position rest of the body will just follow head 
	*/	

	//starting with the last part of body towards head 
	for (int i = snake_length - 1; i > 0; i--) {
		posX[i] = posX[i - 1];
		posY[i] = posY[i - 1];
	}

	if (snake_direction == UP)
		posY[0]++;		//increment posY ,posX remains same
	else if (snake_direction == DOWN)
		posY[0]--;		//decrement posY ,posX remains same
	else if (snake_direction == RIGHT)
		posX[0]++;		//increment posX ,posY remains same
	else if (snake_direction == LEFT)
		posX[0]--;		//decrement posX ,posY remains same




	//values of the snake body are changed now drawing it actually
	for (int i = 0; i<snake_length; i++) {
		if (i == 0)
			//green color for head of the snake
			glColor3f(0.0, 1.0, 0.0);
		else
			glColor3f(0.0, 0.0, 1.0);
		//to draw a solid rectangle for each block of snake body
		glRectd(posX[i], posY[i], posX[i] + 1, posY[i] + 1);
	}
	glColor3f(1.0, 0.0, 0.0);
	if (posX[0] == 0 || posX[0] == gridX - 1 || posY[0] == 0 || posY[0] == gridY - 1 || bodyCollision()) {	// it snake head touches the border of the window or touches itself then GAMEOVER!! 
		gameOver = 1;
	}

	else if(posX[0] == foodX && posY[0] == foodY) {	// checking if newly generated food location is same as the snake location
		score++;
		snake_length++;//body size is increased now so the last block we used to overwrite will be written to this new block at the tail of snake and rest all works same
		
		

		//if snake length is greater than this it will hard to play so we wont increase size of the body
		if (snake_length > MAX)
			snake_length = MAX;
		food = 1;	//we have eaten food now so set it back to true which means generate new food

		//we set this value because every time frame is rendered drawfood is called even if snake\
		//hasnt eaten the food 
	}
	else if(posX[0] == bigX && posY[0] == bigY) {
	//	else if(posX[0] >= bigX && posY[0] >= bigY && posX[0] <= bigX+2 && posY[0] <= bigY+2) {
		score+=4;
		snake_length+=4;
		if (snake_length > MAX)
			snake_length = MAX;
		food = 1;	//we have eaten food now so set it back to true which means generate new food
		bigFoodCount = 0;
		bigFood = 0;
		bigTimer = 0;
	}
	//to display tongue right before snake eats it
	glColor3f(1.0,0.0,0.0);
	if(posX[0] == foodX - 2 && posY[0] == foodY && snake_direction == RIGHT) 
	{
		glRectd(posX[0]+1, posY[0], posX[0] + 2, posY[0]+1);
	}
	else if(posX[0] == foodX + 2 && posY[0] == foodY && snake_direction == LEFT) 
	{
		glRectd(posX[0] - 1, posY[0], posX[0], posY[0] + 1);
	}
	else if(posY[0] == foodY-2 && posX[0] == foodX  && snake_direction == UP) 
	{
		glRectd(posX[0], posY[0]+1, posX[0] + 1, posY[0] + 2);
	}
	else if(posY[0] == foodY+2 && posX[0] == foodX && snake_direction == DOWN) 
	{
		glRectd(posX[0], posY[0] - 1, posX[0] + 1, posY[0]);
	}

}

int findElement(int *a,int length,int ele)
{
	//Search starts from the 0th location
   int i = 0;
   while (i < length && ele != a[i]) {
      i++;
   }
	if (i < length) 
	      return 1;
	return 0;
}

//	======================================================


void drawFood() {
	if (food) {	// generating food position
				//	random(foodX, foodY);
		//inbetween the walls
	//printf("food = %d bigFood = %d bigFoodCount = %d \n",food,bigFood,bigFoodCount);
		int maxX = gridX - 2, maxY = gridY - 2;
		int min = 1;
		while(1)
		{
			foodX = min + rand() % (maxX - min);
			foodY = min + rand() % (maxY - min);
			int statusX = findElement(posX,snake_length,foodX);
			int statusY = findElement(posY,snake_length,foodY);

			if(!statusX || !statusY)
			break;
		}
		
		bigFoodCount++;
		if(bigFoodCount ==5)
		{
			bigFood = 1;
			food = 0;
			return;
		}
	}
	
	food = 0;	//this means we already have a food so dont generate a new food until its eaten
	glColor3f(1.0, 1.0, 1.0);
	glRectf(foodX, foodY, foodX + 1, foodY + 1);	// drawing food
}

void drawBigFood()
{
	if(bigFood)
		{
			int maxX = gridX - 4, maxY = gridY - 4;
			int min = 1;

			bigX = min + rand() % (maxX - min);
			bigY = min + rand() % (maxY - min);	

		}
	bigFood = 0;
	printf("%d\n",bigTimer);
	if(bigTimer++<30)
	{
		
		glColor3f(1.0, 0.0, 0.0);
		if(bigTimer %3==0)
			glRectf(bigX, bigY, bigX + 1, bigY + 1);
	}
	else{
		bigTimer = 0;
		bigFood = 0;
		bigFoodCount = 0;
		food = 1;
	}
}
void mousefunc(int b,int s, int x,int y){
	if(b == GLUT_LEFT_BUTTON && s == GLUT_DOWN)
		clicker = false;
}
//
//	#####################################################

void display_callback() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawGrid(); //draws grid lines every frame
	drawSnake();//draws the snake body in its new/current state for every frame
	drawFood();
	if(bigFoodCount == 5)
		drawBigFood();

	glutSwapBuffers();//swaps content from back buffer to front for display

	if (gameOver) { //game over end the program by displaying score
		char str_score[10];
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);
		char st_score[50];
		sprintf_s(st_score, "Your score is %d", score);

		glRasterPos3f(17.0f, 25.0f, 0.0f);
		for (int i = 0; st_score[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st_score[i]);

		glutSwapBuffers();
		glFlush();
		for (int i = 0; i < 1000000000; i++);
		exit(0);
	}
}


//	=====================================================

void reshape_callback(int w, int h) {
	glViewport(0, 0, w, h);
	/*
	glViewport(x,y,w,h)
	x,y -  lower left corner of the viewport rectangle
	w,h - takes value from the current dimensions of  window
	so when u resize the window it automatically sets dimensions
	*/

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//loads identity matrix
	glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

//	=====================================================

void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	initGrid(COLUMNS, ROWS);	// sets grid values
}

//	=====================================================

void keyboard(int key, int x, int y) {
	//printf("%d \n",key);
	switch (key) {

	//UP 101
	case GLUT_KEY_UP:
		//if snake is moving down then going up directly is impossible

		if (snake_direction != DOWN)
			snake_direction = key;
		break;
	//DOWN 103
	case GLUT_KEY_DOWN:
		//if snake is moving up then going down directly is impossible
		if (snake_direction != UP)
			snake_direction = key;
		break;
	//RIGHT 102
	case GLUT_KEY_RIGHT:
		//if snake is moving left then going right directly is impossible
		if (snake_direction != LEFT)
			snake_direction = key;
		break;
	//LEFT 100 
	case GLUT_KEY_LEFT:
		//if snake is moving right then going left directly is impossible
		if (snake_direction != RIGHT)
			snake_direction = key;
		break;
	}


}

//	=====================================================

void timer_callback(int a) {		//timer_callback(int)
	glutPostRedisplay();//this func is for how often we want to cal this function
	glutTimerFunc(1000 / FPS, timer_callback, 0);
	//i.e, 100 call this every 100ms 
}

//	=====================================================

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Snake Game 2D");
	init();
	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);

	//Special fun instead of keyboard bcuz keyboard gives char but special function gives a  predefined GL constant for up,down,right,left 
	glutSpecialFunc(keyboard); //void glutSpecialFunc(void (*func)(int key, int x, int y));

	glutTimerFunc(0, timer_callback, 0);
	glutMainLoop();
	return 0;
}



























//void drawFirst(){
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
//	glColor3f(1.0,0.0,0.0);
//	char click[50] = "Left click to Play!!!!";
//	char str[100] = "Vishweshwarayya Technological University, Belgaum,Karnataka";
//	char proj[50] = "Snake Game 2D";
//	char by [50] = "Ajay Deshpande & Dhanush C";
//	glRasterPos3f(17.0f, 25.0f, 0.0f);
//	for(int i = 0; str[i] != '\0';i++)
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
//	glRasterPos3f(20.0f,30.0f,0.0f);
//	for(int i = 0; proj[i] != '\0';i++)
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, proj[i]);
//	glRasterPos3f(28.0f,45.0f,0.0f);
//	for(int i = 0; click[i] != '\0';i++)
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, click[i]);
//	glRasterPos3f(50.0f,50.0f,0.0f);
//	for(int i = 0; by[i] != '\0';i++)
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, by[i]);
//	glFlush();
//	glutSwapBuffers();
//	for(int i = 0;i < 1000000000; i++);
//}
