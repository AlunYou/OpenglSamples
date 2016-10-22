// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "lab.h"
#include "ChildView.h"
#include "jitter.h"
#include "accjitter.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const GLfloat  AMBIENT[] = {0.2f, 0.2f, 0.2f, 1.0f};

const GLfloat  LIGHT0LOC[]    = {1.f, 0.5f, 1.f, 0.f};
const GLfloat  LIGHT0DCOLOR[] = {1.f, 1.f, 1.f, 1.f};
const GLfloat  LIGHT0SCOLOR[] = {1.f, 1.f, 1.f, 1.f};
const GLfloat  LIGHT1LOC[]    = {-2.f, 0.4f, 1.f, 0.f};
const GLfloat  LIGHT1DCOLOR[] = {1.f, 1.f, 1.f, 1.f};
const GLfloat  LIGHT1SCOLOR[] = {1.f, 1.f, 1.f, 1.f};
const GLfloat  LIGHT2LOC[]    = {120.f, 90.f, 120.f, 1.f};
const GLfloat  LIGHT2DCOLOR[] = {1.f, 1.f, 1.f, 1.f};
const GLfloat  LIGHT2SCOLOR[] = {1.f, 1.f, 1.f, 1.f};

const double GR_PI = 3.1415926535897932384626433832795;

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
   m_camera.Set(-10, 15, 30, 0, 0, 0, 0, 1, 0);
   m_camera.MouseMode(CGrCamera::PITCHYAW);

   SetDoubleBuffer(true);
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,COpenGLWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_COMMAND(ID_FILE_SAVEBMPFILE, OnFileSavebmpfile)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!COpenGLWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}



//
// Name :         CChildView::OnGLDraw(CDC *pDC)
// Description :  Redraw all OpenGL content.
//

void CChildView::OnGLDraw(CDC *pDC)
{
	//glDrawBuffer(GL_FRONT);

    CWaitCursor   wait;

   //
   // Some standard parameters
   //

   // Enable depth test
   glEnable(GL_DEPTH_TEST);

   // Cull backfacing polygons
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);

   // Determine the screen size so we can determine the aspect ratio
   int width, height;
   GetSize(width, height);
   GLdouble aspectratio = GLdouble(width) / GLdouble(height);

   //////////////////////////////////////////////////////////////////
   //
   // The following code section is what it takes to draw a single
   // image with the camera set up and everything.  
   //

   //
   // Set up the camera 
   //

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set the camera parameters
   gluPerspective(m_camera.FieldOfView(),    // Vertical field of view in degrees.
                  aspectratio,   // The aspect ratio.
                  20.,           // Near clipping
                  1000.);         // Far clipping

   // Set the camera location
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   m_camera.gluLookAt();

   ActualDraw();

  glClear(GL_ACCUM_BUFFER_BIT);
   for (int i = 0; i<4; i++)
   {
	   glPushMatrix();
	   glRotated(i * 10., 0, 1, 0);
	   ActualDraw();
	   glPopMatrix();

		glAccum(GL_ACCUM, 0.25);
   }

   glAccum(GL_RETURN, 1.0);

   glFlush();
}


//
// Name :         CChildView::ActualDraw()
// Description :  This function does all of the actual OpenGL drawing.  
//

void CChildView::ActualDraw()
{
   // Enable lighting
   glEnable(GL_LIGHTING);

   GLfloat black[] = {0.f, 0.f, 0.f, 1.0f};

   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_POSITION, LIGHT0LOC);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT0DCOLOR);
   glLightfv(GL_LIGHT0, GL_SPECULAR, LIGHT0SCOLOR);
   glLightfv(GL_LIGHT0, GL_AMBIENT, AMBIENT);

   GLfloat lightpos1[] = {-1, 0.5, 1, 0.};

   glEnable(GL_LIGHT1);
   glLightfv(GL_LIGHT1, GL_POSITION, LIGHT1LOC);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, LIGHT1DCOLOR);
   glLightfv(GL_LIGHT1, GL_SPECULAR, LIGHT1SCOLOR);

   glEnable(GL_LIGHT2);
   glLightfv(GL_LIGHT2, GL_POSITION, LIGHT2LOC);
   glLightfv(GL_LIGHT2, GL_DIFFUSE, LIGHT2DCOLOR);
   glLightfv(GL_LIGHT2, GL_SPECULAR, LIGHT2SCOLOR);

   // Clear the screen
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   for(double x=-10;  x<=10;  x+=1.0)
      for(double z=-10;  z<=10;  z+= 1.0)
      {
         GLfloat clr[] = {float(0.3 + x / 50), float(0.3 + z / 50.),  0.3f, 1.0f};
         glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, clr);
         Cone(x, 0, z, .3, 1, 30);
      }

}


void CChildView::OnFileSavebmpfile() 
{
   OnSaveImage();	
}




void CChildView::OnMouseMove(UINT nFlags, CPoint point) 
{
   if(nFlags & MK_LBUTTON)
   {
      m_camera.MouseMove(point.x, point.y);
      Invalidate();
   }

	COpenGLWnd ::OnMouseMove(nFlags, point);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   m_camera.MouseDown(point.x, point.y);
	
	COpenGLWnd ::OnLButtonDown(nFlags, point);
}

//
// Name :         CChildView::Cone()
// Description :  Draw a simple code.  I thought this might be 
//                rather fun object to play with in this lab since
//                it has a nice, point to it.
//

void CChildView::Cone(double p_x, double p_y, double p_z, 
                      double p_radius, double p_height, int p_cnt)
{
   glBegin(GL_QUAD_STRIP);

   for(int i=0;  i<=p_cnt;  i++)
   {
      double angle0 = double(i) / double(p_cnt) * GR_PI * 2.;

      CGrPoint n(cos(angle0), p_radius / p_height, -sin(angle0));
      n.Normalize3();

      glNormal3dv(n);

      glVertex3d(p_x, p_y + p_height, p_z);
      glVertex3d(p_x + cos(angle0) * p_radius, p_y, p_z - sin(angle0) * p_radius);
   }

   glEnd();
}



