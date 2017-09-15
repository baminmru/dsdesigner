#ifndef LONGRECT_H
#define LONGRECT_H

////////////////////////////////////////////////////////////////////////////////
/*
//
// class TPointL
// ~~~~~ ~~~~~~~
class  TPointL : public POINTL {
	public:
		// Constructors
		//
		TPointL();
		TPointL(long _x, long _y);
		TPointL(const POINTL far& point);
		TPointL(const TPointL far& point);

		// Information functions/operators
		//
		bool        operator ==(const TPointL& other) const;
		bool        operator !=(const TPointL& other) const;
		long        X() const;
		long        Y() const;

		// Functions/binary-operators that return points or sizes
		//
		TPointL     OffsetBy(long dx, long dy) const;
		TPointL     operator +(const ClassLib::ClassLib::TSize& size) const;
		TPointL     operator -(const TPointL& point) const;
		TPointL     operator -(const ClassLib::TSize& size) const;
		TPointL     operator -() const;

		// Functions/assignement-operators that modify this point
		//
		TPointL&    Offset(long dx, long dy);
		TPointL&    operator +=(const ClassLib::TSize& size);
		TPointL&    operator -=(const ClassLib::TSize& size);
};



//----------------------------------------------------------------------------
// TPointL
//

//
// Default constructor that does nothing.
//
inline
TPointL::TPointL()
{
}

//
// Construct the point to a specific location.
//
inline
TPointL::TPointL(long _x, long _y)
{
  x = _x;
  y = _y;
}

//
// Alias constructor that initializes from an existing point.
//
inline
TPointL::TPointL(const POINTL far& point)
{
  x = point.x;
  y = point.y;
}

//
// Copy constructor. Makes a copy of the location.
//
inline
TPointL::TPointL(const TPointL far& point)
{
  x = point.x;
  y = point.y;
}

//
// Return the X component of the point.
//
inline long
TPointL::X() const
{
  return x;
}

//
// Return the Y component of the point.
//
inline long
TPointL::Y() const
{
  return y;
}

//
// Return the new point (x+dx, y+dy).
//
inline TPointL
TPointL::OffsetBy(long dx, long dy) const
{
  return TPointL(x + dx, y + dy);
}

//
// Return the negative of the point.
//
inline TPointL
TPointL::operator -() const
{
	return TPointL(-x, -y);
}

//
// Return true if positions are the same.
//
inline bool
TPointL::operator ==(const TPointL& other) const
{
	return (bool)(other.x == x && other.y == y);
}

//
// Return true if the positions are not the same.
//
inline bool
TPointL::operator !=(const TPointL& other) const
{
	return (bool)(other.x != x || other.y != y);
}

//
// Return the new point (x+cx, y+cy).
//
inline TPointL
TPointL::operator +(const ClassLib::TSize& size) const
{
	return TPointL(x + size.cx, y + size.cy);
}

//
// Return the difference between the two points.
//
inline TPointL
TPointL::operator -(const TPointL& point) const
{
	return TPointL(x - point.x, y - point.y);
}

//
// Return the new point (x-cx, y-cy).
//
inline TPointL
TPointL::operator -(const ClassLib::TSize& size) const
{
	return TPointL(x - size.cx, y - size.cy);
}

//
// Return the point (x+dx, y+dy).
//
inline TPointL&
TPointL::Offset(long dx, long dy)
{
  x += dx;
  y += dy;
  return *this;
}

//
// Return the point (x+cx, y+cy).
//
inline TPointL&
TPointL::operator +=(const ClassLib::TSize& size)
{
  x += size.cx;
  y += size.cy;
  return *this;
}

//
// Return the point (x-cx, y-cy).
//
inline TPointL&
TPointL::operator -=(const ClassLib::TSize& size)
{
  x -= size.cx;
  y -= size.cy;
	return *this;
}

*/

struct tagRECTL {
		long left;
		long top;
		long right;
		long bottom;
};


