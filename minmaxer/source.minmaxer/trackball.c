/*
 *	Implementation of a virtual trackball.
 *	Implemented by Gavin Bell, lots of ideas from Thant Tessman and
 *		the August '88 issue of Siggraph's "Computer Graphics," pp. 121-129.
 *
 */
#include "trackball.h"

/*
 * This size should really be based on the distance from the center of
 * rotation to the point on the object underneath the mouse.  That
 * point would then track the mouse as closely as possible.  This is a
 * simple example, though, so that is left as an Exercise for the
 * Programmer.
 */
#define TRACKBALLSIZE  (0.8)

/*
 * Local function prototypes (not defined in trackball.h)
 */
float tb_project_to_sphere(float, float, float);
void normalize_quat(float [4]);

/*
 * Ok, simulate a track-ball.  Project the points onto the virtual
 * trackball, then figure out the axis of rotation, which is the cross
 * product of P1 P2 and O P1 (O is the center of the ball, 0,0,0)
 * Note:  This is a deformed trackball-- is a trackball in the center,
 * but is deformed into a hyperbolic sheet of rotation away from the
 * center.  This particular function was chosen after trying out
 * several variations.
 * 
 * It is assumed that the arguments to this routine are in the range
 * (-1.0 ... 1.0)
 */
void
trackball(q, p1x, p1y, p2x, p2y)

     float q[4], p1x, p1y, p2x, p2y;

{
	float a[3];	/* Axis of rotation */
	float phi;	/* how much to rotate about axis */
	float p1[3], p2[3], d[3];
	float t;

	if (p1x == p2x && p1y == p2y)
	{
		vzero(q); q[3] = 1.0; /* Zero rotation */
		return;
	}

/*
 * First, figure out z-coordinates for projection of P1 and P2 to
 * deformed sphere
 */
	vset(p1,p1x,p1y,tb_project_to_sphere(TRACKBALLSIZE,p1x,p1y));
	vset(p2,p2x,p2y,tb_project_to_sphere(TRACKBALLSIZE,p2x,p2y));

/*
 *	Now, we want the cross product of P1 and P2
 */
	vcross(p2,p1,a);

/*
 *	Figure out how much to rotate around that axis.
 */
	vsub(p1,p2,d);
	t = vlength(d) / (2.0*TRACKBALLSIZE);
	/*
	 * Avoid problems with out-of-control values...
	 */
	if (t > 1.0) t = 1.0;
	if (t < -1.0) t = -1.0;
	phi = 2.0 * asin(t);

	axis_to_quat(a,phi,q);
}

/*
 *	Given an axis and angle, compute quaternion.
 */
void
axis_to_quat(a, phi, q)

     float a[3], phi, q[4];

{
	vnormal(a);
	vcopy(a,q);
	vscale(q,fsin(phi/2.0));
	q[3] = fcos(phi/2.0);
}

/*
 * Project an x,y pair onto a sphere of radius r OR a hyperbolic sheet
 * if we are away from the center of the sphere.
 */
static float
tb_project_to_sphere(r, x, y)

     float r, x, y;

{
	float d, t, z;

	d = fsqrt(x*x + y*y);
	if (d < r*M_SQRT1_2)  	/* Inside sphere */
	z = fsqrt(r*r - d*d);
	else
	{ 			/* On hyperbola */
		t = r / M_SQRT2;
		z = t*t / d;
	}
	return z;
}

/*
 * Given two rotations, e1 and e2, expressed as quaternion rotations,
 * figure out the equivalent single rotation and stuff it into dest.
 * 
 * This routine also normalizes the result every RENORMCOUNT times it is
 * called, to keep error from creeping in.
 *
 * NOTE: This routine is written so that q1 or q2 may be the same
 * as dest (or each other).
 */

#define RENORMCOUNT 97

void
add_quats(q1, q2, dest)

     float q1[4], q2[4], dest[4];

{
	static int count=0;
	int i;
	float t1[4], t2[4], t3[4];
	float tf[4];

	vcopy(q1,t1); 
	vscale(t1,q2[3]);

	vcopy(q2,t2); 
	vscale(t2,q1[3]);

	vcross(q2,q1,t3);
	vadd(t1,t2,tf);
	vadd(t3,tf,tf);
	tf[3] = q1[3] * q2[3] - vdot(q1,q2);

	dest[0] = tf[0];
	dest[1] = tf[1];
	dest[2] = tf[2];
	dest[3] = tf[3];

	if (++count > RENORMCOUNT)
	{
		count = 0;
		normalize_quat(dest);
	}
}

/*
 * Quaternions always obey:  a^2 + b^2 + c^2 + d^2 = 1.0
 * If they don't add up to 1.0, dividing by their magnitued will
 * renormalize them.
 *
 * Note: See the following for more information on quaternions:
 * 
 * - Shoemake, K., Animating rotation with quaternion curves, Computer
 *   Graphics 19, No 3 (Proc. SIGGRAPH'85), 245-254, 1985.
 * - Pletinckx, D., Quaternion calculus as a basic tool in computer
 *   graphics, The Visual Computer 5, 2-13, 1989.
 */
static void
normalize_quat(q)

     float q[4];

{
	int i;
	float mag;

	mag = (q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
	for (i = 0; i < 4; i++) q[i] /= mag;
}

/*
 * Build a rotation matrix, given a quaternion rotation.
 *
 */
void
build_rotmatrix(m, q)

     float m[4][4], q[4];

{
	m[0][0] = 1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]);
	m[0][1] = 2.0 * (q[0] * q[1] - q[2] * q[3]);
	m[0][2] = 2.0 * (q[2] * q[0] + q[1] * q[3]);
	m[0][3] = 0.0;

	m[1][0] = 2.0 * (q[0] * q[1] + q[2] * q[3]);
	m[1][1] = 1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]);
	m[1][2] = 2.0 * (q[1] * q[2] - q[0] * q[3]);
	m[1][3] = 0.0;

	m[2][0] = 2.0 * (q[2] * q[0] - q[1] * q[3]);
	m[2][1] = 2.0 * (q[1] * q[2] + q[0] * q[3]);
	m[2][2] = 1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]);
	m[2][3] = 0.0;

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}
