/*
	File: Shape.cc
	Copyright (C) 2008 Arnd Begemann

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "Shape.h"
#include "assert.h"
#include <cmath>
Shape::Shape()
{
}
Shape::~Shape()
{
	// TODO: put destructor code here
}

bool Shape::isCircle()
{
	// since just arcs could be a circle
	return false;
}
Point2D Shape::getStartTangentPoint(double length, double offset, bool right_side)
{
	// first calculate the offset
	// positive offset -> line
	// negative offset <- line (-180 Deg)
	Point2D p_offset_start_line;
	double offset_angle=this->getContourStartAngle()/180.0*M_PI;
	p_offset_start_line.x=this->getStartPoint().x+cos(offset_angle)*offset;
	p_offset_start_line.y=this->getStartPoint().y+sin(offset_angle)*offset;
	// calculate Tangente
	double angle_tanget=0.0;
	if(right_side)
	{
		angle_tanget=(this->getContourStartAngle() -90.0)/180.0*M_PI;
	}
	else
	{
		angle_tanget=(this->getContourStartAngle() +90.0)/180.0*M_PI;
	}
	Point2D p_tangent;
	p_tangent.x=p_offset_start_line.x+cos(angle_tanget)*length;
	p_tangent.y=p_offset_start_line.y+sin(angle_tanget)*length;
	return p_tangent;	
}

/*
 * Return the center point of a circle of given radius that is tangent
 * to the shape at the end point.
 *
 *   length        radius of circle
 *   offset        distance to offset endpoint along the line of motion;
 *                 positive is forward, negative is reverse
 *   right_side    true for the tangent circle on the right side of the
 *                 motion vector, false for the left side
 */
Point2D Shape::getEndTangentPoint(double length, double offset, bool right_side)
{
	// first cacualte the offset
	// positive offset -> line
	// negative offset <- line (-180 Deg)
	Point2D p_offset_end_line;
	double offset_angle=this->getContourEndAngle()/180.0*M_PI;
	p_offset_end_line.x=this->getEndPoint().x+cos(offset_angle)*offset;
	p_offset_end_line.y=this->getEndPoint().y+sin(offset_angle)*offset;

  // calculate Tangent
	double angle_tanget=0.0;
	if(right_side)
	{
		angle_tanget=(this->getContourEndAngle() -90.0)/180.0*M_PI;
	}
	else
	{
		angle_tanget=(this->getContourEndAngle() +90.0)/180.0*M_PI;
	}
	Point2D p_tangent;
	p_tangent.x=p_offset_end_line.x+cos(angle_tanget)*length;
	p_tangent.y=p_offset_end_line.y+sin(angle_tanget)*length;
	return p_tangent;	
}

/*
  Write g-code for an entry move at the end point (why the end point??)
  Tool size compensation is turned on in the course of this move.
  Before this move is executed, Z should be at a safe position for fast
  motion and tool comp should be off.
 
  For an OUTSIDE cut, the move consists of the following steps:

    1. fast move x,y to "tool start point"
    2. fast move z to 0
    3. slow plunge z to cut height
    4. turn on tool comp
    5. slow move x,y to start of entry arc
    6. slow entry arc
 
  For an INSIDE cut, the move consists of the following steps:

    1. fast move x,y to "tool start point"
    2. turn on tool comp
    3. slow move x,y to start of entry arc
    4. fast move z to 0
    5. slow plunge z to cut height
    6. slow entry arc

  The "tool start point" is the center of the tool when it is tangent
  to the end point of the shape, offset by MIN_OFFSET_ENTRY_MOVE perpendicular
  to the shape AND again in the reverse direction of motion.
 */
std::ostream& Shape::getEndCncEntryMove(std::ostream &os, double z_zero_pos, double z_cut_pos)
{
	assert(m_property);

  if(m_property->cutLeft() || m_property->cutRight())
	{
		os << "(Entry Move Start)" << std::endl;
		double tool_radius = 0.0;

    if (m_property->getTool())
			tool_radius = m_property->getTool()->getDiameter()/2;

    double arc_radius = tool_radius + ENTRY_ARC_EXTRA_RADIUS;
    
    Point2D p_start;
		Point2D p_start_arc;
		Point2D p_center_arc;

    // center point of entry arc
    p_center_arc = this->getEndTangentPoint(arc_radius,
                                            0.0,
                                            m_property->cutRight());

    // start point of entry arc
    p_start_arc=this->getEndTangentPoint(arc_radius,
                                         -arc_radius,
                                         m_property->cutRight());

    // start point of tool comp move (tangent to entry arc to avoid EMC errors)
		p_start = this->getEndTangentPoint(arc_radius+tool_radius,
                                       -arc_radius,
                                       m_property->cutRight());

		Point2D p_offset_arc_center = p_center_arc - p_start_arc;

    // fast move to safe Z height
    os << "G0" << " Z" << z_zero_pos << std::endl;

    // fast move to start point
		os << "G0" << " X" << p_start.x << " Y" << p_start.y << std::endl;

    // set cut feed rate
		if (m_property->getCutFeedRate() != 0.0)
      os << "F" << m_property->getCutFeedRate() << std::endl;

    // enable tool compensation and move to start of entry arc
    if (m_property->cutRight()) {
			os << "G42" << " G1" << " X" << p_start_arc.x
                           << " Y" << p_start_arc.y << std::endl;			
		}
		else {
			os << "G41" << " G1" << " X" << p_start_arc.x
                           << " Y" << p_start_arc.y << std::endl;
		}

    // set plunge feed rate
    if (m_property->getZFeedRate() != 0.0)
        os << "F" << m_property->getZFeedRate() << std::endl;

    // DIVE DIVE AWOOGA AWOOGA
    os << "G1" << " Z" << z_cut_pos << std::endl;

    // set cut feed rate
		if (m_property->getCutFeedRate() != 0.0)
      os << "F" << m_property->getCutFeedRate() << std::endl;

    // cut entry arc
    if (m_property->cutRight()) {
			os << "G2"; //CW entry arc
		}
		else {
			os << "G3"; //CCW entry arc
		}

    os << " X" << this->getEndPoint().x << " Y" << this->getEndPoint().y
		   << " I" << p_offset_arc_center.x << " J" << p_offset_arc_center.y << std::endl;

    os << "(Entry Move End)" << std::endl;
	}
  
	return os;
}