class  TRectL : public tagRECTL {
	public:
		// Constructors
		//
		TRectL();
		TRectL(const tagRECT far& rect);
		TRectL(const ClassLib::TRect far& rect);

		TRectL(const tagRECTL far& rect);
		TRectL(const TRectL far& rect);

		TRectL(long _left, long _top, long _right, long _bottom);
		TRectL(const TPointL& upLeft, const TPointL& loRight);
		TRectL(const TPointL& origin, const ClassLib::TSize& extent);

		// (Re)Initializers
		//
		void        SetNull();
		void        SetEmpty();
		void        Set(long _left, long _top, long _right, long _bottom);
		void        SetWH(long _left, long _top, long w, long h);

		// Type Conversion operators
		//
		operator    const TPointL*() const;
		operator    TPointL*();

		operator    ClassLib::TRect ();
		operator    ClassLib::TRect* ();


		// Testing functions
		//
		bool        IsEmpty() const;
		bool        IsNull() const;
		bool        operator ==(const TRectL& other) const;
		bool        operator !=(const TRectL& other) const;

		// Information/access functions (const and non-const)
		//
		long         Left() const;
		long         X() const;
		long         Top() const;
		long         Y() const;
		long         Right() const;
		long         Bottom() const;

		const TPointL& TopLeft() const;
		TPointL&     TopLeft();
		TPointL      TopRight() const;
		TPointL      BottomLeft() const;
		const TPointL& BottomRight() const;
		TPointL&     BottomRight();

		long         Width() const;
		long         Height() const;
		ClassLib::TSize       Size() const;
		long        Area() const;

		bool        Contains(const TPointL& point) const;
		bool        Contains(const TRectL& other) const;
		bool        Touches(const TRectL& other) const;
		TRectL       OffsetBy(long dx, long dy) const;
		TRectL       operator +(const ClassLib::TSize& size) const;
		TRectL       operator -(const ClassLib::TSize& size) const;
		TRectL       MovedTo(long x, long y);
		TRectL       InflatedBy(long dx, long dy) const;
		TRectL       InflatedBy(const ClassLib::TSize& size) const;
		TRectL       Normalized() const;
		TRectL       operator &(const TRectL& other) const;
		TRectL       operator |(const TRectL& other) const;

		long         Subtract(const ClassLib::TRect& other, ClassLib::TRect result[]) const;

		// Manipulation functions/operators
		//
		TRectL&      Normalize();
		TRectL&      Offset(long dx, long dy);
		TRectL&      operator +=(const ClassLib::TSize& delta);
		TRectL&      operator -=(const ClassLib::TSize& delta);
		TRectL&      MoveTo(long x, long y);
		TRectL&      Inflate(long dx, long dy);
		TRectL&      Inflate(const ClassLib::TSize& delta);
		TRectL&      operator &=(const ClassLib::TRect& other);
		TRectL&      operator |=(const ClassLib::TRect& other);
};

inline
TRectL::TRectL()
{
	left=top=bottom=right=0;
}

//
// Create an empty rectangle.
//
inline void
TRectL::SetEmpty()
{
	SetNull();
}


inline
TRectL::operator ClassLib::TRect()
{
	ClassLib::TRect tt;
	tt.left=left;
	tt.right=right;
	tt.top=top;
	tt.bottom=bottom;
	return tt;
};

inline
TRectL::operator ClassLib::TRect*()
{

	ClassLib::TRect *tt =new ClassLib::TRect();
	tt->left=left;
	tt->right=right;
	tt->top=top;
	tt->bottom=bottom;
	return tt;
};


//
// Return an array of two points (upperleft and bottomright)
//
inline
TRectL::operator const TPointL*() const
{
	return (const TPointL*)this;
}

//
// Return an array of two points (upperleft and bottomright)
//
inline
TRectL::operator TPointL*()
{
	return (TPointL*)this;
}

//
// Return the left value.
//
inline long
TRectL::Left() const
{
	return left;
}

//
// Return the left value.
//
inline long
TRectL::X() const
{
	return left;
}

