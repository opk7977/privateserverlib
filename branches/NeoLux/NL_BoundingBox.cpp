#include "NL_BoundingBox.h"
#include "NL_Debug.h"

NDebug* NBoundingBox::pDebug = 0;

NBoundingBox::NBoundingBox(void)	
{
	if(!pDebug)
		pDebug = I_NDebug;
}

NBoundingBox::~NBoundingBox(void)
{
}

void NBoundingBox::Init()
{
	if( !pDebug )
		pDebug = I_NDebug;
}

void NBoundingBox::Init(const NVECTOR3& vMin,const NVECTOR3& vMax)
{
	Init();

	this->vMin = vMin;
	this->vMax = vMax;
}

void NBoundingBox::Render(NMATRIX& matWorld)
{
	pDebug->DrawBoundingBox( *this, matWorld );
}

void NBoundingBox::ResetYpos(float yMin, float yMax)
{
	if(vMin.y > yMin)	vMin.y = yMin;
	if(vMax.y < yMax)	vMax.y = yMax;
}

bool NBoundingBox::IsIntersect( const NBoundingBox& bbox ) const
{
	if ( vMax.x < bbox.vMin.x )	return false;
	if ( vMin.x > bbox.vMax.x ) return false;
	if ( vMax.y < bbox.vMin.y )	return false;
	if ( vMin.y > bbox.vMax.y )	return false;
	if ( vMax.z < bbox.vMin.z )	return false;
	if ( vMin.z > bbox.vMax.z )	return false;

	return true;
}