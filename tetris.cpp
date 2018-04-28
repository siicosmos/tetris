#include "include/Angel.h"
#include "constants.h"
#include "tiles.h"
#include <sys/time.h>
#include <unistd.h>

bool gameOver = false;
vec2 points[TotalPoints];
vec4 colors[TotalPoints];
tiles tiles_g;
board board_g;

std::uint32_t Base = 0;

void init(void)
{
	// points
	for(int i = 0; i < NumHoriLines; i++)
	{
		points[i*2  ] = vec2(-wide, -height + BlockHeight * i);
		points[i*2+1] = vec2( wide, -height + BlockHeight * i);
	}
	for (int i = 0; i < NumVertLines; i++)
	{
		points[NumHoriPoints+i*2  ] = vec2(-wide + BlockWide * i, -height);
		points[NumHoriPoints+i*2+1] = vec2(-wide + BlockWide * i,  height);
	}

	// colors
	for (int i = 0; i < NumHoriPoints + NumVertPoints; i++)
		colors[i] = vec4( 0.3, 0.3, 0.3, 1.0 );

	// for render stuffs
	// create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// create and initialize a vertex buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW);

	// separate the vertec buffer into subbuffers
    glBufferSubData(GL_ARRAY_BUFFER, 0, ColorsOffset, points);
    glBufferSubData(GL_ARRAY_BUFFER, ColorsOffset, sizeof(colors), colors);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program); // Installs a program object as part of current rendering state

    // Initialize the vertex position attribute from the vertex shader
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(ColorsOffset));

	glClearColor( 0, 0, 0, 1.0 );
}

void resetGame(void)
{
    tiles_g.interval = DefInterval;
    tiles_g.reset();
    board_g.reset();
    gameOver = false;
}

void display(void)
{
    if(!gameOver)
    {
        tiles_g.writeBuffer();
        board_g.writeBuffer();

        // clear the window
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_CULL_FACE);

        // draw the grid
        glDrawArrays(GL_LINES, 0, BeginTetroPoints);
        // draw current tetromino
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLE_STRIP, BeginTetroPoints, NumOfTetroPoints);
        // draw bottom blocks
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLE_STRIP, BeginBoardPoints, board_g.numPoints);

        // force execution of GL commands in finite time
        glutSwapBuffers();
        glFlush();
    }
    else
    {
        glutSwapBuffers();
        glFlush();
    }
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void keyboard(unsigned char key, int i, int j)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			tiles_g.left();
			break;
		case GLUT_KEY_RIGHT:
			tiles_g.right();
			break;
		case GLUT_KEY_UP:
			tiles_g.rotate();
			break;
		case GLUT_KEY_DOWN:
			tiles_g.down();
            break;
		case 'r': case 'R':
			resetGame();
            break;
        case ESC_27: case ESC_33: case 'q': case 'Q':
            exit(EXIT_SUCCESS);
			break;
	}
}

void keyboardSpecial(int key, int i, int j)
{
	keyboard(key, i, j);
}

void idle()
{
    usleep(30);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
	timeval T;
	gettimeofday(&T, NULL);
	srand((unsigned)(T.tv_sec * 1000 + T.tv_usec));
    
    tiles_g.board_t = &board_g;

    glutInit(&argc, argv);
	#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE);
	#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	#endif

	// If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    // glutInitContextVersion( 3, 2 );
    // glutInitContextProfile( GLUT_CORE_PROFILE );
    // Iff you get a segmentation error at line 34, please uncomment the line below
    // glewExperimental = GL_TRUE; 
    // glewInit();

    glutInitWindowSize(300, 600);
	glutCreateWindow("Tetris");

	#ifndef __APPLE__
    glewInit();
	#endif

    resetGame();
	// initialization function for my data
	init();
	glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}
