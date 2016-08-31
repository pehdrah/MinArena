#include "collisions.h"
#include <iostream>

using namespace std;

/* 
 * Verify Effects of Collisions
 */
bool collisionEffect(Object* o1, Object* o2)
{
	unsigned char type1, type2;
	bool value;

	value = o1->collision(o2);
	type1 = o1->getType();
	type2 = o2->getType();

	if(value)
	{
		if(type1 == 0)
		{
			if(type2 == 1)
			{
				collisionImmovable(o2, o1);
			}
		}
		else if(type1 == 1)
		{
			if(type2 == 0)
			{
				collisionImmovable(o1, o2);
			}
			else if(type2 == 1)
			{
				collisionObstruse(o1, o2);
			}
			else if(type2 == 2 && o2->source != o1->id)
			{
				collisionAttack(o1, o2);
			}
		}
		else if(type1 == 2)
		{
			if(type2 == 1 && o1->source != o2->id)
			{
				collisionAttack(o2, o1);
			}
		}
	}

	return value;
}

/*
 * Collision optimized for immovable objects
 */
void collisionImmovable(Object *movable, Object *imovable)
{
	bool xCollision;
	ObjectStats *m, *i;
	Rect mb, ib;
	char mdx, mdy;

	m = movable->stats;
	mb = movable->getBox();
	mdx = m->ps->dx;
	mdy = m->ps->dy;

	i = imovable->stats;
	ib = imovable->getBox();

	mb.x = mb.x - mdx;
	mb.y = mb.y - mdy;

	xCollision = false;
	if(mb.y + mb.h > ib.y && mb.y < ib.y + ib.h)
	{
		xCollision = true;
	}

	mb.x = mb.x + mdx;
	mb.y = mb.y + mdy;

	if(xCollision)
	{
		if(mdx > 0)
		{
			mb.x = ib.x - mb.w;
		}
		else if(mdx < 0)
		{
			mb.x = ib.x + ib.w;
		}
		else
		{
			if(mb.x > ib.x)
			{
				mb.x = ib.x + ib.w;
			}
			else
			{
				mb.x = ib.x - mb.w;
			}
		}
		mdx = 0;
	}
	else
	{
		if(mdy > 0)
		{
			mb.y = ib.y - mb.h;
		}
		else if(mdx < 0)
		{
			mb.y = ib.y + ib.h;
		}
		else
		{
			if(mb.y > ib.y)
			{
				mb.y = ib.y + ib.h;
			}
			else
			{
				mb.y = ib.y - mb.h;
			}
		}
		mdy = 0;
	}

	movable->setPosition(mb.x, mb.y);
	movable->setSpeed(mdx, mdy);
	movable->updateGeometry();
}

/*
 * Collision between obstrusing objects
 */
void collisionObstruse(Object *o1, Object *o2)
{
	bool xCollision, swapped;
	float relative, distance;
	char ldx, ldy, rdx, rdy;
	ObjectStats *l, *r;
	PrimaryStats *lp, *rp;
	Rect lb, rb;
	Object *a;

	o1->moveBackwardsX();
	o2->moveBackwardsX();

	xCollision = true;
	if(o1->collision(o2))
	{
		xCollision = false;
	}

	if(xCollision)
	{
		swapped = false;		
		distance = lb.x + lb.w - rb.x;

		l = o1->stats;
		r = o2->stats;
		lb = o1->getBox();
		rb = o2->getBox();

		if(rb.x < lb.x)
		{
			l = o2->stats;
			r = o1->stats;
			lb = o2->getBox();
			rb = o1->getBox();
			swapped = true;
		}

		lp = l->ps;
		rp = r->ps;
		ldx = lp->dx;
		rdx = rp->dx;
		ldy = lp->dy;
		rdy = rp->dy;
		lb.x += ldx;
		rb.y += rdy;

		relative = rdx - ldx;
		if(ldx * rdx >= 0)
		{
			if(ldx*ldx > rdx*rdx)
			{
				lb.x = rb.x - lb.w;
				ldx = rdx;
			}
			else
			{
				rb.x = lb.x + lb.w;
				rdx = ldx;
			}
		}
		else
		{
			if(relative != 0)
			{
				ldx = ldx*(distance)/relative;
				lb.x = lb.x - ldx + ldx;
			}
			rdx = lb.x + lb.w - rb.x + rdx;
			rb.x = lb.x + lb.w;
		}
	}
	else
	{
		o1->moveBackwardsY();
		o2->moveBackwardsY();
		swapped = false;
		distance = lb.y + lb.h - rb.y;

		l = o1->stats;
		r = o2->stats;
		lb = o1->getBox();
		rb = o2->getBox();

		if(rb.y < lb.y)
		{
			l = o2->stats;
			r = o1->stats;
			lb = o2->getBox();
			rb = o1->getBox();
			swapped = true;
		}

		lp = l->ps;
		rp = r->ps;
		ldy = lp->dy;
		rdy = rp->dy;
		ldx = lp->dx;
		rdx = rp->dx;
		lb.y += ldy;
		rb.y += rdy;

		relative = rdy - ldy;
		if(ldy * rdy >= 0)
		{
			if(ldy*ldy > rdy*rdy)
			{
				lb.y = rb.y - lb.h;
				ldy = rdy;
			}
			else
			{
				rb.y = lb.y + lb.h;
				rdy = ldy;
			}
		}
		else
		{
			if(relative != 0)
			{
				ldy = ldy*(distance)/relative;
				lb.y = lb.y - ldy + ldy;
			}
			rdy = lb.y + lb.h - rb.y + rdy;
			rb.y = lb.y + lb.h;
		}
	}

	if(!swapped)
	{
		o1->setPosition(lb.x, lb.y);
		o2->setPosition(rb.x, rb.y);
		o1->setSpeed(ldx, ldy);
		o2->setSpeed(rdx, rdy);
	}
	else
	{
		o2->setPosition(lb.x, lb.y);
		o1->setPosition(rb.x, rb.y);
		o2->setSpeed(ldx, ldy);
		o1->setSpeed(rdx, rdy);
	}

	o1->updateGeometry();
	o2->updateGeometry();
}

/*
 * Collision between player and attack objects
 *
 * o1 is a creature(1) type of object, o2 is the attack(2) type object
 */
void collisionAttack(Object *o1, Object *o2)
{
	float xDistance, yDistance, xRatio, yRatio;
	PrimaryStats* ps;

	ps = o1->getPrimaryStats();

	xDistance = o1->surface->center.x - o2->surface->center.x;
	yDistance = o1->surface->center.y - o2->surface->center.y;

	xRatio = xDistance*xDistance;
	yRatio = yDistance*yDistance;

	xRatio = (o2->stats->ss.melee)*o2->stats->ss.melee*xDistance/(xRatio);
	yRatio = (o2->stats->ss.melee)*o2->stats->ss.melee*yDistance/(yRatio);

	ps->dx = ps->dx + (char)(xRatio);
	ps->dy = ps->dy + (char)(yRatio);

	ps->life = ps->life - o2->stats->ss.melee;
	ps->state = ps->state | 0x40;
	o1->stats->ss.ticks = 0;
}