//
// Return the top value.
//
inline long
TRectL::Top() const
{
	return top;
}

//
// Return the top value.
//
inline long
TRectL::Y() const
{
  return top;
}

//
// Return the right value.
//
inline long
TRectL::Right() const
{
  return right;
}

//
// Return the bottom value.
//
inline long
TRectL::Bottom() const
{
  return bottom;
}

//
// Return the upperleft point.
//
inline const TPointL&
TRectL::TopLeft() const
{
  return *(TPointL*)&left;
}

//
// Return the upperleft point.
//
inline TPointL&
TRectL::TopLeft()
{
  return *(TPointL*)&left;
}

//
// Return the upperright point.
//
inline TPointL
TRectL::TopRight() const
{
  return TPointL(right, top);
}

//
// Return the bottomleft point.
//
inline TPointL
TRectL::BottomLeft() const
{
  return TPointL(left, bottom);
}

//
// Return the bottomright point.
//
inline const TPointL&
TRectL::BottomRight() const
{
  return *(TPointL*)&right;
}

//
// Return the bottomright point.
//
inline TPointL&
TRectL::BottomRight()
{
  return *(TPointL*)&right;
}

//
// Return the width of the rectangle.
//
inline long
TRectL::Width() const
{
  return right-left;
}

//
// Return the height of the rectangle.
//
inline long
TRectL::Height() const
{
  return bottom-top;
}

//
// Return the size of rectangle.
//
inline ClassLib::TSize
TRectL::Size() const
{
	ClassLib::TSize sz;
	sz.cx=Width();
	sz.cy=Height();
	return sz;
}

//
// Return the area of th rectangle.
//
inline long
TRectL::Area() const
{
	return long(Width())*long(Height());
}

//
// Set all the data members to 0.
//
inline void
TRectL::SetNull()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

//
// Explicitly set all of the coordinates of the rectangle.
//
inline void
TRectL::Set(long _left, long _top, long _right, long _bottom)
{
	left = _left;
	top = _top;
	right = _right;
	bottom = _bottom;
}

//
// Determine the rectangle, given its upperleft point, width, and height
//
inline void
TRectL::SetWH(long _left, long _top, long w, long h)
{
	left = _left;
	top = _top;
	right = _left + w;
	bottom = _top + h;
}

//
// Copy from an existing rectangle.
//
inline
TRectL::TRectL(const tagRECT far& rect)
{
			 left = rect.left;
			 top = rect.top;
			 right = rect.right;
			 bottom = rect.bottom;
}

inline
TRectL::TRectL(const tagRECTL far& rect)
{
	*(tagRECTL far*)this = rect;
}


//
// Copy from an existing rectangle.
//
inline
TRectL::TRectL(const TRectL far& rect)
{
	*(tagRECTL far*)this = *(tagRECTL far*)&rect;
}

inline
TRectL::TRectL(const ClassLib::TRect far& rect)
{
			 left = rect.left;
			 top = rect.top;
			 right = rect.right;
			 bottom = rect.bottom;
}


//
// Constructor that sets all the values explicitly.
//
inline
TRectL::TRectL(long _left, long _top, long _right, long _bottom)
{
	Set(_left, _top, _right, _bottom);
}

