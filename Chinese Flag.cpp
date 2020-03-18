#include <Windows.h>
#include <gl/glut.h>
#include <cmath>
const GLfloat PI = 3.1415926536f;

void init(GLvoid) {
	glShadeModel(GL_SMOOTH);				//enable smooth shading 启用阴影平滑
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);			//red background 
	glClearDepth(1.0f);					//depth buffer setup 设置深度缓存
	glEnable(GL_DEPTH_TEST);				//enable depth testing 启用深度测试
	glDepthFunc(GL_LEQUAL);					//the type of depth testing to do 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//tell the system to correct the perspective 告诉系统对透视进行修正
}

/*绘制五角星*/
void drawStar(GLfloat px, GLfloat py, GLfloat vx, GLfloat vy,int flag) {
	
	/*为求五角星所有顶点做准备*/
	GLfloat len_long, len_short, vtx[12],vty[12],angle;
	len_long = sqrt(pow(vx - px, 2) + pow(vy - py, 2));
	len_short = len_long * sin(18 * PI / 180) / sin(126 * PI / 180);
	vtx[0] = px, vty[0] = py;						//五角星中心点
	vtx[1] = vx, vty[1] = vy;						//五角星已知的一个外顶点
	angle = atan((vty[1] - vty[0]) / (vtx[1] - vtx[0]));			//求外顶点和中心点构成的边与x轴的角度

	/*求剩下顶点的坐标*/
	if (flag)						
		angle -= PI;							//为啥加这一步？？  还没想明白
	for (int i = 2; i < 12; i++) {
		angle -= 36 * PI / 180;						//顺时针求顶点坐标
		if (i % 2) {	//外顶点
			vtx[i] = vtx[0] + len_long * cos(angle);
			vty[i] = vty[0] + len_long * sin(angle);
		}
		else {			//内顶点
			vtx[i] = vtx[0] + len_short * cos(angle);
			vty[i] = vty[0] + len_short * sin(angle);
		}
	}

	/*绘制五角星*/
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 12; i++)
		glVertex2f(vtx[i], vty[i]);
	glEnd();
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			//clear screen and depth buffer

	/*绘制国旗红色矩形*/
	glColor3f(1.0f, 0.0f, 0.0f);						//red background setup
	glBegin(GL_QUADS);							//begin to draw quad
	glVertex2f(-0.9, 0.6);							//二维坐标  相对于窗口比例 1.0,1.0是右上角
	glVertex2f(0.9, 0.6);
	glVertex2f(0.9, -0.6);
	glVertex2f(-0.9, -0.6);
	glEnd();								//end drawing

	/*绘制黄色五角星*/
	GLfloat														
		px[5] = { -0.6,-0.3,-0.18,-0.18,-0.3 },					
		py[5] = { 0.3,0.48,0.36,0.18,0.06}, 
		vx[5], vy[5];									
	vx[0] = -0.6, vy[0] = 0.48;

	for (int i = 1; i < 5; i++) {
		GLfloat temp = atan((py[i] - py[0]) / (px[i] - px[0]));
		vx[i] = px[i] - 0.06 * cos(temp);
		vy[i] = py[i] - 0.06 * sin(temp);
	}

	glColor3f(1.0f, 1.0f, 0.0f);
	drawStar(px[0], py[0], vx[0], vy[0], 0);				//绘制大星星
	for (int i = 1; i < 5; i++)						//绘制4个小星星
		drawStar(px[i],py[i],vx[i], vy[i],1);

	glutSwapBuffers();							//交换缓冲区  把绘制的最终结果从后台缓冲复制到屏幕上
}

void reshape(int w, int h) {	//w,h是哪里传来的？

	//if (h == 0)
		//h = 1;//有啥用？？	
	glViewport(0, 0, w, h);							//视口变换 按照窗体大小制作OpenGL屏幕
	glMatrixMode(GL_PROJECTION);						//投影变换
	glLoadIdentity();							//把当前矩阵设置为单位矩阵

	//下面代码找时间研究一下！
	//int minn = w < h ? w : h;
	//glViewport(0, 0, minn, minn);
	if (w <= h)		//根据短的一边来确定绘图比例  用w<=h作为判断条件是因为初始窗口是正方形  自己的理解  正确性找时间再判断
		glOrtho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w, 1.0, -1.0);
	else
		glOrtho(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w/ (GLfloat)h, -1.0, 1.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(int argc, char** argv) {						//parameters can't be omitted 参数不可省略
	glutInit(&argc, argv);							//glut init 对glut进行初始化
	init();
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);				//display mode setup 设置显示模式：使用RGB颜色、使用双缓冲
	glutInitWindowSize(600, 600);						//window size
	glutCreateWindow("Chinese Flag");   //create a window according to the above info and setup title;window show until glutMainLoop
	glutDisplayFunc(display);						//if u draw,use it
	glutReshapeFunc(reshape);						//防止因窗口改变而图形变形

	glutMainLoop();
}