//
// Determine a rectangle given two points.
//
inline
TRectL::TRectL(const TPointL& topLeft, const TPointL& bottomRight)
{
	Set(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

//
// Determine a rectangle given its upper left point and a dimension.
//
inline
TRectL::TRectL(const TPointL& origin, const ClassLib::TSize& extent)
{
	Set(origin.x, origin.y, origin.x + extent.cx, origin.y + extent.cy);
}

//
// Return true if the rectangle is empty.
//
inline bool
TRectL::IsEmpty() const
{
	return (left >= right || top >= bottom);
}

//
// Return true if all of the points on the rectangle is 0.
//
inline bool
TRectL::IsNull() const
{
	return (bool)(!left && !right && !top && !bottom);
}

//
// A rectangle is equal to another if all of their points are identical.
//
inline bool
TRectL::operator ==(const TRectL& other) const
{
	return (other.left==left   && other.top==top
						 && other.right==right && other.bottom==bottom);
}

//
// Return the opposite of operator ==.
//
inline bool
TRectL::operator !=(const TRectL& other) const
{
	return (!(other==*this));
}

//
// Return true if the rectangle contains the point.
//
inline bool
TRectL::Contains(const TPointL& point) const
{
  return (point.x >= left && point.x < right
             && point.y >= top  && point.y < bottom);
}

//
// Return true if the rectangle complete surrounds the other.
//
inline bool
TRectL::Contains(const TRectL& other) const
{
  return (other.left >= left && other.right <= right
             && other.top >= top   && other.bottom <= bottom);
}

//
// Return true if the intersection of the two rectangles is not null.
//
inline bool
TRectL::Touches(const TRectL& other) const
{
	return (other.right > left && other.left < right
						 && other.bottom > top && other.top < bottom);
}

//
// Move the points of the rectangle by the offsets.
//
inline TRectL
TRectL::OffsetBy(long dx, long dy) const
{
	return TRectL(left+dx, top+dy, right+dx, bottom+dy);
}

//
// Move the points of the rectangle by the offsets.
//
inline TRectL
TRectL::operator +(const ClassLib::TSize& size) const
{
	return OffsetBy(size.cx, size.cy);
}

//
// Move the points of the rectangle negatively by the offsets.
//
inline TRectL
TRectL::operator -(const ClassLib::TSize& size) const
{
	return OffsetBy(-size.cx, -size.cy);
}

//
// Move the upper left point of the rectangle while maintaining the
// current dimension.
//
inline TRectL
TRectL::MovedTo(long x, long y)
{
	return TRectL(x, y, x + Width(), y + Height());
}

//
// Inflate the size of the rectangle.
//
inline TRectL
TRectL::InflatedBy(long dx, long dy) const
{
  return TRectL(left-dx, top-dy, right+dx, bottom+dy);
}

//
// Inflate the size of the rectangle.
//
inline TRectL
TRectL::InflatedBy(const ClassLib::TSize& size) const
{
  return InflatedBy(size.cx, size.cy);
}

//
// Normalize the rectangle, such that left is less than  right and
// top is less than bottom.
//
inline TRectL
TRectL::Normalized() const
{
  return TRectL(Min(left, right), Min(top, bottom),
               Max(left, right), Max(top, bottom));
}

//
// Return the intersection of two rectangles.
//
inline TRectL
TRectL::operator &(const TRectL& other) const
{
  if (Touches(other))
    return TRectL(Max(left, other.left), Max(top, other.top),
                 Min(right, other.right), Min(bottom, other.bottom));
  return TRectL(0, 0, 0, 0);
}

//
// Return the union of two rectangles.
//
inline TRectL
TRectL::operator |(const TRectL& other) const
{
  return TRectL(Min(left, other.left), Min(top, other.top),
               Max(right, other.right), Max(bottom, other.bottom));
}

//
// Move the rectangle by delta.
//
inline TRectL&
TRectL::operator +=(const ClassLib::TSize& delta)
{
	Offset(delta.cx, delta.cy);
	return *this;
}

//
// Move the rectangle by negative delta.
//
inline TRectL&
TRectL::operator -=(const ClassLib::TSize& delta)
{
	ClassLib::TSize dd;
	dd=delta;
	dd.cx*=-1;
	dd.cy*=-1;
	return *this += dd;
}

//
// Move the upper left corner of the rectangle to a new location and
// maintain the current dimension.
//
inline TRectL&
TRectL::MoveTo(long x, long y)
{
	right = x + Width();
	bottom = y + Height();
	left = x;
	top = y;
	return *this;
}

//
// Inflate the size of the rectangle by delta.
//
inline TRectL&
TRectL::Inflate(const ClassLib::TSize& delta)
{
  return Inflate(delta.cx, delta.cy);
}
////////////////////////////////////////////////////////////////////////////////
#endif